#ifndef __UART_BRIDGE_H_
#define	__UART_BRIDGE_H_


#include <rtthread.h>
#include "rtdevice.h"
#include <dma.h>


typedef struct
{
    uint32_t total_out;
    uint32_t total_out_speed;
    uint32_t total_in;
    uint32_t total_in_speed;
}stat_t;

typedef struct
{
    struct rt_ringbuffer trb;   /* tx ring buffer instance */
    uint8_t trb_buf[512];      /* tx ring buffer */
    uint8_t tx_idle;            /* if uart tx is idle */
    int     tx_dma_complete_timeout;
    
    struct rt_ringbuffer rrb;   /* rx ring buffer instance */
    uint8_t rrb_buf[253*8];      /* rx ring buffer */
    
    DMA_ChlSetup_t tx_setup;
    DMA_ChlSetup_t rx_setup;

    uint8_t tbuf[512];          /* tmp buffer for serial tx to send */
    
    uint8_t rbuf[256];          /* tmp buffer for tx to send */
    uint8_t rdma_buf[1024];      /* tmp buffer for rx to received */
    
    uint32_t tx_sum;
    uint32_t rx_sum;
    uint32_t rx_dma_sum;
}uart_bridge_t;

uint32_t bridge_uart_send(uint8_t chl, uint8_t *buf, uint32_t size);
uint32_t bridge_uart_tx_get_free(uint8_t ch);
uint32_t bridge_uart_rx_get_free(uint8_t ch);
void bridge_uart_usb_data_in_ready(void);
void bridge_uart_usb_ctl_in_ready(void);



#endif
