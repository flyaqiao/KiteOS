/* 
* Copyright (c) 2011 - 2011, 凯特科技
* All rights reserved. 
* 
*  
* 文件名称：$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Target.c $ 
* 摘    要：
*  
* 当前版本：$Revision: 626 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$LastChangedDate:: 2011-10-20 22:49:48#$
*
*/
#include "LPC11xx.h"
#include "Target.h"  
#include "absacc.h"

#if NVIC_SETUP    
__align(256) DWORD VectorsTable[128];
//extern DWORD __Vectors[];
//#define VTOR                    (*(volatile unsigned long *)0xE000ED08)
 
void InitVIC(void)    
{   
//   BYTE i = 0;
//   for (i = 0; i < 128; i++)
//     VectorsTable[i] = __Vectors[i];
//   OsEnterCritical();
//   LPC_SYSCON->SYSMEMREMAP = 0x01;
//   VTOR = (DWORD)VectorsTable; 
//   OsExitCritical();
	return;   
}   
/******************************************************************************  
** Function name:       install_irq  
**  
** Descriptions:        Install interrupt handler  
** parameters:          Interrupt number, interrupt handler address,   
**                      interrupt priority  
** Returned value:      true or false, return false if IntNum is out of range  
**   
******************************************************************************/   
void InstallIRQ(int IntNumber, void* HandlerAddr)   
{   
	VectorsTable[IntNumber + 16] = (DWORD)HandlerAddr;
}  
#endif
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit(void)
{
  int i, uiRegVal;

  LPC_SYSCON->SYSOSCCTRL  = 0x00;                                                 /*  振荡器频率工作范围选择      */

  LPC_SYSCON->PDRUNCFG &= ~(0x1ul << 5);                                          /*  系统振荡器上电              */
  for (i = 0; i < 0x100; i++ ) {                                                  /*  等待振荡器稳定              */
  }
  LPC_SYSCON->SYSPLLCLKSEL = MAIN_CLKSRCSEL_VALUE;                                /*  选择系统振荡器OSC           */
  LPC_SYSCON->SYSPLLCLKUEN = 0x00;                                                /*  切换时钟源                  */
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;                                                /*  更新时钟源                  */
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01)) {                                    /*  等待更新完成                */	
  }
  uiRegVal   = LPC_SYSCON->SYSPLLCTRL;
  uiRegVal  &= ~0x1FF;
  LPC_SYSCON->SYSPLLCTRL = (uiRegVal | (PLL_PVALUE << 5) | PLL_MVALUE);           /*  预分频：M+1 与 2*P          */
  LPC_SYSCON->PDRUNCFG  &= ~(0x01ul << 7);                                        /*  系统MAIN PLL上电            */
  while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)){                                       /*  等待锁定                    */
  }
  LPC_SYSCON->MAINCLKSEL = 0x03;                                                  /*  选择PLL输出                 */
  LPC_SYSCON->MAINCLKUEN = 0x01;                                                  /*  更新MCLK时钟源选择          */
  LPC_SYSCON->MAINCLKUEN = 0x00;                                                  /*  翻转更新寄存器              */
  LPC_SYSCON->MAINCLKUEN = 0x01;
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01)) {                                      /*  等待更新完成                */
  }
  LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;                                    /*  SYStem AHB时钟分频          */
  
  LPC_SYSCON->SYSAHBCLKCTRL = AHBCLKCTRL_Val;                                     /*  使能AHB总线时钟             */
#if NVIC_SETUP
  InitVIC();
#endif
  return;
}

__asm void SysCtlDelay(DWORD ulCount)
{
  SUBS    R0, #1;                                                         /* R0减1，R0实际上就是参数ULCOUNT*/
  BNE     SysCtlDelay;                                                    /* 如果结果不为0则跳转到SysCtlDelay*/
  BX      LR;                                                             /* 子程序返回                                */
}

void DelayUs(DWORD us)
{
  SysCtlDelay(us * (FAHBCLK / 3000000));
}

void DelayMs(DWORD ms)
{
  SysCtlDelay(ms * (FAHBCLK / 3000));
}
