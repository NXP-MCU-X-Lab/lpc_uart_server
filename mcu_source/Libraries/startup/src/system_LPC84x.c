/**************************************************************************//**
 * @file     system_LPC84x.c
 * @brief    CMSIS Device System Source File for
 *           NXP LPC84x Device Series
 * @version  V1.0.0
 * @date     19. April 2017
 ******************************************************************************/
/* Copyright (c) 2014 - 2017 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "LPC84x.h"

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*- SystemCoreClock Configuration -------------------------------------------*/
// <e0> SystemCoreClock Configuration
#define CLOCK_SETUP           1
//
//   <h> External Clock Select (EXTCLKSEL)
//     <o.0>      Sel: Clock source for external clock
//          <0=> System oscillator
//          <1=> CLK_IN
//   </h>
#define EXTCLKSEL_Val        0x00000000              // Reset value: 0x000
//
//   <h> System Oscillator Control (SYSOSCCTRL)
//     <o.0>      BYPASS: System Oscillator Bypass Enable
//                     <i> If enabled then PLL input (sys_osc_clk) is fed
//                     <i> directly from XTALIN and XTALOUT pins.
//     <o.1>      FREQRANGE: System Oscillator Frequency Range
//                     <i> Determines frequency range for Low-power oscillator.
//                   <0=> 1 - 20 MHz
//                   <1=> 15 - 25 MHz
//   </h>
#define SYSOSCCTRL_Val        0x00000000              // Reset value: 0x000
//
//   <h> FR Oscillator Setting (FROOSCCTRL)
//     <o.0..1> CORESEL: Select FRO frequency
//          <0=> 18 MHz clock output
//          <1=> 24 MHz clock output
//          <2=> 30 MHz clock output
//          <3=> 30 MHz clock output
//     <o.2..11> FRO_TRIM: FRO Trim value
//          <0-0x3FF>
//     <o.12..16> FRO_TEMP_TRIM: FRO Temperature Coefficient tuning
//          <0-0x1F>
//     <o.17> FRO_DIRECT: FRO clock is divided by 2 or 16 (depending on FAIM slow boot value)
//       <i>FRO clock is divided by 2 or 16 (depending on FAIM slow boot value) or is direct from FRO Oscillator
//          <0=> FRO clock is dvided
//          <1=> FRO clock is direct
//     <o.17> DEBUG_ENABLE: Enable FRO debug
//   </h>
#define FROOSCCTRL_Val        0x00000801              // Reset value: 0x0A0
//
//
//   <h> WDT Oscillator Setting (WDTOSCCTRL)
//     <o.0..4> DIVSEL: Select Divider for Fclkana
//            <i> wdt_osc_clk = Fclkana / (2 × (1 + DIVSEL))
//          <0-31>
//     <o.5..8> FREQSEL: Select WD Oscillator Analog Output Frequency (Fclkana)
//          <1=> 0.6 MHz
//          <2=> 1.05 MHz
//          <3=> 1.4 MHz
//          <4=> 1.75 MHz
//          <5=> 2.1 MHz
//          <6=> 2.4 MHz
//          <7=> 2.7 MHz
//          <8=> 3.0 MHz
//          <9=> 3.25 MHz
//          <10=> 3.5 MHz
//          <11=> 3.75 MHz
//          <12=> 4.0 MHz
//          <13=> 4.2 MHz
//          <14=> 4.4 MHz
//          <15=> 4.6 MHz
//   </h>
#define WDTOSCCTRL_Val        0x00000180              // Reset value: 0x0A0
//
//   <o.0..1> System PLL Clock Source Select (SYSPLLCLKSEL)
//        <0=> FRO
//        <1=> External clock
//        <2=> Watchdog oscillator
//        <3=> FRO DIV
#define SYSPLLCLKSEL_Val      0x00000000              // Reset value: 0x000
//
//   <e> Clock Configuration (Manual)
#define CLOCK_SETUP_REG       1
//
//     <o.0..1> Main Clock Source Select (MAINCLKSEL)
//          <0=> FRO
//          <1=> External clock
//          <2=> Watchdog oscillator
//          <3=> FRO DIV
#define MAINCLKSEL_Val      0x00000000               // Reset value: 0x000
//
//     <h> System PLL Setting (SYSPLLCTRL)
//              <i> F_clkout = M * F_clkin = F_CCO / (2 * P)
//              <i> F_clkin must be in the range of  10 MHz to  25 MHz
//              <i> F_CCO   must be in the range of 156 MHz to 320 MHz
//       <o.0..4> MSEL: Feedback Divider Selection
//              <i> M = MSEL + 1
//            <0-31>
//       <o.5..6> PSEL: Post Divider Selection
//              <i> Post divider ratio P. Division ratio is 2 * P
//            <0=> P = 1
//            <1=> P = 2
//            <2=> P = 4
//            <3=> P = 8
//     </h>
#define SYSPLLCTRL_Val        0x00000041              // Reset value: 0x000
//
//     <o.0..1> Main Clock PLL Source Select (MAINCLKPLLSEL)
//        <0=> main_clk_pre_pll
//        <1=> SYS PLL
#define MAINCLKPLLSEL_Val     0x00000001              // Reset value: 0x000
//
//     <o.0..7>   System AHB Clock Divider (SYSAHBCLKDIV.DIV)
//            <i> Divides main clock to provide system clock to core, memories, and peripherals.
//            <i> 0 = is disabled
//          <0-255>
#define SYSAHBCLKDIV_Val      0x00000001              // Reset value: 0x001
//   </e>
//
//   <e> Clock Configuration (via ROM PLL API)
#define CLOCK_SETUP_API       0
//
//     <o> PLL API Mode Select
//          <0=> Exact
//          <1=> Less than or equal
//          <2=> Greater than or equal
//          <3=> As close as possible
#define PLL_API_MODE_Val      3
//
//     <o> CPU Frequency [Hz]  <1000000-30000000:1000>
#define PLL_API_FREQ_Val      30000000
//   </e>
//
// <o0>System Oscillator (XTAL) Frequency [Hz] <1000000-25000000>
//          <i> XTAL frequency must be in the range of  1 MHz to  25 MHz
//
#define XTAL_CLK_Val          12000000ul

