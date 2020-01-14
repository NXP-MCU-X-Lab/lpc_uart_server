#include <rtthread.h>
#include "common.h"
#include "uart.h"
#include "uart_bridge.h"
#include "usbd.h"

#ifdef FINSH_USING_MSH
#include <finsh.h>

static int reboot(int argc, char** argv)
{
    NVIC_SystemReset();
    return 0;
}
MSH_CMD_EXPORT(reboot, reboot);



static int cpuinfo(int argc, char** argv)
{
    uint32_t clock;
    rt_kprintf("LPC54606\r\n");
    
    clock = GetClock(kCoreClock);
    rt_kprintf("CoreClock:%dHz\r\n", clock);
    
    uint32_t tick = rt_tick_get();
    rt_kprintf("tick:%d, time:%ds\r\n", tick, tick/RT_TICK_PER_SECOND);
    return 0;
}
MSH_CMD_EXPORT(cpuinfo, cpuinfo);

static int uloop(int argc, char** argv)
{
    int i, instance, val;
    rt_kprintf("uloop\r\n");

    if(argc == 1)
    {
        for(i=0; i<10; i++)
        {
            rt_kprintf("uart%d loop:%d\r\n", i, UART_GetLoopbackMode(i));
        }
    }
    
    if(argc == 3)
    {
        instance = strtoul(argv[1], NULL, NULL);
        val = strtoul(argv[2], NULL, NULL);
        UART_SetLoopbackMode(instance, val);
        rt_kprintf("set: uart%d loop:%d\r\n", instance, UART_GetLoopbackMode(instance));
    }
    
    return 0;
}
MSH_CMD_EXPORT(uloop, set uart loopback mode);

extern uart_bridge_t bridge[10];
extern stat_t stat;

static int ustat(int argc, char** argv)
{
    int i;
    rt_kprintf("uart stat\r\n");

    if(argc == 2)
    {
        for(i=0; i<10; i++)
        {
            bridge[i].tx_sum = 0;
            bridge[i].rx_sum = 0;
            bridge[i].rx_dma_sum = 0;
        }
    }
    
    for(i=0; i<10; i++)
    {
        rt_kprintf("uart[%d] down:%d up:%d rx_dma:%d tbuf:%d rbuf:%d\r\n", i, bridge[i].tx_sum, bridge[i].rx_sum, bridge[i].rx_dma_sum, bridge_uart_tx_get_free(i), bridge_uart_rx_get_free(i));
    }
    
    rt_kprintf("total out:%d B/s\r\n", stat.total_out_speed);
    rt_kprintf("total  in:%d B/s\r\n", stat.total_in_speed);
    
    return 0;
}
MSH_CMD_EXPORT(ustat, uart stat info);


static int epsend(int argc, char** argv)
{
    uint8_t ep = strtoul(argv[1], NULL, NULL);
    uint8_t data = strtoul(argv[2], NULL, NULL);
    rt_kprintf("ep%d send :0x%X\r\n", ep, data);
    
    uint8_t buf[4];
    buf[0] = 13;
    buf[1] = data;
    buf[2] = 0;
    buf[3] = 0;
    usbd_ep_write(2, buf, sizeof(buf));
    
    return 0;
}
MSH_CMD_EXPORT(epsend, send ep data);

static int test_uart_tx(int argc, char** argv)
{
    uint8_t chl = strtoul(argv[1], NULL, NULL);
    bridge_uart_send(chl, "123456", 6);
}
MSH_CMD_EXPORT(test_uart_tx, test uart tx send);


static int led(int argc, char** argv)
{
    uint8_t idx = strtoul(argv[1], NULL, NULL);
    uint8_t val = strtoul(argv[2], NULL, NULL);
    set_led(idx, val);
}
MSH_CMD_EXPORT(led, test led);

#define CPU_USAGE_CALC_TICK    10
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

static void cpu_usage_idle_hook()
{
    rt_tick_t tick;
    rt_uint32_t count;
    volatile rt_uint32_t loop;

    if (total_count == 0)
    {
        /* get total count */
        rt_enter_critical();
        tick = rt_tick_get();
        while(rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
        {
            total_count ++;
            loop = 0;
            while (loop < CPU_USAGE_LOOP) loop ++;
        }
        rt_exit_critical();
    }

    count = 0;
    /* get CPU usage */
    tick = rt_tick_get();
    while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
    {
        count ++;
        loop  = 0;
        while (loop < CPU_USAGE_LOOP) loop ++;
    }

    /* calculate major and minor */
    if (count < total_count)
    {
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    }
    else
    {
        total_count = count;

        /* no CPU usage */
        cpu_usage_major = 0;
        cpu_usage_minor = 0;
    }
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);

    *major = cpu_usage_major;
    *minor = cpu_usage_minor;
}

static int cpu(int argc, char** argv)
{
    printf("%-16s%d.%d%%\r\n", "cpu ", cpu_usage_major, cpu_usage_minor);
    return 0;
}
MSH_CMD_EXPORT(cpu, cpu usage);

void cpu_usage_init(void)
{
    /* set idle thread hook */
    rt_thread_idle_sethook(cpu_usage_idle_hook);
}


#endif


