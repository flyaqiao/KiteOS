/****************************************************************************** 
 * @file:    Target.h 
 * @purpose: CMSIS Cortex-M3 Device Peripheral Access Layer Header File 
 *           for the NXP LPC17xx Device Series  
 * @version: V1.0 
 * @date:    25. Nov. 2008 
 *---------------------------------------------------------------------------- 
 * 
 * Copyright (C) 2008 ARM Limited. All rights reserved. 
 * 
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3  
 * processor based microcontrollers.  This file can be freely distributed  
 * within development tools that are supporting such ARM based processors.  
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED 
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. 
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. 
 * 
 ******************************************************************************/ 
 
#ifndef __TARGET_H 
#define __TARGET_H 
#include <LPC17xx.H>
#include "Sys.h"
   
/*  
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------  
*/   
   
/*--------------------- Clock Configuration ----------------------------------  
//  
// <E> Clock Configuration  
//   <H> System Controls and Status Register (SCS)  
//     <O1.4>    OSCRANGE: Main Oscillator Range Select  
//                     <0=>  1 MHz to 20 MHz  
//                     <1=> 15 MHz to 24 MHz  
//     <E1.5>       OSCEN: Main Oscillator Enable  
//     </E>  
//   </H>  
//  
//   <H> Clock Source Select Register (CLKSRCSEL)  
//     <O2.0..1>   CLKSRC: PLL Clock Source Selection  
//                     <0=> Internal RC oscillator  
//                     <1=> Main oscillator  
//                     <2=> RTC oscillator  
//   </H>  
//  
//   <E3> PLL0 Configuration (Main PLL)  
//     <H> PLL0 Configuration Register (PLL0CFG)  
//                     <I> F_cco0 = (2 * M * F_in) / N  
//                     <I> F_in must be in the range of 32 kHz to 50 MHz  
//                     <I> F_cco0 must be in the range of 275 MHz to 550 MHz  
//       <O4.0..14>  MSEL: PLL Multiplier Selection  
//                     <6-32768><#-1>  
//                     <I> M Value  
//       <O4.16..23> NSEL: PLL Divider Selection  
//                     <1-256><#-1>  
//                     <I> N Value  
//     </H>  
//   </E>  
//  
//   <E5> PLL1 Configuration (USB PLL)  
//     <H> PLL1 Configuration Register (PLL1CFG)  
//                     <I> F_usb = M * F_osc or F_usb = F_cco1 / (2 * P)  
//                     <I> F_cco1 = F_osc * M * 2 * P  
//                     <I> F_cco1 must be in the range of 156 MHz to 320 MHz  
//       <O6.0..4>   MSEL: PLL Multiplier Selection  
//                     <1-32><#-1>  
//                     <I> M Value (for USB maximum value is 4)  
//       <O6.5..6>   PSEL: PLL Divider Selection  
//                     <0=> 1  
//                     <1=> 2  
//                     <2=> 4  
//                     <3=> 8  
//                     <I> P Value  
//     </H>  
//   </E>  
//  
//   <H> CPU Clock Configuration Register (CCLKCFG)  
//     <O7.0..7>  CCLKSEL: Divide Value for CPU Clock from PLL0  
//                     <2-256:2><#-1>  
//   </H>  
//  
//   <H> USB Clock Configuration Register (USBCLKCFG)  
//     <O8.0..3>   USBSEL: Divide Value for USB Clock from PLL1  
//                     <0-15>  
//                     <I> Divide is USBSEL + 1  
//   </H>  
//  
//   <H> Peripheral Clock Selection Register 0 (PCLKSEL0)  
//     <O9.0..1>    PCLK_WDT: Peripheral Clock Selection for WDT  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.2..3>    PCLK_TIMER0: Peripheral Clock Selection for TIMER0  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.4..5>    PCLK_TIMER1: Peripheral Clock Selection for TIMER1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.6..7>    PCLK_UART0: Peripheral Clock Selection for UART0  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.8..9>    PCLK_UART1: Peripheral Clock Selection for UART1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.12..13>  PCLK_PWM1: Peripheral Clock Selection for PWM1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.14..15>  PCLK_I2C0: Peripheral Clock Selection for I2C0  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.16..17>  PCLK_SPI: Peripheral Clock Selection for SPI  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.20..21>  PCLK_SSP1: Peripheral Clock Selection for SSP1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.22..23>  PCLK_DAC: Peripheral Clock Selection for DAC  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.24..25>  PCLK_ADC: Peripheral Clock Selection for ADC  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O9.26..27>  PCLK_CAN1: Peripheral Clock Selection for CAN1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 6  
//     <O9.28..29>  PCLK_CAN2: Peripheral Clock Selection for CAN2  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 6  
//     <O9.30..31>  PCLK_ACF: Peripheral Clock Selection for ACF  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 6  
//   </H>  
//  
//   <H> Peripheral Clock Selection Register 1 (PCLKSEL1)  
//     <O10.0..1>   PCLK_QEI: Peripheral Clock Selection for the Quadrature Encoder Interface  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.2..3>   PCLK_GPIO: Peripheral Clock Selection for GPIOs  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.4..5>   PCLK_PCB: Peripheral Clock Selection for the Pin Connect Block  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.6..7>   PCLK_I2C1: Peripheral Clock Selection for I2C1  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.10..11> PCLK_SSP0: Peripheral Clock Selection for SSP0  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.12..13> PCLK_TIMER2: Peripheral Clock Selection for TIMER2  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.14..15> PCLK_TIMER3: Peripheral Clock Selection for TIMER3  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.16..17> PCLK_UART2: Peripheral Clock Selection for UART2  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.18..19> PCLK_UART3: Peripheral Clock Selection for UART3  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.20..21> PCLK_I2C2: Peripheral Clock Selection for I2C2  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.22..23> PCLK_I2S: Peripheral Clock Selection for I2S  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.26..27> PCLK_RIT: Peripheral Clock Selection for the Repetitive Interrupt Timer  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.28..29> PCLK_SYSCON: Peripheral Clock Selection for the System Control Block  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//     <O10.30..31> PCLK_MC: Peripheral Clock Selection for the Motor Control PWM  
//                     <0=> Pclk = Cclk / 4  
//                     <1=> Pclk = Cclk  
//                     <2=> Pclk = Cclk / 2  
//                     <3=> Pclk = Hclk / 8  
//   </H>  
//  
//   <H> Power Control for Peripherals Register (PCONP)  
//     <O11.1>      PCTIM0: Timer/Counter 0 power/clock enable  
//     <O11.2>      PCTIM1: Timer/Counter 1 power/clock enable  
//     <O11.3>      PCUART0: UART 0 power/clock enable  
//     <O11.4>      PCUART1: UART 1 power/clock enable  
//     <O11.6>      PCPWM1: PWM 1 power/clock enable  
//     <O11.7>      PCI2C0: I2C interface 0 power/clock enable  
//     <O11.8>      PCSPI: SPI interface power/clock enable  
//     <O11.9>      PCRTC: RTC power/clock enable  
//     <O11.10>     PCSSP1: SSP interface 1 power/clock enable  
//     <O11.12>     PCAD: A/D converter power/clock enable  
//     <O11.13>     PCCAN1: CAN controller 1 power/clock enable  
//     <O11.14>     PCCAN2: CAN controller 2 power/clock enable  
//     <O11.15>     PCGPIO: GPIOs power/clock enable  
//     <O11.16>     PCRIT: Repetitive interrupt timer power/clock enable  
//     <O11.17>     PCMC: Motor control PWM power/clock enable  
//     <O11.18>     PCQEI: Quadrature encoder interface power/clock enable  
//     <O11.19>     PCI2C1: I2C interface 1 power/clock enable  
//     <O11.21>     PCSSP0: SSP interface 0 power/clock enable  
//     <O11.22>     PCTIM2: Timer 2 power/clock enable  
//     <O11.23>     PCTIM3: Timer 3 power/clock enable  
//     <O11.24>     PCUART2: UART 2 power/clock enable  
//     <O11.25>     PCUART3: UART 3 power/clock enable  
//     <O11.26>     PCI2C2: I2C interface 2 power/clock enable  
//     <O11.27>     PCI2S: I2S interface power/clock enable  
//     <O11.29>     PCGPDMA: GP DMA function power/clock enable  
//     <O11.30>     PCENET: Ethernet block power/clock enable  
//     <O11.31>     PCUSB: USB interface power/clock enable  
//   </H>  
//  
//   <H> Clock Output Configuration Register (CLKOUTCFG)  
//     <O12.0..3>   CLKOUTSEL: Selects clock source for CLKOUT  
//                     <0=> CPU clock  
//                     <1=> Main oscillator  
//                     <2=> Internal RC oscillator  
//                     <3=> USB clock  
//                     <4=> RTC oscillator  
//     <O12.4..7>   CLKOUTDIV: Selects clock divider for CLKOUT  
//                     <1-16><#-1>  
//     <O12.8>      CLKOUT_EN: CLKOUT enable control  
//   </H>  
//  
// </E>  
*/   
#define CLOCK_SETUP           1    
#define SCS_Val               0x00000020    
#define CLKSRCSEL_Val         0x00000001    
#define PLL0_SETUP            1    
#define PLL0CFG_Val           0x0000000B    
#define PLL1_SETUP            0    
#define PLL1CFG_Val           0x00000000    
#define CCLKCFG_Val           0x00000002    
#define USBCLKCFG_Val         0x00000000    
#define PCLKSEL0_Val          0x00000000    
#define PCLKSEL1_Val          0x00000000    
#define PCONP_Val             0x00C6825E    
#define CLKOUTCFG_Val         0x00000000    
   
