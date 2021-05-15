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
#ifndef __SYS_H__
#define __SYS_H__

typedef unsigned char     BYTE; 
typedef unsigned short    WORD; 
typedef unsigned long     DWORD; 
typedef unsigned int      BOOL; 

typedef BYTE*					    LPBYTE;

#define TRUE              1
#define FALSE             0 
#define NULL              0
#define INFINITE          0xFFFF

#define OS_MAX_TASK       7               // 最大任务数
#define OS_MAX_SIGNAL     16              // 最大信号量
#define OS_MAX_MAIL       16               // 最大邮箱
#define OS_TIMER_EN       1
#define OS_TICKS_PER_SEC  200             // 系统节拍

// 任务状态说明
typedef enum TaskStatus
{
  TSNone,           // 没有该任务
  TSActive,         // 激活任务
  TSDelay,          // 任务延迟
  TSWaitSignal,     // 任务等待信号
  TSWaitMail        // 任务等待邮箱
} TASKSTATUS;

typedef struct OsTcb
{
  TASKSTATUS  Status;             // 任务状态
  DWORD       Stack;              // 任务堆栈指针
  DWORD       Delay;              // 任务延迟
#if OS_MAX_SIGNAL > 0
  DWORD       WaitSignal;         // 任务等待信号量
#endif
#if OS_MAX_MAIL > 0
  DWORD       WaitMail;           // 任务等待邮箱
#endif
} OS_TCB;

extern void   OsInit(void);                                   
extern void   OsStart(void);
extern void   OsCreateTask(void* addtsk, DWORD* stkTop, BYTE taskNo);
extern void   OsWaitDelay(WORD dlyTime);
#if OS_MAX_SIGNAL > 0
extern BYTE   OsWaitSignal(DWORD signal, WORD dlyTime);
extern void   OsSetSignal(DWORD signal);
#endif
#if OS_MAX_MAIL > 0
extern BYTE   OsWaitMail(BYTE mailNo, WORD dlyTime);
extern DWORD  OsGetMail(BYTE mailNo);
//extern void   OsSendMail(BYTE mln, DWORD mail);
//extern void   OsIsrSendMail(BYTE mln);
#endif
#if OS_TIMER_EN > 0
extern DWORD  OsGetTimer(void);
extern void   OsResetTimer(void);
#endif
extern DWORD  OsEnterCritical(void);
extern DWORD  OsExitCritical(void);
extern void   OsIntEnter(void);
extern void   OsIntExit(void);

#endif // __SYS_H__
