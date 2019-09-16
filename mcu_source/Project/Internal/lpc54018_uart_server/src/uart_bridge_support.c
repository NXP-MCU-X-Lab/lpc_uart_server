#include "common.h"
#include "uart.h"
#include "dma.h"
#include "ulog.h"


void _setup_dma_tx_desc(uint8_t ch, DMA_ChlSetup_t *setup)
{
    setup->chl = DMAREQ_FLEXCOMM0_RX + ch*2 + 1;
    
    if(ch == 8)
    {
        setup->chl = DMAREQ_FLEXCOMM8_TX;
    }
    
    if(ch == 9)
    {
        setup->chl = DMAREQ_FLEXCOMM9_TX;
    }
    
    setup->isPeriph = true;
    setup->dataWidth = 1;
    
    setup->sAddrInc = 1;
    setup->dAddrInc = 0;
    
    switch(setup->chl)
    {
        case DMAREQ_FLEXCOMM0_TX:
            setup->dAddr = (uint32_t)(&(USART0->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM1_TX:
            setup->dAddr = (uint32_t)(&(USART1->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM2_TX:
            setup->dAddr = (uint32_t)(&(USART2->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM3_TX:
            setup->dAddr = (uint32_t)(&(USART3->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM4_TX:
            setup->dAddr = (uint32_t)(&(USART4->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM5_TX:
            setup->dAddr = (uint32_t)(&(USART5->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM6_TX:
            setup->dAddr = (uint32_t)(&(USART6->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM7_TX:
            setup->dAddr = (uint32_t)(&(USART7->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM8_TX:
            setup->dAddr = (uint32_t)(&(USART8->FIFOWR));
            break;
        case DMAREQ_FLEXCOMM9_TX:
            setup->dAddr = (uint32_t)(&(USART9->FIFOWR));
            break;
        default:
            LOG_I("_setup_dma_tx_desc failed %d\r\n", setup->chl);
            break;
    }
    
    DMA_SetupChl(setup);
}

void _setup_dma_rx_desc(uint8_t ch, DMA_ChlSetup_t *setup)
{
    setup->chl = DMAREQ_FLEXCOMM0_RX + ch*2;
    
    if(ch == 8)
    {
        setup->chl = DMAREQ_FLEXCOMM8_RX;
    }
    
    if(ch == 9)
    {
        setup->chl = DMAREQ_FLEXCOMM9_RX;
    }
    
    setup->isPeriph = true;
    setup->dataWidth = 1;
    
    setup->sAddrInc = 0;
    setup->dAddrInc = 1;
    
    switch(setup->chl)
    {
        case DMAREQ_FLEXCOMM0_RX:
            setup->sAddr = (uint32_t)(&(USART0->FIFORD));
            break;
        case DMAREQ_FLEXCOMM1_RX:
            setup->sAddr = (uint32_t)(&(USART1->FIFORD));
            break;
        case DMAREQ_FLEXCOMM2_RX:
            setup->sAddr = (uint32_t)(&(USART2->FIFORD));
            break;
        case DMAREQ_FLEXCOMM3_RX:
            setup->sAddr = (uint32_t)(&(USART3->FIFORD));
            break;
        case DMAREQ_FLEXCOMM4_RX:
            setup->sAddr = (uint32_t)(&(USART4->FIFORD));
            break;
        case DMAREQ_FLEXCOMM5_RX:
            setup->sAddr = (uint32_t)(&(USART5->FIFORD));
            break;
        case DMAREQ_FLEXCOMM6_RX:
            setup->sAddr = (uint32_t)(&(USART6->FIFORD));
            break;
        case DMAREQ_FLEXCOMM7_RX:
            setup->sAddr = (uint32_t)(&(USART7->FIFORD));
            break;
        case DMAREQ_FLEXCOMM8_RX:
            setup->sAddr = (uint32_t)(&(USART8->FIFORD));
            break;
        case DMAREQ_FLEXCOMM9_RX:
            setup->sAddr = (uint32_t)(&(USART9->FIFORD));
            break;
        default:
            LOG_I("_setup_dma_rx_desc failed %d\r\n", ch);
            break;
    }
    
    DMA_SetupChl(setup);
}


//static void uart_irq_handler(uint8_t chl)
//{
//    uint8_t ch;
//    if(UART_GetChar(chl, &ch) == CH_OK)
//    {
//        rt_ringbuffer_put(&bridge[chl].rrb, &ch, 1);
//    }
//}

//void FLEXCOMM1_IRQHandler(void)
//{
//    uart_irq_handler(1);
//}

//void FLEXCOMM2_IRQHandler(void)
//{
//    uart_irq_handler(2);
//}
//void FLEXCOMM3_IRQHandler(void)
//{
//    uart_irq_handler(3);
//}

//void FLEXCOMM4_IRQHandler(void)
//{
//    uart_irq_handler(4);
//}

//void FLEXCOMM5_IRQHandler(void)
//{
//    uart_irq_handler(5);
//}

//void FLEXCOMM6_IRQHandler(void)
//{
//    uart_irq_handler(6);
//}

//void FLEXCOMM7_IRQHandler(void)
//{
//    uart_irq_handler(7);
//}

//void FLEXCOMM8_IRQHandler(void)
//{
//    uart_irq_handler(8);
//}

//void FLEXCOMM9_IRQHandler(void)
//{
//    uart_irq_handler(9);
//}

