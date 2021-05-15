/* 
* Copyright (c) 2009, 凯特科技 
* All rights reserved. 
*  
* 文件名称：$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/Common/NVIC.h $ 
* 摘    要：
*  
* 当前版本：$Revision: 1698 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$Date:: 2011-03-13 13:28:24#$
*
*/ 
#ifndef __NVIC_H  
#define __NVIC_H 
#include "Sys.h"
 
/* Be aware that, from compiler to compiler, nested interrupt will have to 
be handled differently. More details can be found in Philips LPC2000 
family app-note AN10381 */ 
  
/* unlike Keil CARM Compiler, in ARM's RealView compiler, don't save and  
restore registers into the stack in RVD as the compiler does that for you.  
See RVD ARM compiler Inline and embedded assemblers, "Rules for  
using __asm and asm keywords. */ 
 
void InitVIC(void); 
void InstallIRQ(int IntNumber, DWORD HandlerAddr); 
/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            禁止中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern __asm DWORD IrqDisable(void);

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            允许中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern __asm DWORD IrqEnable(void);

#endif /* end __NVIC_H */ 
 
/****************************************************************************** 
**                            End Of File 
******************************************************************************/
