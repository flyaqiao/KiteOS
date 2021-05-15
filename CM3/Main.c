/* 
* Copyright (c) 2008 - 2009, 凯特科技 
* All rights reserved. 
*  
* 文件名称：$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/DriveBoard/Main.c $ 
* 摘    要：
*  
* 当前版本：$Revision: 1993 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$LastChangedDate:: 2011-10-18 08:30:57#$
*
*/  
#include <LPC17xx.H>   
#include "stdio.h"        
#include "NVIC.h"
#include "string.h"
#include "KiteLib.h" 
#include "Uart.h"
#include "Config.h"
DWORD  AppStatusStack[256];

#define LED_LAMP  (1 << 0)

void AppStatus(void)
{
	LPC_GPIO2->FIODIR	|= (LED_LAMP);
  while (1)
  {   
    OsWaitDelay(OS_TICKS_PER_SEC);
  	LPC_GPIO2->FIOPIN ^= LED_LAMP;
 }
}
void AppEntry(void)  
{   
//  GpioConfig();  
  UartInit();   
//  printf("System running.\r\n");
  OsCreateTask(AppStatus, AppStatusStack + 256, 4);  
       
  while (1)
  {            
    OsWaitDelay(OS_TICKS_PER_SEC);
  }
}
