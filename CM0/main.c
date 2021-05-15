/* 
* Copyright (c) 2011 - 2011, ���ؿƼ�
* All rights reserved. 
* 
*  
* �ļ����ƣ�$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/main.c $ 
* ժ    Ҫ��
*  
* ��ǰ�汾��$Revision: 663 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$LastChangedDate:: 2011-11-02 13:04:19#$
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
* Description:          TIMER0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void Timer0Isr(void)
{
  LPC_TMR32B0->IR = 0x01;                                                   /*  ����жϱ�־                */
  LPC_GPIO0->MASKED_ACCESS[1 << 5] ^= HIGH;
}
/*********************************************************************************************************
** Function name:       timer0Init
** Descriptions:        32λ��ʱ��0��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Timer0Init(void)
{
  LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 9);                                        /*  �򿪶�ʱ��ģ��              */

  LPC_TMR32B0->IR      = 1;
  LPC_TMR32B0->PR      = 1000;                                              /*  ���÷�Ƶϵ��                */
  LPC_TMR32B0->MCR     = 3;                                                 /*  ����MR0ƥ���λTC�������ж�*/
  LPC_TMR32B0->MR0     = FAHBCLK / 1000;                                     /*  �����ж�ʱ��                */
//  zyIsrSet(NVIC_TIMER32B0, (unsigned long)timer0Isr, PRIO_TWO);       /*  �����жϲ�ʹ��              */
  LPC_TMR32B0->TCR     = 0x01;                                              /*  ������ʱ��                  */
  InstallIRQ(TIMER_32_0_IRQn, Timer0Isr);
  NVIC_EnableIRQ(TIMER_32_0_IRQn);
}
          
void AppEntry(void)  
{  
  LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 6);                                        /*  ʹ��GPIOģ��ʱ��            */
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
//  LPC_SYSCON->WDTCLKSEL      = 0x00;                                              /*  ѡ��WDTʱ��Դ               */
//  LPC_SYSCON->WDTCLKUEN      = 0x00;
//  LPC_SYSCON->WDTCLKUEN      = 0x01;                                              /*  ����ʹ��                    */
//  LPC_SYSCON->WDTCLKDIV      = 0x0A;                                              /*  ��ƵΪ1                     */
//  LPC_SYSCON->SYSAHBCLKCTRL |= (0x01 << 15);                                      /*  ��WDTģ��                 */
//  LPC_WDT->TC          			 = 0xFFFFFFFF;                                        /*  ��ʱʱ��                    */
//  LPC_WDT->MOD          		 = (0x01 << 0)                                        /*  ʹ��WDT                     */
//                 						 | (0x01 << 1);                                       /*  ʹ��WDT�ж�                 */
//  LPC_WDT->FEED        			 = 0xAA;                                              /*  ι��                        */
//  LPC_WDT->FEED        			 = 0x55;
//}
//void Feed(void)
//{
//	OsEnterCritical();
//  LPC_WDT->FEED        			= 0xAA;                                              	/*  ι��                        */
//  LPC_WDT->FEED        			= 0x55;
//	OsExitCritical();
//}