/*
//-------- <<< end of configuration section >>> ------------------------------
*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL_CLK     ( XTAL_CLK_Val)        /* Oscillator freq              */
#define __SYS_OSC_CLK  (   __XTAL_CLK)        /* System oscillator freq       */
#define __CLKIN_CLK    (   12000000UL)        /* CLKIN pin frequency          */

/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

#if (CHECK_RSVD((EXTCLKSEL_Val),  ~0x00000001))
   #error "EXTCLKSEL_Val: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((SYSOSCCTRL_Val),  ~0x00000003))
   #error "SYSOSCCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((FROOSCCTRL_Val),  ~0x0007FFFF))
   #error "FROOSCCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((WDTOSCCTRL_Val),  ~0x000001FF))
   #error "WDTOSCCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((SYSPLLCLKSEL_Val), 0, 3))
   #error "SYSPLLCLKSEL: Value out of range!"
#endif

#if (CHECK_RSVD((SYSPLLCTRL_Val),  ~0x0000007F))
   #error "SYSPLLCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((MAINCLKSEL_Val),  ~0x00000003))
   #error "MAINCLKSEL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((MAINCLKPLLSEL_Val),  ~0x00000003))
   #error "MAINCLKPLLSEL: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((SYSAHBCLKDIV_Val), 0, 255))
   #error "SYSAHBCLKDIV: Value out of range!"
#endif

#if ( CLOCK_SETUP_REG == CLOCK_SETUP_API )
   #error "You must select either manual or API based Clock Configuration!"
#endif

#if (CHECK_RANGE(XTAL_CLK_Val, 1000000, 25000000))
   #error "XTAL frequency is out of bounds"
#endif

#if (CHECK_RANGE(PLL_API_MODE_Val, 0, 3))
   #error "PLL API Mode Select not valid"
#endif

#if (CHECK_RANGE(PLL_API_FREQ_Val, 1000000, 30000000))
   #error "CPU Frequency (API mode) not valid"
#endif



