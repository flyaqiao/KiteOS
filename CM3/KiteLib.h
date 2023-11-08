/* 
* Copyright (c) 2011 - 2011, 凯特科技
* All rights reserved. 
* 
*  
* 文件名称：$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Sys.h $ 
* 摘    要：
*  
* 当前版本：V0.1 $Revision: 642 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$LastChangedDate:: 2011-10-26 23:20:54#$
*
*/
#ifndef __KITELIB_H__
#define __KITELIB_H__

typedef unsigned char     BYTE; 
typedef unsigned short    WORD; 
typedef unsigned long     DWORD; 
typedef unsigned int      BOOL; 

typedef BYTE*					    LPBYTE;

#define TRUE              1
#define FALSE             0 
#define NULL              0
#define INFINITE          0xFFFF

#define OS_TICKS_PER_SEC  200             // 系统节拍

void    OsInit(void);                                   
void    OsStart(void);
void    OsCreateTask(void *addtsk, DWORD *stkTop, BYTE taskNo, void* param);;
void    OsWaitDelay(WORD dlyTime);
BYTE    OsWaitSignal(DWORD signal, WORD dlyTime);
void    OsSetSignal(DWORD signal);
BYTE    OsWaitMail(BYTE mailNo, WORD dlyTime);
DWORD   OsGetMail(BYTE mailNo);
//extern void   OsSendMail(BYTE mln, DWORD mail);
//extern void   OsIsrSendMail(BYTE mln);
DWORD   OsGetTimer(void);
void    OsResetTimer(void);
DWORD   OsEnterCritical(void);
DWORD   OsExitCritical(void);
void    OsIntEnter(void);
void    OsIntExit(void);
void    UartInit(void);
void    InstallIRQ(int IntNumber, DWORD HandlerAddr);

#define P0_00			0x0000
#define P0_01			0x0001
#define P0_02			0x0002
#define P0_03			0x0003
#define P0_04			0x0004
#define P0_05			0x0005
#define P0_06			0x0006
#define P0_07			0x0007
#define P0_08			0x0008
#define P0_09			0x0009
#define P0_10			0x000A
#define P0_11			0x000B
#define P0_15			0x000F
#define P0_16			0x0010
#define P0_17			0x0011
#define P0_18			0x0012
#define P0_19			0x0013
#define P0_20			0x0014
#define P0_21			0x0015
#define P0_22			0x0016
#define P0_23			0x0017
#define P0_24			0x0018
#define P0_25			0x0019
#define P0_26			0x001A
#define P0_27			0x001B
#define P0_28			0x001C
#define P0_29			0x001D
#define P0_30			0x001E

#define P1_00			0x0100
#define P1_01			0x0101
#define P1_04			0x0104    
#define P1_08			0x0108
#define P1_09			0x0109
#define P1_10			0x010A
#define P1_14			0x010E
#define P1_15			0x010F
#define P1_16			0x0110
#define P1_17			0x0111
#define P1_18			0x0112
#define P1_19			0x0113
#define P1_20			0x0114
#define P1_21			0x0115
#define P1_22			0x0116
#define P1_23			0x0117
#define P1_24			0x0118
#define P1_25			0x0119
#define P1_26			0x011A
#define P1_27			0x011B
#define P1_28			0x011C
#define P1_29			0x011D
#define P1_30			0x011E
#define P1_31			0x011F

//   -- P2 -- 

#define P2_00     0x0200
#define P2_01     0x0201
#define P2_02     0x0202
#define P2_03     0x0203
#define P2_04     0x0204
#define P2_05     0x0205
#define P2_06     0x0206
#define P2_07     0x0207
#define P2_08     0x0208
#define P2_09     0x0209
#define P2_10     0x020A
#define P2_11     0x020B
#define P2_12     0x020C
#define P2_13     0x020D

//   -- P3 -- 
#define P3_25     0x0319
#define P3_26     0x031A

//   -- P4 -- 
#define P4_28     0x041C
#define P4_29     0x041D

BYTE GpioGet(WORD portPin);

void GpioClr(WORD portPin);

void GpioSet(WORD portPin);

void GpioCpl(WORD portPin);

#endif // __KITELIB_H__