/*--------------------- Flash Accelerator Configuration ----------------------  
//  
// <E> Flash Accelerator Configuration  
//   <O1.0..1>   FETCHCFG: Fetch Configuration  
//               <0=> Instruction fetches from flash are not buffered  
//               <1=> One buffer is used for all instruction fetch buffering  
//               <2=> All buffers may be used for instruction fetch buffering  
//               <3=> Reserved (do not use this setting)  
//   <O1.2..3>   DATACFG: Data Configuration  
//               <0=> Data accesses from flash are not buffered  
//               <1=> One buffer is used for all data access buffering  
//               <2=> All buffers may be used for data access buffering  
//               <3=> Reserved (do not use this setting)  
//   <O1.4>      ACCEL: Acceleration Enable  
//   <O1.5>      PREFEN: Prefetch Enable  
//   <O1.6>      PREFOVR: Prefetch Override  
//   <O1.12..15> FLASHTIM: Flash Access Time  
//               <0=> 1 CPU clock (for CPU clock up to 20 MHz)  
//               <1=> 2 CPU clocks (for CPU clock up to 40 MHz)  
//               <2=> 3 CPU clocks (for CPU clock up to 60 MHz)  
//               <3=> 4 CPU clocks (for CPU clock up to 80 MHz)  
//               <4=> 5 CPU clocks (for CPU clock up to 100 MHz)  
//               <5=> 6 CPU clocks (for any CPU clock)  
// </E>  
*/   
#define FLASH_SETUP           1    
#define FLASHCFG_Val          0x0000503A    
/*  
//-------- <<< end of configuration section >>> ------------------------------  
*/   
   