/*----------------------------------------------------------------------------
  Calculate system core clock
 *----------------------------------------------------------------------------*/
  /* FRO clock calculation */
  #define __FRO_FREQ       ((FROOSCCTRL_Val      ) & 0x03)
  #define __FRO_DIRECT     ((FROOSCCTRL_Val >> 17) & 0x01)
  #define __FRO_DIV        (((__FRO_DIRECT + 1) & 0x01) + 1)

  #if   (__FRO_FREQ ==  0)
    #define __FRO_OSC_CLK          (18000000 / __FRO_DIV)
  #elif (__FRO_FREQ ==  1)
    #define __FRO_OSC_CLK          (24000000 / __FRO_DIV)
  #elif (__FRO_FREQ ==  2)
    #define __FRO_OSC_CLK          (30000000 / __FRO_DIV)
  #else
    #define __FRO_OSC_CLK          (30000000 / __FRO_DIV)
  #endif

#if (CLOCK_SETUP)                               /* Clock Setup */

  /* ext. clock calculation */
  #if   ((EXTCLKSEL_Val & 0x01) == 0)
    #define  __EXT_CLK              __SYS_OSC_CLK
  #else
    #define  __EXT_CLK              __CLKIN_CLK
  #endif

  /* WDT clock calculation */
  #define __WDT_FREQSEL   ((WDTOSCCTRL_Val >> 5) & 0x0F)
  #define __WDT_DIVSEL   (((WDTOSCCTRL_Val & 0x1F) + 1) << 1)

  #if  (__WDT_FREQSEL ==  0)
    #error "WDTOSCCTRL.FREQSEL undefined!"
  #elif (__WDT_FREQSEL ==  1)
    #define __WDT_OSC_CLK          ( 600000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  2)
    #define __WDT_OSC_CLK          (1050000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  3)
    #define __WDT_OSC_CLK          (1400000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  4)
    #define __WDT_OSC_CLK          (1750000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  5)
    #define __WDT_OSC_CLK          (2100000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  6)
    #define __WDT_OSC_CLK          (2400000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  7)
    #define __WDT_OSC_CLK          (2700000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  8)
    #define __WDT_OSC_CLK          (3000000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL ==  9)
    #define __WDT_OSC_CLK          (3250000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL == 10)
    #define __WDT_OSC_CLK          (3500000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL == 11)
    #define __WDT_OSC_CLK          (3750000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL == 12)
    #define __WDT_OSC_CLK          (4000000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL == 13)
    #define __WDT_OSC_CLK          (4200000 / __WDT_DIVSEL)
  #elif (__WDT_FREQSEL == 14)
    #define __WDT_OSC_CLK          (4400000 / __WDT_DIVSEL)
  #else
    #define __WDT_OSC_CLK          (4600000 / __WDT_DIVSEL)
  #endif

  /* sys_pllclkin calculation */
  #if   ((SYSPLLCLKSEL_Val & 0x03) == 0)
    #define __SYS_PLLCLKIN         (__FRO_OSC_CLK)
  #elif ((SYSPLLCLKSEL_Val & 0x03) == 1)
    #define __SYS_PLLCLKIN         (__EXT_CLK)
  #elif ((SYSPLLCLKSEL_Val & 0x03) == 2)
    #define __SYS_PLLCLKIN         (__WDT_OSC_CLK)
  #elif ((SYSPLLCLKSEL_Val & 0x03) == 3)
    #define __SYS_PLLCLKIN         (__FRO_OSC_CLK / 2)
  #endif


  #if (CLOCK_SETUP_REG == 1)                    /* Clock Setup via Register */

    /* Syspll clock out calculation */
    #define  __SYS_PLLCLKOUT       (__SYS_PLLCLKIN * ((SYSPLLCTRL_Val & 0x01F) + 1))

    /* main clock calculation */
    #if   ((MAINCLKSEL_Val & 0x03) == 0)
      #define __MAIN_CLOCK         (__FRO_OSC_CLK)
    #elif ((MAINCLKSEL_Val & 0x03) == 1)
      #define __MAIN_CLOCK         (__EXT_CLK)
    #elif ((MAINCLKSEL_Val & 0x03) == 2)
      #define __MAIN_CLOCK         (__WDT_OSC_CLK)
    #elif ((MAINCLKSEL_Val & 0x03) == 3)
      #define __MAIN_CLOCK         (__FRO_OSC_CLK / 2)
    #endif

    /* main pll clock calculation */
    #if   ((MAINCLKPLLSEL_Val & 0x03) == 0)
      #define __MAINPLL_CLOCK      (__MAIN_CLOCK)
    #elif ((MAINCLKPLLSEL_Val & 0x03) == 1)
      #define __MAINPLL_CLOCK      (__SYS_PLLCLKOUT)
    #else
      #error "Oops"
    #endif

    #define __SYSTEM_CLOCK         (__MAINPLL_CLOCK / SYSAHBCLKDIV_Val)
  #endif /* Clock Setup via Register */

  #if (CLOCK_SETUP_API == 1)                    /* Clock Setup via ROM API */
    #define __SYSTEM_CLOCK         (PLL_API_FREQ_Val)
  #endif /* Clock Setup via PLL API */

