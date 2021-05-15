#ifndef __TARGET_H__
#define __TARGET_H__
#include <LPC11xx.h>
#include "Sys.h"
/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Clock Configuration ----------------------------------
//
// <e> Clock Configuration
//   <o1.0>   SYS Clock Enable
//   <o1.1>   ROM Clock Enable
//   <o1.2>   RAM Clock Enable
//   <o1.3>   FLASH1 Clock Enable
//   <o1.4>   FLASH2 Clock Enable
//   <o1.5>   I2C Clock Enable
//   <o1.6>   GPIO Clock Enable
//   <o1.7>   CT16B0 Clock Enable
//   <o1.8>   CT16B1 Clock Enable
//   <o1.9>   CT32B0 Clock Enable
//   <o1.10>  CT32B1 Clock Enable
//   <o1.11>  SSP Clock Enable
//   <o1.12>  UART Clock Enable
//   <o1.13>  ADC Clock Enable
//   <o1.14>  USB_REG Clock Enable
//   <o1.15>  SWDT Clock Enable
//   <o1.16>  IOCON Clock Enable
// </e>
*/
#define CLOCK_SETUP           1
#define AHBCLKCTRL_Val        0x0001005F

/*--------------------- NVIC Mapping Configuration -------------------------
//
// <e> NVIC Mapping
// </e>
*/
#define NVIC_SETUP          1

/*
//-------- <<< end of configuration section >>> ------------------------------
*/
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define SYSAHBCLKDIV_Val            1
#define MAIN_CLKSRCSEL_VALUE        1                                   /*  0x00:选择IRC振荡器          */
                                                                        /*  0x01:选择系统振器           */
                                                                        /*  0x02:选择WDT振荡器          */

#define FOSC                        12000000                            /*  振荡器频率                  */

#define FCCLK                       48000000                            /*  主时钟频率<=50Mhz           */
                                                                        /*  FOSC的整数倍                */
#define FCCO                       (FCCLK * 2 * (1 << PLL_PVALUE))      /*  PLL频率(156Mhz~320Mhz)      */
#define FAHBCLK                    (FCCLK / SYSAHBCLKDIV_Val)           /*  AHB CLK分频或与FCCLK相同    */
#define PLL_PVALUE                  1                                   /*  1~256                       */
#define PLL_MVALUE                 (FCCLK / FOSC - 1 )                  /*  1~256                       */

#define SystemFrequency             FAHBCLK
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit(void);
#if NVIC_SETUP
void InstallIRQ(int IntNumber, void* HandlerAddr);
#endif
void DelayUs(DWORD ns);
void DelayMs(DWORD ms);

#endif // __TARGET_H__
