/******************************************************************************  
 * @file:    Target.c  
 * @purpose: CMSIS Cortex-M3 Device Peripheral Access Layer Source File  
 *           for the NXP LPC17xx Device Series   
 * @version: V1.1  
 * @date:    18th May 2009  
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
#include <STDINT.H>    
#include <LPC17xx.h>
#include "Target.h"
#include "NVIC.h" 
   
/*----------------------------------------------------------------------------  
  DEFINES  
 *----------------------------------------------------------------------------*/   
       
/*----------------------------------------------------------------------------  
  Define clocks  
 *----------------------------------------------------------------------------*/   
#define XTAL        (12000000UL)        /* Oscillator frequency               */    
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */    
#define RTC_CLK     (   32000UL)        /* RTC oscillator frequency           */    
#define IRC_OSC     ( 4000000UL)        /* Internal RC oscillator frequency   */    
   
   
/*----------------------------------------------------------------------------  
  Clock Variable definitions  
 *----------------------------------------------------------------------------*/   
uint32_t SystemFrequency; /*!< System Clock Frequency (Core Clock)  */   
   
/**  
 * Initialize the system  
 *  
 * @param  none  
 * @return none  
 *  
 * @brief  Setup the microcontroller system.  
 *         Initialize the System and update the SystemFrequency variable.  
 */   
void SystemInit (void)   
{   
#if (CLOCK_SETUP)                       /* Clock Setup                        */    
  LPC_SC->SCS       = SCS_Val;   
  if (SCS_Val & (1 << 5)) {             /* If Main Oscillator is enabled      */   
    while ((LPC_SC->SCS & (1 << 6)) == 0);  /* Wait for Oscillator to be ready    */   
  }   
   
  LPC_SC->CCLKCFG   = CCLKCFG_Val;          /* Setup Clock Divider                */   
   
#if (PLL0_SETUP)    
  LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;        /* Select Clock Source for PLL0       */   
  LPC_SC->PLL0CFG   = PLL0CFG_Val;   
  LPC_SC->PLL0CON   = 0x01;                 /* PLL0 Enable                        */   
  LPC_SC->PLL0FEED  = 0xAA;   
  LPC_SC->PLL0FEED  = 0x55;   
  while (!(LPC_SC->PLL0STAT & (1 << 26)));  /* Wait for PLOCK0                    */   
   
  LPC_SC->PLL0CON   = 0x03;                 /* PLL0 Enable & Connect              */   
  LPC_SC->PLL0FEED  = 0xAA;   
  LPC_SC->PLL0FEED  = 0x55;   
#endif    
   
#if (PLL1_SETUP)    
  LPC_SC->PLL1CFG   = PLL1CFG_Val;   
  LPC_SC->PLL1CON   = 0x01;                 /* PLL1 Enable                        */   
  LPC_SC->PLL1FEED  = 0xAA;   
  LPC_SC->PLL1FEED  = 0x55;   
  while (!(LPC_SC->PLL1STAT & (1 << 10)));  /* Wait for PLOCK1                    */   
   
  LPC_SC->PLL1CON   = 0x03;                 /* PLL1 Enable & Connect              */   
  LPC_SC->PLL1FEED  = 0xAA;   
  LPC_SC->PLL1FEED  = 0x55;   
#else    
  LPC_SC->USBCLKCFG = USBCLKCFG_Val;        /* Setup USB Clock Divider            */   
#endif    
   
  LPC_SC->PCLKSEL0  = PCLKSEL0_Val;         /* Peripheral Clock Selection         */   
  LPC_SC->PCLKSEL1  = PCLKSEL1_Val;   
   
  LPC_SC->PCONP     = PCONP_Val;            /* Power Control for Peripherals      */   
   
  LPC_SC->CLKOUTCFG = CLKOUTCFG_Val;        /* Clock Output Configuration         */   
#endif    
   
  /* Determine clock frequency according to clock register values             */   
  if (((LPC_SC->PLL0STAT >> 24) & 3) == 3) {/* If PLL0 enabled and connected      */   
    switch (LPC_SC->CLKSRCSEL & 0x03) {   
      case 0:                           /* Internal RC oscillator => PLL0     */   
      case 3:                           /* Reserved, default to Internal RC   */   
        SystemFrequency = (IRC_OSC *    
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /   
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /   
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));   
        break;   
      case 1:                           /* Main oscillator => PLL0            */   
        SystemFrequency = (OSC_CLK *    
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /   
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /   
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));   
        break;   
      case 2:                           /* RTC oscillator => PLL0             */   
        SystemFrequency = (RTC_CLK *    
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /   
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /   
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));   
        break;   
    }   
  } else {   
    switch (LPC_SC->CLKSRCSEL & 0x03) {   
      case 0:                           /* Internal RC oscillator => PLL0     */   
      case 3:                           /* Reserved, default to Internal RC   */   
        SystemFrequency = IRC_OSC / ((LPC_SC->CCLKCFG & 0xFF)+ 1);   
        break;   
      case 1:                           /* Main oscillator => PLL0            */   
        SystemFrequency = OSC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);   
        break;   
      case 2:                           /* RTC oscillator => PLL0             */   
        SystemFrequency = RTC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);   
        break;   
    }   
  }   
   
#if (FLASH_SETUP == 1)                  /* Flash Accelerator Setup            */    
  LPC_SC->FLASHCFG  = FLASHCFG_Val;   
#endif    
  InitVIC();       
  LPC_SC->SCS |= 0x01; 
}

