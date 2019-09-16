#include <rtthread.h>
#include "uart_bridge.h"
#include "common.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"

void _setup_dma_tx_desc(uint8_t ch, DMA_ChlSetup_t *setup);
void _setup_dma_rx_desc(uint8_t ch, DMA_ChlSetup_t *setup);


uint8_t rx_buf0[128];
DMA_ChlSetup_t rx_dma0;

void uart_dma_test_thread_entry(void* parameter)
{
    int sum, len;
    
    printf("uart_dma_test_thread_entry\r\n");
    
    UART_SetLoopbackMode(HW_UART4, false);
    UART_SetIntMode(HW_UART4, kUART_IntRx, false);

    /* setup rx */
    rx_dma0.dAddr = (uint32_t)rx_buf0;
    rx_dma0.transferCnt = sizeof(rx_buf0);
    _setup_dma_rx_desc(4, &rx_dma0);

  //  DMA_SetChlIntMode(DMAREQ_FLEXCOMM4_RX, true);
    DMA_SWTrigger(DMAREQ_FLEXCOMM4_RX);
    
    sum = 0;
    while(1)
    {
        while(DMA0->COMMON[0].BUSY) {};
        DMA0->COMMON[0].ENABLECLR = (1 << DMAREQ_FLEXCOMM4_RX);
            
        len = sizeof(rx_buf0) - DMA_GetTransferCnt(DMAREQ_FLEXCOMM4_RX);
        sum += len;
        
        DMA_SetupChl(&rx_dma0);
        DMA_SWTrigger(DMAREQ_FLEXCOMM4_RX);
        if(len)
        {
            printf("len:%d sum%d\r\n", len, sum);
        }
        
        DelayMs(2);
    }
}


void DMA0_IRQHandler(void)
{
    int i;
    static uint32_t INTA;
    INTA = DMA0->COMMON[0].INTA;
    
    /* clear int */
    DMA0->COMMON[0].INTA = DMA0->COMMON[0].INTA;

    if(INTA & (1 << DMAREQ_FLEXCOMM4_RX))
    {
//        rx_dma.dAddr = (uint32_t)rx_buf;
//        rx_dma.transferCnt = 4;
//        _setup_dma_rx_desc(DMAREQ_FLEXCOMM4_RX, &rx_dma);
//        DMA_SWTrigger(DMAREQ_FLEXCOMM4_RX);
//        printf("%X %X\r\n", rx_buf[0], rx_buf[1]);
    }
    
}

