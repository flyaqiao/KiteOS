# KiteOS
lite os, micro os for arm CM0~CM3

examples
```
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