#else
  #define __SYSTEM_CLOCK           (__FRO_OSC_CLK)
#endif  /* CLOCK_SETUP */



#if ((CLOCK_SETUP == 1) && (CLOCK_SETUP_API == 1)) /* PLL Setup via PLL API */
#include "power_api.h"

typedef struct _ROM {
   const unsigned p_dev0;
   const unsigned p_dev1;
   const unsigned p_dev2;
   const PWRD *   pPWRD;                        /* ROM Power Management API */
   const unsigned p_dev4;
   const unsigned p_dev5;
   const unsigned p_dev6;
   const unsigned p_dev7;
}  ROM;

/*----------------------------------------------------------------------------
  PLL API Function
 *----------------------------------------------------------------------------*/
static void setPLL(const uint32_t pllMode, const uint32_t pllInFreq, const uint32_t reqCpuFreq) {
  uint32_t cmd[5], res[5];
  ROM ** rom = (ROM **) 0x0F001FF8;             /* pointer to power API calls */

  cmd[0] = pllInFreq;                           /* PLL's input   freq in KHz  */
  cmd[1] = reqCpuFreq;                          /* requested CPU freq in KHz  */
  cmd[2] = pllMode;
  cmd[3] = 0;                                   /* no timeout for PLL to lock */

  /* Execute API call */
  __disable_irq();                              /* disable all interrupts     */
  (*rom)->pPWRD->set_pll(cmd, res);             /* call API function          */
  if ((res[0] != PLL_CMD_SUCCESS)){             /* in case of an error ...    */
    while(1);                                   /* ... stay here              */
  }
	__enable_irq();                               /* enable all interrupts       */

}
#endif




/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;      /* System Clock Frequency     */


