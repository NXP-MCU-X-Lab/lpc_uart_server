#include <rtthread.h>
#include "uart_bridge.h"
#include "common.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "rtdevice.h"
#include "usbd.h"
#include "ulog.h"


void set_led(uint8_t idx, uint8_t val);
void _setup_dma_tx_desc(uint8_t ch, DMA_ChlSetup_t *setup);
void _setup_dma_rx_desc(uint8_t ch, DMA_ChlSetup_t *setup);

static rt_sem_t utx_dma_complete;
rt_mutex_t usb_lock;
static int usb_in_ready = 1;
int usb_ctl_need_report_stat = 0;
int usb_report_stat_chl = 1;

uart_bridge_t bridge[10];
stat_t stat;



static void stat_timer_cb(void* parameter)
{
    /* caluate total OUT and IN speed */
    stat.total_in_speed = stat.total_in;
    stat.total_in = 0;
    
    stat.total_out_speed = stat.total_out;
    stat.total_out = 0;
}


void uart_bridge_init(uint8_t ch, uart_bridge_t *handle)
{
    /* tx */
    rt_ringbuffer_init(&handle->trb, handle->trb_buf, sizeof(handle->trb_buf));
    _setup_dma_tx_desc(ch, &handle->tx_setup);
    DMA_SetChlIntMode(handle->tx_setup.chl, true);
    handle->tx_idle = 1;
    handle->tx_sum = 0;
    handle->tx_dma_complete_timeout = 0;
    
    /* rx */
    handle->rx_sum = 0;
    handle->rx_dma_sum = 0;
    rt_ringbuffer_init(&handle->rrb, handle->rrb_buf, sizeof(handle->rrb_buf));
    handle->rx_setup.transferCnt = sizeof(handle->rdma_buf);
    handle->rx_setup.dAddr = (uint32_t)handle->rdma_buf;
    _setup_dma_rx_desc(ch, &handle->rx_setup);
    DMA_SWTrigger(handle->rx_setup.chl);
    
}

uint32_t bridge_uart_send(uint8_t ch, uint8_t *buf, uint32_t size)
{
    return rt_ringbuffer_put(&bridge[ch].trb, buf, size);
}

uint32_t bridge_uart_tx_get_free(uint8_t ch)
{
    return rt_ringbuffer_space_len(&bridge[ch].trb);
}

uint32_t bridge_uart_rx_get_free(uint8_t ch)
{
    return rt_ringbuffer_space_len(&bridge[ch].rrb);
}

void bridge_uart_usb_data_in_ready(void)
{
    usb_in_ready = 1;
}




void serial_tx_thread_entey(void* parameter)
{
    int i;
    uint32_t len;
    for(i=1; i<ARRAY_SIZE(bridge); i++)
    {
        uart_bridge_init(i, &bridge[i]);
    }
    
    rt_timer_start(rt_timer_create("stat", stat_timer_cb, RT_NULL, rt_tick_from_millisecond(1000), RT_TIMER_FLAG_PERIODIC));
    utx_dma_complete = rt_sem_create("tx", 1, RT_IPC_FLAG_FIFO);
    usb_lock = rt_mutex_create("ulock", RT_IPC_FLAG_FIFO);
    
    while(1)
    {
        rt_sem_take(utx_dma_complete, 1);
        
        for(i=1; i< ARRAY_SIZE(bridge); i++)
        {
            if(bridge[i].tx_idle)
            {
                bridge[i].tx_dma_complete_timeout = 0;
                len = rt_ringbuffer_get(&bridge[i].trb, bridge[i].tbuf, sizeof(bridge[i].tbuf));
                if(len)
                {
                    bridge[i].tx_idle = 0;
                    
                    bridge[i].tx_setup.sAddr = (uint32_t)bridge[i].tbuf;
                    bridge[i].tx_setup.transferCnt = len;
                    set_led(i, 0);
                    DMA_SetupChl(&bridge[i].tx_setup);
                    DMA_SWTrigger(bridge[i].tx_setup.chl);
                    bridge[i].tx_sum += len;
                    
                    rt_thread_delay(1);
                    //printf("down[%d] len:%d sum:%d\r\n", i, len, bridge[i].tx_sum );
                }
            }
            else
            {
                bridge[i].tx_dma_complete_timeout ++;
            }
            
            if(bridge[i].tx_dma_complete_timeout > 1000)
            {
                bridge[i].tx_idle = 1;
            }
        }
        
    }
}