/*----------------------------------------------------------------------------  
  Check the register settings  
 *----------------------------------------------------------------------------*/   
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))    
#define CHECK_RSVD(val, mask)                     (val & mask)    
   
/* Clock Configuration -------------------------------------------------------*/   
#if (CHECK_RSVD((SCS_Val),       ~0x00000030))    
   #error "SCS: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RANGE((CLKSRCSEL_Val), 0, 2))    
   #error "CLKSRCSEL: Value out of range!"    
#endif    
   
#if (CHECK_RSVD((PLL0CFG_Val),   ~0x00FF7FFF))    
   #error "PLL0CFG: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RSVD((PLL1CFG_Val),   ~0x0000007F))    
   #error "PLL1CFG: Invalid values of reserved bits!"    
#endif    
   
//#if ((CCLKCFG_Val != 0) && (((CCLKCFG_Val - 1) % 2)))    
//   #error "CCLKCFG: CCLKSEL field does not contain only odd values or 0!"    
//#endif    
   
#if (CHECK_RSVD((USBCLKCFG_Val), ~0x0000000F))    
   #error "USBCLKCFG: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RSVD((PCLKSEL0_Val),   0x000C0C00))    
   #error "PCLKSEL0: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RSVD((PCLKSEL1_Val),   0x03000300))    
   #error "PCLKSEL1: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RSVD((PCONP_Val),      0x10100821))    
   #error "PCONP: Invalid values of reserved bits!"    
#endif    
   
#if (CHECK_RSVD((CLKOUTCFG_Val), ~0x000001FF))    
   #error "CLKOUTCFG: Invalid values of reserved bits!"    
#endif    
   
/* Flash Accelerator Configuration -------------------------------------------*/   
#if (CHECK_RSVD((FLASHCFG_Val), ~0x0000F07F))    
   #error "FLASHCFG: Invalid values of reserved bits!"    
#endif     
extern uint32_t SystemFrequency;
#define FAHBCLK       (SystemFrequency / 4)

#endif 


