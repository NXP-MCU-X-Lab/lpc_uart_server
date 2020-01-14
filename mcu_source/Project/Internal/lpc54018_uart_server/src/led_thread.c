#include "rtthread.h"
#include "rtdevice.h"
#include "gpio.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

typedef struct
{
    uint8_t port;
    uint8_t pin;
} led_t;

led_t leds[] = 
{
    {HW_GPIO3, 6},
    {HW_GPIO3, 7},
    {HW_GPIO3, 8},
    {HW_GPIO3, 9},
    {HW_GPIO3, 10},
    {HW_GPIO3, 11},
    {HW_GPIO3, 12},
    {HW_GPIO3, 13},
    {HW_GPIO3, 14},
    {HW_GPIO3, 15},
};

static uint32_t led_flag = 0;
static uint32_t led_idle_cnt[10];

void set_led(uint8_t idx, uint8_t val)
{
    led_flag |= (1<<idx);
}

void thread_led_entry(void* parameter)
{
    int i;
    
    /* init led pins */
    GPIO_Init(HW_GPIO1, 1, kGPIO_OPPH); 
    
    for(i=0; i<ARRAY_SIZE(leds); i++)
    {
        GPIO_Init(leds[i].port, leds[i].pin, kGPIO_OPPH); 
    }
    
    while(1)
    {
        for(i=0; i<ARRAY_SIZE(leds); i++)
        {
            if(led_flag & (1<<i))
            {
                GPIO_PinWrite(leds[i].port, leds[i].pin, 0);
                led_idle_cnt[i]++;
            }
            
            if(led_idle_cnt[i] > 2)
            {
                led_idle_cnt[i] = 0;
                led_flag &= ~(1<<i);
                GPIO_PinWrite(leds[i].port, leds[i].pin, 1);
            }
        }
        
        GPIO_PinToggle(HW_GPIO1, 1);
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
}
