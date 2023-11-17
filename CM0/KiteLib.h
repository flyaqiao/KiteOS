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

typedef unsigned char  BYTE; 
typedef unsigned short WORD; 
typedef unsigned long  DWORD; 

typedef BYTE*					 PBYTE;
typedef WORD*          PWORD;
typedef DWORD*         PDWORD;
typedef enum { FALSE = 0, TRUE = !FALSE } BOOL; 

#ifndef NULL
#define NULL							((void*) 0)
#endif
#define INFINITE          (-1)

#define OS_TICKS_PER_SEC  200             // 系统节拍

void    OsInit(void);                                   
void    OsStart(void);
void    OsCreateTask(void *addtsk, DWORD *stkTop, BYTE taskNo, void* param);;
void    OsWaitDelay(int dlyTime);
BYTE    OsWaitSignal(DWORD signal, int dlyTime);
void    OsSetSignal(DWORD signal);
BYTE    OsWaitMail(BYTE mailNo, int dlyTime);
DWORD   OsGetMail(BYTE mailNo);
//extern void   OsSendMail(BYTE mln, DWORD mail);
//extern void   OsIsrSendMail(BYTE mln);
DWORD   OsGetTimer(void);
void    OsResetTimer(void);
DWORD   OsEnterCritical(void);
DWORD   OsExitCritical(void);
void    OsIntEnter(void);
void    OsIntExit(void);
void    UartInit(DWORD baudRate, DWORD iRecvNewLineEvent);
BYTE    UartReadBuf(BYTE *pBuf, DWORD count);
void    UartSendByte(BYTE dat);
BYTE    UartHasData(void);
void    InstallIRQ(int IntNumber, void* HandlerAddr);
void    DelayUs(DWORD ns);
void    DelayMs(DWORD ms);

#endif // __KITELIB_H__
