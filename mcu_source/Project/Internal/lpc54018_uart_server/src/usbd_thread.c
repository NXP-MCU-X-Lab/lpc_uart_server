#include <rtthread.h>
#include "common.h"
#include "usbd.h"
#include "usbd_cdc.h"
#include "usbd_cp210x.h"
#include "usbd_digi_serial.h"

rt_mq_t msd_mq;
static struct usbd_t usbd;
void USBD_Connect (bool con);
void USBD_Init (void);
//struct ucdc_line_coding coding;

typedef struct
{
    uint8_t param;
    void (*exec)(uint32_t param);
}msd_msg_t;



///* host need device's line_coding config */
//uint32_t get_line_coding(struct ucdc_line_coding *line_coding)
//{
//    rt_memcpy(line_coding, &coding, sizeof(struct ucdc_line_coding));
//    USBD_TRACE("get_line_coding, baud:%d\r\n", line_coding->dwDTERate);
//    return CH_OK;
//}

///* device need to implment host's config */
//uint32_t set_line_coding(struct ucdc_line_coding *line_coding)
//{
//    rt_memcpy(&coding, line_coding, sizeof(struct ucdc_line_coding));
//    USBD_TRACE("set line coding:%d\r\n", line_coding->dwDTERate);
//    return CH_OK;
//}


//uint32_t cdc_data_received(uint8_t *buf, uint32_t len)
//{
//    int i;
//    for(i=0; i<len; i++)
//    {
//        rt_kprintf("%c", buf[i]);
//    }
//    return CH_OK;
//}

//uint32_t set_control_line_serial_state(uint8_t val)
//{
//    printf("set_control_line_serial_state %d\r\n", val);
//    return CH_OK;
//}

//uint32_t send_notify(void)
//{
//    return CH_OK;
//}

//struct usbd_cdc_callback_t cdc_cb = 
//{
//    get_line_coding,
//    set_line_coding,
//    set_control_line_serial_state,
//    cdc_data_received,
//    send_notify,
//};


void usbd_thread(void* parameter)
{
    msd_msg_t msg;
    msd_mq = rt_mq_create("mq", sizeof(msd_msg_t), 20, RT_IPC_FLAG_FIFO);
    
    /* VBUS LPC54608 */
    IOCON->PIO[0][22] = IOCON_PIO_FUNC(7) | IOCON_PIO_DIGIMODE_MASK | IOCON_PIO_MODE(0);
    
    ch_usb_init(&usbd);
  //  usbd_cdc_set_cb(&cdc_cb);
    //usbd_cdc_init(&usbd);
   // usbd_vsc_cp210x_init(&usbd);
    usbd_vsc_digi_serial_init(&usbd);
    
    USBD_Init();
    USBD_Connect(true);

    while(1)
    {
        /* handling usbd sof event */
        if(rt_mq_recv(msd_mq, &msg, sizeof(msd_msg_t), RT_WAITING_FOREVER) == RT_EOK)
        {
            usbd_data_ep_handler(msg.param/2, msg.param % 2);
        }
    }
    
}

void rt_usbd_init(void)
{
    rt_thread_t tid = rt_thread_create("usbd", usbd_thread, RT_NULL, 1024, 13, 20);
    rt_thread_startup(tid);
}


