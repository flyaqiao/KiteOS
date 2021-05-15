/* 
* Copyright (c) 2009, 凯特科技 
* All rights reserved. 
*  
* 文件名称：$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/Common/Gpio.c $ 
* 摘    要：
*  
* 当前版本：$Revision: 1270 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$Date:: 2010-09-25 16:32:05#$
*
*/ 
#include <LPC17xx.H>
#include "Gpio.h"

#define GPIO_DIR
#define DIRBASE 0x2009C000 
#define PINBASE 0x2009C014
#define SETBASE 0x2009C018
#define CLRBASE 0x2009C01C

BYTE GpioGet(WORD portPin)
{
  BYTE port = portPin >> 8;
  BYTE val = portPin & 0x001F;//((DWORD)1 << (portPin & 0x00FF));
#ifdef GPIO_DIR
  volatile unsigned long* dir = (volatile unsigned long*)(DIRBASE + 0x20 * port);
#endif
  volatile unsigned long* pin = (volatile unsigned long*)(PINBASE + 0x20 * port);
#ifdef GPIO_DIR
  *dir &= ~(1UL << val);
#endif
  return ((*pin) >> val) & 0x0001;
}

void GpioClr(WORD portPin)
{
  WORD port = portPin >> 8;
  DWORD val = (1UL << (portPin & 0x001F));   
#ifdef GPIO_DIR
  volatile unsigned long* dir = (volatile unsigned long*)(DIRBASE + 0x20 * port);
#endif
  volatile unsigned long* clr = (volatile unsigned long*)(CLRBASE + 0x20 * port);
#ifdef GPIO_DIR
  *dir |= val;
#endif
  *clr = val;
	return;
}

void GpioSet(WORD portPin)
{
  WORD port = portPin >> 8;
  DWORD val = (1UL << (portPin & 0x001F));  
#ifdef GPIO_DIR
  volatile unsigned long* dir = (volatile unsigned long*)(DIRBASE + 0x20 * port);
#endif
  volatile unsigned long* set = (volatile unsigned long*)(SETBASE + 0x20 * port);
#ifdef GPIO_DIR     
  *dir |= val;
#endif
  *set = val;
	return;
}

void GpioCpl(WORD portPin)
{
  WORD port = portPin >> 8;
  DWORD val = (1UL << (portPin & 0x001F));
#ifdef GPIO_DIR
  volatile unsigned long* dir = (volatile unsigned long*)(DIRBASE + 0x20 * port);
#endif
  volatile unsigned long* pin = (volatile unsigned long*)(PINBASE + 0x20 * port);
#ifdef GPIO_DIR
  *dir |= val;
#endif
  *pin ^= val;//((*pin) & ~val) | (~(*pin) & val);
	return;
}

