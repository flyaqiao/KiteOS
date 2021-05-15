/* 
* Copyright (c) 2009, ���ؿƼ� 
* All rights reserved. 
*  
* �ļ����ƣ�$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/Common/NVIC.h $ 
* ժ    Ҫ��
*  
* ��ǰ�汾��$Revision: 1698 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$Date:: 2011-03-13 13:28:24#$
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
** Descriptions:            ��ֹ�ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern __asm DWORD IrqDisable(void);

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            �����ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern __asm DWORD IrqEnable(void);

#endif /* end __NVIC_H */ 
 
/****************************************************************************** 
**                            End Of File 
******************************************************************************/