/*----------------------------------------------------------------------------
  SystemCoreClockUpdate
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)               /* Get Core Clock Frequency   */
{
  uint32_t wdt_osc = 0;
  uint32_t fro_osc = 0;

  /* Determine clock frequency according to clock register values */
  switch ((LPC_SYSCON->FROOSCCTRL     ) & 0x03) {
    case 0:  fro_osc = 18000000; break;
    case 1:  fro_osc = 24000000; break;
    case 2:  fro_osc = 30000000; break;
    case 3:  fro_osc = 30000000; break;
  }
  if (((LPC_SYSCON->FROOSCCTRL >> 17) & 0x01) == 0) {
    fro_osc = fro_osc >> 1;
  }

  switch ((LPC_SYSCON->WDTOSCCTRL >> 5) & 0x0F) {
    case 0:  wdt_osc =       0; break;
    case 1:  wdt_osc =  600000; break;
    case 2:  wdt_osc = 1050000; break;
    case 3:  wdt_osc = 1400000; break;
    case 4:  wdt_osc = 1750000; break;
    case 5:  wdt_osc = 2100000; break;
    case 6:  wdt_osc = 2400000; break;
    case 7:  wdt_osc = 2700000; break;
    case 8:  wdt_osc = 3000000; break;
    case 9:  wdt_osc = 3250000; break;
    case 10: wdt_osc = 3500000; break;
    case 11: wdt_osc = 3750000; break;
    case 12: wdt_osc = 4000000; break;
    case 13: wdt_osc = 4200000; break;
    case 14: wdt_osc = 4400000; break;
    case 15: wdt_osc = 4600000; break;
  }
  wdt_osc /= (((LPC_SYSCON->WDTOSCCTRL & 0x1F) + 1) << 1);

  switch (LPC_SYSCON->MAINCLKPLLSEL & 0x03) {
    case 0:                                           /* main_clk_pre_pll     */
      switch (LPC_SYSCON->MAINCLKSEL & 0x03) {
        case 0:                                       /* Free running oscillator (FRO) */
          SystemCoreClock = fro_osc;
          break;
        case 1:                                       /* System oscillator */
          SystemCoreClock = __SYS_OSC_CLK;
          break;
        case 2:                                       /* watchdog oscillator */
          SystemCoreClock = wdt_osc;
          break;
        case 3:                                       /* Free running oscillator (FRO) / 2 */
          SystemCoreClock = (fro_osc >> 1);
          break;
      }
      break;
    case 1:                                           /* System PLL Clock Out  */
      switch (LPC_SYSCON->SYSPLLCLKSEL & 0x03) {
        case 0:                                       /* Free running oscillator (FRO) */
          SystemCoreClock = fro_osc        * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
          break;
        case 1:                                       /* System oscillator */
          SystemCoreClock = __SYS_OSC_CLK  * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
          break;
        case 2:                                       /* watchdog oscillator */
          SystemCoreClock = wdt_osc        * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
          break;
        case 3:                                       /* Free running oscillator (FRO) / 2 */
          SystemCoreClock = (fro_osc >> 1) * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
          break;
      }
      break;
  }

  SystemCoreClock /= LPC_SYSCON->SYSAHBCLKDIV;
}

/*----------------------------------------------------------------------------
  SystemInit
 *----------------------------------------------------------------------------*/
