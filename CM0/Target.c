/* 
* Copyright (c) 2011 - 2011, ���ؿƼ�
* All rights reserved. 
* 
*  
* �ļ����ƣ�$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Target.c $ 
* ժ    Ҫ��
*  
* ��ǰ�汾��$Revision: 626 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$LastChangedDate:: 2011-10-20 22:49:48#$
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

  LPC_SYSCON->SYSOSCCTRL  = 0x00;                                                 /*  ����Ƶ�ʹ�����Χѡ��      */

  LPC_SYSCON->PDRUNCFG &= ~(0x1ul << 5);                                          /*  ϵͳ�����ϵ�              */
  for (i = 0; i < 0x100; i++ ) {                                                  /*  �ȴ������ȶ�              */
  }
  LPC_SYSCON->SYSPLLCLKSEL = MAIN_CLKSRCSEL_VALUE;                                /*  ѡ��ϵͳ����OSC           */
  LPC_SYSCON->SYSPLLCLKUEN = 0x00;                                                /*  �л�ʱ��Դ                  */
  LPC_SYSCON->SYSPLLCLKUEN = 0x01;                                                /*  ����ʱ��Դ                  */
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01)) {                                    /*  �ȴ��������                */	
  }
  uiRegVal   = LPC_SYSCON->SYSPLLCTRL;
  uiRegVal  &= ~0x1FF;
  LPC_SYSCON->SYSPLLCTRL = (uiRegVal | (PLL_PVALUE << 5) | PLL_MVALUE);           /*  Ԥ��Ƶ��M+1 �� 2*P          */
  LPC_SYSCON->PDRUNCFG  &= ~(0x01ul << 7);                                        /*  ϵͳMAIN PLL�ϵ�            */
  while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)){                                       /*  �ȴ�����                    */
  }
  LPC_SYSCON->MAINCLKSEL = 0x03;                                                  /*  ѡ��PLL���                 */
  LPC_SYSCON->MAINCLKUEN = 0x01;                                                  /*  ����MCLKʱ��Դѡ��          */
  LPC_SYSCON->MAINCLKUEN = 0x00;                                                  /*  ��ת���¼Ĵ���              */
  LPC_SYSCON->MAINCLKUEN = 0x01;
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01)) {                                      /*  �ȴ��������                */
  }
  LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;                                    /*  SYStem AHBʱ�ӷ�Ƶ          */
  
  LPC_SYSCON->SYSAHBCLKCTRL = AHBCLKCTRL_Val;                                     /*  ʹ��AHB����ʱ��             */
#if NVIC_SETUP
  InitVIC();
#endif
  return;
}

__asm void SysCtlDelay(DWORD ulCount)
{
  SUBS    R0, #1;                                                         /* R0��1��R0ʵ���Ͼ��ǲ���ULCOUNT*/
  BNE     SysCtlDelay;                                                    /* ��������Ϊ0����ת��SysCtlDelay*/
  BX      LR;                                                             /* �ӳ��򷵻�                                */
}

void DelayUs(DWORD us)
{
  SysCtlDelay(us * (FAHBCLK / 3000000));
}

void DelayMs(DWORD ms)
{
  SysCtlDelay(ms * (FAHBCLK / 3000));
}
