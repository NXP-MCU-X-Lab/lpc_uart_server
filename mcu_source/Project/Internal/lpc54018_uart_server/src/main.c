#include <stdio.h>
#include <string.h>
#include <rthw.h>
#include <stdlib.h>
#include "common.h"
#include "gpio.h"
#include "syscon.h"
#include "shell.h"
#include "dma.h"
#include "uart.h"
#include "rtthread.h"

#define LOG_TAG              "example"
#define LOG_LVL              LOG_LVL_DBG
#include "ulog.h"


int rt_hw_uart_init(const char *name);
void rt_usbd_init(void);
void serial_tx_thread_entey(void* parameter);
void usb_data_in_thread_entry(void* parameter);
void usb_ctl_in_thread_entry(void* parameter);
void serial_rx_thread_entry(void* parameter);
void uart_dma_test_thread_entry(void* parameter);
    
void cpu_usage_init(void);

void init_thread_entry(void* parameter)
{
    int i;
    rt_thread_t tid;
    cpu_usage_init();
    rt_show_version();

//    LOG_I("ResetCause:0x%08X", GetResetCause());
//    LOG_I("kCoreClock:%dHz", GetClock(kCoreClock));
//    LOG_I("kFROHfClock:%dHz", GetClock(kFROHfClock));
//    LOG_I("kFROLfClock:%dHz", GetClock(kFROLfClock));

//    rt_hw_uart_init("uart4");
//    
//    rt_device_t u = rt_device_find("uart4");
//    rt_device_open(u, RT_DEVICE_FLAG_DMA_TX | RT_DEVICE_OFLAG_RDWR);

//    rt_device_write(u, 0, "1234567890 ", 10);
//    rt_device_write(u, 0, "1234567890 ", 10);
//    
//    while(1)
//    {
//        rt_thread_delay(1);
//    }
    
    rt_usbd_init();
    
    DMA_Init();
    
    /* loopback */
    for(i=1; i<10; i++)
    {
        UART_Init(i, 115200);
        UART_SetDMAMode(i, kUART_DMARx, true);
    }

    tid = rt_thread_create("serial_tx", serial_tx_thread_entey, RT_NULL, 256, 11, 20);
    rt_thread_startup(tid);
    
    rt_thread_delay(10);

    tid = rt_thread_create("usb_data_in", usb_data_in_thread_entry, RT_NULL, 512, 9, 20);
    rt_thread_startup(tid);
    
    tid = rt_thread_create("usb_ctl_in", usb_ctl_in_thread_entry, RT_NULL, 512, 16, 20);
    rt_thread_startup(tid);

    tid = rt_thread_create("serial_rx", serial_rx_thread_entry, RT_NULL, 256, 12, 20);
    rt_thread_startup(tid);

//    tid = rt_thread_create("test", uart_dma_test_thread_entry, RT_NULL, 512, 11, 20);
//    rt_thread_startup(tid);
    
    while(1)
    {
        rt_thread_delay(100);
    }

}


void rt_application_init(void* parameter)
{
    rt_thread_t tid;
    
    /* FC0 */
    SetPinMux(HW_GPIO0,  29, 1);    /* UART0 RX */
    SetPinMux(HW_GPIO0,  30, 1);    /* UART0 TX */
    
    /* FC1  */
    SetPinMux(HW_GPIO1,  11, 2);
    SetPinMux(HW_GPIO1,  10, 2);
    SetPinMux(HW_GPIO0,  13, 1);
    SetPinMux(HW_GPIO0,  14, 1);

    
    /* FC2  */
    SetPinMux(HW_GPIO1,  25, 1);
    SetPinMux(HW_GPIO1,  24, 1);
    SetPinMux(HW_GPIO1,  26, 1);
    SetPinMux(HW_GPIO1,  27, 1);
    
    
    /* FC3  */
    SetPinMux(HW_GPIO0,  2, 1);
    SetPinMux(HW_GPIO0,  3, 1);
    SetPinMux(HW_GPIO0,  1, 2);
    SetPinMux(HW_GPIO0,  7, 1);
    
    
    /* FC4  */
    SetPinMux(HW_GPIO3,  27, 3);
    SetPinMux(HW_GPIO3,  26, 3);
    SetPinMux(HW_GPIO3,  28, 3);
    SetPinMux(HW_GPIO3,  39, 3);
    
    /* FC5 */
    SetPinMux(HW_GPIO2,  13, 5);
    SetPinMux(HW_GPIO2,  12, 5);
    SetPinMux(HW_GPIO2,  14, 5);
    SetPinMux(HW_GPIO2,  15, 5);
    
    
    /* FC6 */
    SetPinMux(HW_GPIO4,  3, 2);
    SetPinMux(HW_GPIO4,  2, 2);
    SetPinMux(HW_GPIO4,  0, 2);
    //SetPinMux(HW_GPIO0,  3, 1);
    
    
    /* FC7 */
    SetPinMux(HW_GPIO2,  20, 3);
    SetPinMux(HW_GPIO2,  19, 3);
    SetPinMux(HW_GPIO1,  21, 1);
    SetPinMux(HW_GPIO1,  20, 1);
    
    
    /* FC8 */
    SetPinMux(HW_GPIO3,  16, 1);
    SetPinMux(HW_GPIO3,  17, 1);
    SetPinMux(HW_GPIO3,  4, 3);
    SetPinMux(HW_GPIO3,  5, 3);
    
    
    /* FC9 */
    SetPinMux(HW_GPIO3,  3,  2);
    SetPinMux(HW_GPIO3,  2,  2);
    SetPinMux(HW_GPIO3,  30, 1);
    SetPinMux(HW_GPIO3,  31, 1);
    
    
    rt_hw_uart_init("uart0");
    rt_console_set_device("uart0");

    rt_components_board_init();
    rt_components_init();
    
    tid = rt_thread_create("init", init_thread_entry, RT_NULL, 512, 5, 20);
    rt_thread_startup(tid);
}

int main(void)
{
    SetFROClock(96*1000*1000, true);
    SetupSystemPLL(kPLLSrcCLKIN, 180*1000*1000);
    
	rt_hw_interrupt_disable();
    
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND - 1);
	rt_system_timer_init();
    rt_system_scheduler_init();

    rt_device_init_all();
    rt_system_timer_thread_init();
    rt_thread_idle_init();
    rt_system_heap_init((void*)0x20000000, (void*)(0x10000 + 0x20000000));  
    rt_application_init(RT_NULL);
    rt_system_scheduler_start();

    while(1)
    {
        DelayMs(500);
    }
}



void SysTick_Handler(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}


void DelayMs(uint32_t ms)
{
    rt_thread_delay(rt_tick_from_millisecond(ms));
}

void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
    delta = SysTick->VAL;
    while (delta - SysTick->VAL< us);
}


