/* 
* Copyright (c) 2009, 凯特科技 
* All rights reserved. 
*  
* 文件名称：$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/Common/NVIC.c $ 
* 摘    要：
*  
* 当前版本：$Revision: 1739 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$Date:: 2011-03-20 15:53:36#$
*
*/  
#include <LPC17xx.h>            /* LPC23XX/24xx Peripheral Registers */    
#include "Target.h"    
#include "NVIC.h"    
#include "absacc.h"
DWORD VectorsTable[128] __at(0x10000000);
   
/* Initialize the interrupt controller */   
/******************************************************************************  
** Function name:       init_VIC  
**  
** Descriptions:        Initialize VIC interrupt controller.  
** parameters:          None  
** Returned value:      None  
**   
******************************************************************************/ 
extern DWORD __Vectors; 
void InitVIC(void)    
{   
  BYTE i = 0;
  for (i = 0; i < 128; i++)
    VectorsTable[i] = *(DWORD*)((i * 4));
  SCB->VTOR = (DWORD)VectorsTable;    
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
void InstallIRQ(int IntNumber, DWORD HandlerAddr)   
{   
	VectorsTable[IntNumber + 16] = HandlerAddr;
}  
/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            禁止中断
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
__asm DWORD IrqDisable(void)
{
    CPSID   I
    MOV     R0, #0
    BX      LR
}

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            允许中断
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
__asm DWORD IrqEnable(void)
{
    CPSIE   I
    MOV     R0, #0
    BX      LR
}
/******************************************************************************  
**                            End Of File  
******************************************************************************/  
