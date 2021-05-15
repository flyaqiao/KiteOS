/* 
* Copyright (c) 2011 - 2011, 凯特科技
* All rights reserved. 
* 
*  
* 文件名称：$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/main.c $ 
* 摘    要：
*  
* 当前版本：$Revision: 663 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$LastChangedDate:: 2011-11-02 13:04:19#$
*
*/
#include <LPC11xx.h>
#include "stdio.h"    
#include "stdlib.h" 
#include "absacc.h"
#include "Target.h"
#include "main.h"    
#include "Uart.h" 
#include "Config.h"
#include "MD5.h"
DWORD AppStatusStack[128];
DWORD buf[4];
                            
void AppStatus(void)
{
	LPC_GPIO2->DIR	|= (LED_LAMP);
  LPC_GPIO0->DIR	|= (1 << 0);
  while (1)
  {                     
    MD5Calc("1234567890", 10, buf); 
    OsWaitDelay(OS_TICKS_PER_SEC);
  	LPC_GPIO2->MASKED_ACCESS[LED_LAMP] ^= HIGH; 
  	LPC_GPIO0->MASKED_ACCESS[1 << 0] ^= HIGH;
 }
}
/*********************************************************************************************************
* Function Name:        timer0Isr
* Description:          TIMER0 中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void Timer0Isr(void)
{
  LPC_TMR32B0->IR = 0x01;                                                   /*  清除中断标志                */
  LPC_GPIO0->MASKED_ACCESS[1 << 5] ^= HIGH;
}
/*********************************************************************************************************
** Function name:       timer0Init
** Descriptions:        32位定时器0初始化函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Timer0Init(void)
{
  LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 9);                                        /*  打开定时器模块              */

  LPC_TMR32B0->IR      = 1;
  LPC_TMR32B0->PR      = 1000;                                              /*  设置分频系数                */
  LPC_TMR32B0->MCR     = 3;                                                 /*  设置MR0匹配后复位TC并产生中断*/
  LPC_TMR32B0->MR0     = FAHBCLK / 1000;                                     /*  设置中断时间                */
//  zyIsrSet(NVIC_TIMER32B0, (unsigned long)timer0Isr, PRIO_TWO);       /*  设置中断并使能              */
  LPC_TMR32B0->TCR     = 0x01;                                              /*  启动定时器                  */
  InstallIRQ(TIMER_32_0_IRQn, Timer0Isr);
  NVIC_EnableIRQ(TIMER_32_0_IRQn);
}
          
void AppEntry(void)  
{  
  LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 6);                                        /*  使能GPIO模块时钟            */
  UartInit(0);   
  printf("System running.\r\n");
  OsCreateTask(AppStatus, AppStatusStack + 128, 4);  
  LPC_GPIO0->DIR |= (1 << 5);       
  LPC_GPIO1->DIR |= (1 << 1);    
  Timer0Init();
   
  while (1)
  {           
    OsWaitDelay(OS_TICKS_PER_SEC);
    LPC_GPIO1->MASKED_ACCESS[1 << 1] ^= HIGH;
  }
}
//void WDTInit(void)
//{
//  LPC_SYSCON->WDTCLKSEL      = 0x00;                                              /*  选择WDT时钟源               */
//  LPC_SYSCON->WDTCLKUEN      = 0x00;
//  LPC_SYSCON->WDTCLKUEN      = 0x01;                                              /*  更新使能                    */
//  LPC_SYSCON->WDTCLKDIV      = 0x0A;                                              /*  分频为1                     */
//  LPC_SYSCON->SYSAHBCLKCTRL |= (0x01 << 15);                                      /*  打开WDT模块                 */
//  LPC_WDT->TC          			 = 0xFFFFFFFF;                                        /*  定时时间                    */
//  LPC_WDT->MOD          		 = (0x01 << 0)                                        /*  使能WDT                     */
//                 						 | (0x01 << 1);                                       /*  使能WDT中断                 */
//  LPC_WDT->FEED        			 = 0xAA;                                              /*  喂狗                        */
//  LPC_WDT->FEED        			 = 0x55;
//}
//void Feed(void)
//{
//	OsEnterCritical();
//  LPC_WDT->FEED        			= 0xAA;                                              	/*  喂狗                        */
//  LPC_WDT->FEED        			= 0x55;
//	OsExitCritical();
//}