void SystemInit (void) {
  uint32_t i;

  for (i = 1; i < 1; i++) __NOP();                          /* avoid warning */
  /* System clock to the IOCON & the SWM need to be enabled or
     most of the I/O related peripherals won't work. */
  LPC_SYSCON->SYSAHBCLKCTRL[0] |= ( (1 << 7) |              /* Enables SWM clock */
                                    (1 << 18) );            /* Enables IOCON clock */

#if (CLOCK_SETUP)                                           /* Clock Setup */

#if ((SYSPLLCLKSEL_Val & 0x03) == 0) || \
     ((MAINCLKSEL_Val  & 0x03) == 0)                        /* FRO clock selected */
    LPC_SYSCON->FROOSCCTRL      = FROOSCCTRL_Val;           /* Select Clock Source */
    LPC_SYSCON->FRODIRECTCLKUEN = 0x00;                     /* Toggle Update Register */
    LPC_SYSCON->FRODIRECTCLKUEN = 0x01;
    while (!(LPC_SYSCON->FRODIRECTCLKUEN & 0x01)) __NOP();  /* Wait Until Updated */
#endif

#if ((SYSPLLCLKSEL_Val & 0x03) == 1) || \
     ((MAINCLKSEL_Val  & 0x03) == 1)                        /* external clock selected */
  #if ((EXTCLKSEL_Val & 0x01) == 0)                         /* system oscillator selected */
    #if ((SYSOSCCTRL_Val & 0x01) == 0)                      /* Oscillator is not bypassed */
      LPC_IOCON->PIO0_8   &= ~(3 <<  3);                    /* no pull-down/pull-up */
      LPC_IOCON->PIO0_9   &= ~(3 <<  3);                    /* no pull-down/pull-up */
      LPC_SWM->PINENABLE0 &= ~(3 <<  7);                    /* enable XTALIN/XTALOUT func.*/
    #endif
    LPC_SYSCON->SYSOSCCTRL = SYSOSCCTRL_Val;
    LPC_SYSCON->PDRUNCFG  &= ~(1 << 5);                     /* Power-up sysosc */
    for (i = 0; i < 200; i++) __NOP();                      /* Wait for osc to stabilize  */
  #else                                                     /* CLKIN selected */
    LPC_IOCON->PIO0_1   &= ~(3 <<  3);                      /* no pull-down/pull-up */
    LPC_SWM->PINENABLE0 &= ~(1 << 10);                      /* enable CLKIN func. */
    for (i = 0; i < 200; i++) __NOP();
  #endif
#endif

#if ((SYSPLLCLKSEL_Val & 0x03) == 2)  || \
    ((MAINCLKSEL_Val   & 0x03) == 2)                        /* WDT clock selected */
    LPC_SYSCON->WDTOSCCTRL = WDTOSCCTRL_Val;
    LPC_SYSCON->PDRUNCFG  &= ~(1 << 6);                     /* Power-up WDT Clock */
    for (i = 0; i < 200; i++) __NOP();                      /* Wait for osc to stabilize */
#endif

#if ((SYSPLLCLKSEL_Val & 0x03) == 3) || \
    ((MAINCLKSEL_Val   & 0x03) == 3)                        /* FRO/2 clock selected */
    LPC_SYSCON->FROOSCCTRL      = FROOSCCTRL_Val;           /* Select Clock Source */
    LPC_SYSCON->FRODIRECTCLKUEN = 0x00;                     /* Toggle Update Register */
    LPC_SYSCON->FRODIRECTCLKUEN = 0x01;
    while (!(LPC_SYSCON->FRODIRECTCLKUEN & 0x01)) __NOP();  /* Wait Until Updated */
#endif


#if (CLOCK_SETUP_REG == 1)                                  /* Clock Setup via Register */

  #if ((MAINCLKPLLSEL_Val & 0x01) == 0)                     /* Main Clock is main_clk_pre_dll */
    LPC_SYSCON->MAINCLKSEL    = MAINCLKSEL_Val;             /* Select Clock Source */
    LPC_SYSCON->MAINCLKUEN    = 0x00;                       /* Toggle Update Register */
    LPC_SYSCON->MAINCLKUEN    = 0x01;
    while (!(LPC_SYSCON->MAINCLKUEN & 0x01)) __NOP();       /* Wait Until Updated */
  #else                                                     /* Main clock is sys_pll0_clk */
    LPC_SYSCON->SYSPLLCLKSEL  = SYSPLLCLKSEL_Val;           /* Select PLL Input */
    LPC_SYSCON->SYSPLLCLKUEN  = 0x00;                       /* Toggle Update Register */
    LPC_SYSCON->SYSPLLCLKUEN  = 0x01;
    while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01)) __NOP();     /* Wait Until Updated */

    LPC_SYSCON->SYSPLLCTRL    = SYSPLLCTRL_Val;
    LPC_SYSCON->PDRUNCFG     &= ~(1 << 7);                  /* Power-up SYSPLL */
    while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)) __NOP();       /* Wait Until PLL Locked */
  #endif

  #if ((MAINCLKSEL_Val & 0x03) == 3)                        /* Main Clock is PLL Out */
    LPC_SYSCON->SYSPLLCTRL    = SYSPLLCTRL_Val;
    LPC_SYSCON->PDRUNCFG     &= ~(1 << 7);                  /* Power-up SYSPLL */
    while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)) __NOP();       /* Wait Until PLL Locked */
  #endif

  LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;

  LPC_SYSCON->MAINCLKPLLSEL    = MAINCLKPLLSEL_Val;         /* Select Clock Source */
  LPC_SYSCON->MAINCLKPLLUEN    = 0x00;                      /* Toggle Update Register */
  LPC_SYSCON->MAINCLKPLLUEN    = 0x01;
  while (!(LPC_SYSCON->MAINCLKPLLUEN & 0x01)) __NOP();      /* Wait Until Updated */
#endif  /* CLOCK_SETUP_REG == 1 */

#if (CLOCK_SETUP_API == 1)                                  /* Clock Setup via PLL API */

  LPC_SYSCON->SYSPLLCLKSEL  = SYSPLLCLKSEL_Val;             /* Select PLL Input */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x00;                         /* Toggle Update Register */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01)) __NOP();       /* Wait Until Updated */

  LPC_SYSCON->SYSAHBCLKDIV  = 1;

  setPLL(PLL_API_MODE_Val, __SYS_PLLCLKIN / 1000, PLL_API_FREQ_Val / 1000);
#endif /* CLOCK_SETUP_API == 1 */

#endif /* CLOCK_SETUP */
}