void serial_rx_thread_entry(void* parameter)
{
    int i;
    int rx_dma_cnt;
    int timeout;
    
    while(1)
    {
        for(i=1; i< ARRAY_SIZE(bridge); i++)
        {
            timeout = 0;
            while(DMA0->COMMON[0].BUSY & (1 << bridge[i].rx_setup.chl))
            {
                timeout++;
                if(timeout > 10000)
                {
                    break;
                }
            }
            
            DMA0->COMMON[0].ENABLECLR = (1 << bridge[i].rx_setup.chl);

            rx_dma_cnt = sizeof(bridge[i].rdma_buf) - DMA_GetTransferCnt(bridge[i].rx_setup.chl);
            
            if(rx_dma_cnt)
            {
                set_led(i, 0);
                stat.total_in += rx_dma_cnt;
                rt_ringbuffer_put(&bridge[i].rrb, bridge[i].rdma_buf, rx_dma_cnt);
                bridge[i].rx_dma_sum += rx_dma_cnt;
            }
            
            DMA_SetupChl(&bridge[i].rx_setup);
            DMA_SWTrigger(bridge[i].rx_setup.chl);
        }
        rt_thread_delay(1);
    }
}

void usb_ctl_in_thread_entry(void* parameter)
{
    int free;
    
    /* waitting for usb readly */
    while(usbd_is_configured() == false)
    {
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
    
    while(1)
    {
        uint8_t buf[4];
        buf[0] = 22;
        buf[1] = usb_report_stat_chl;
        buf[2] = 0;
        buf[3] = 0;
        
        free = bridge_uart_tx_get_free(usb_report_stat_chl);
        
        if(free < 100)
        {
            buf[2] = 1; /* full */
        }
        else
        {
            buf[2] = 0; /* empty */
        }

        if(usb_ctl_need_report_stat)
        {
            usb_ctl_need_report_stat = 0;
           // LOG_I("ctl in chl:%d %X\r\n", buf[1], buf[2]);
            usbd_ep_write(2, buf, sizeof(buf));
        }
        
        rt_thread_delay(1);
    }
    
}


void usb_data_in_thread_entry(void* parameter)
{
    int len, i;
    int usb_not_ready_cnt = 0;
    int min_free, min_free_chl, free;
    
    /* waitting for usb readly */
    while(usbd_is_configured() == false)
    {
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
    
    while(1)
    {
        min_free = 2048;
        min_free_chl = 1;
            
        /* find the buf is most likely full */
        for(i=1; i< ARRAY_SIZE(bridge); i++)
        {
            free = bridge_uart_rx_get_free(i);
            if(free < min_free)
            {
                min_free = free;
                min_free_chl = i;
            }
        }
        
        if(usb_in_ready)
        {
            usb_not_ready_cnt = 0;

             //len = rt_ringbuffer_get(&bridge[min_free_chl].rrb, bridge[min_free_chl].rbuf, sizeof(bridge[min_free_chl].rbuf));
            len = rt_ringbuffer_get(&bridge[min_free_chl].rrb, &bridge[min_free_chl].rbuf[3], sizeof(bridge[min_free_chl].rbuf)-3);
            if(len)
            {
                usb_in_ready = 0;
                bridge[min_free_chl].rx_sum += len;
                
                //printf("up[%d] len:%d sum:%d\r\n", min_free_chl, len, bridge[min_free_chl].rx_sum);
                bridge[min_free_chl].rbuf[0] = 19; /* DIGI_CMD_RECEIVE_DATA */
                bridge[min_free_chl].rbuf[1] = min_free_chl;
                bridge[min_free_chl].rbuf[2] = len;
                
                usbd_ep_write(1, bridge[min_free_chl].rbuf, len+3);
                //ulog_hexdump("TEST", 16, bridge[min_free_chl].rbuf, len);
                //usbd_ep_write(1, bridge[min_free_chl].rbuf, len);
               // rt_thread_delay(1);
            }
            else
            {
                rt_thread_delay(1);
            }
           
//            if(usb_in_ready)
//            {
//                rt_thread_delay(1);
//            }
        }
        else
        {
            rt_thread_delay(1);
            usb_not_ready_cnt++;
            
            if(usb_not_ready_cnt > 1000)
            {
                usb_not_ready_cnt = 0;
                usb_in_ready = 1;
            }
        }
        
    }
}


void DMA0_IRQHandler(void)
{
    int i;
    static volatile uint32_t INTA;
    
    INTA = DMA0->COMMON[0].INTA;
    DMA0->COMMON[0].INTA = DMA0->COMMON[0].INTA;
    
    if(INTA & (1 << DMAREQ_FLEXCOMM8_TX))
    {
        bridge[8].tx_idle = 1;
    }
    
    if(INTA & (1 << DMAREQ_FLEXCOMM9_TX))
    {
        bridge[9].tx_idle = 1;
    }
    
    for(i=1; i<ARRAY_SIZE(bridge); i++)
    {
        if(INTA & (1 << (i*2 + 1)))
        {
            bridge[i].tx_idle = 1;
        }
    }

    rt_sem_release(utx_dma_complete);
}

