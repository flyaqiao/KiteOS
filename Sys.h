/* 
* Copyright (c) 2011 - 2011, ���ؿƼ�
* All rights reserved. 
* 
*  
* �ļ����ƣ�$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Sys.h $ 
* ժ    Ҫ��
*  
* ��ǰ�汾��V0.1 $Revision: 642 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$LastChangedDate:: 2011-10-26 23:20:54#$
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

#define OS_MAX_TASK       7               // ���������
#define OS_MAX_SIGNAL     16              // ����ź���
#define OS_MAX_MAIL       16               // �������
#define OS_TIMER_EN       1
#define OS_TICKS_PER_SEC  200             // ϵͳ����

// ����״̬˵��
typedef enum TaskStatus
{
  TSNone,           // û�и�����
  TSActive,         // ��������
  TSDelay,          // �����ӳ�
  TSWaitSignal,     // ����ȴ��ź�
  TSWaitMail        // ����ȴ�����
} TASKSTATUS;

typedef struct OsTcb
{
  TASKSTATUS  Status;             // ����״̬
  DWORD       Stack;              // �����ջָ��
  DWORD       Delay;              // �����ӳ�
#if OS_MAX_SIGNAL > 0
  DWORD       WaitSignal;         // ����ȴ��ź���
#endif
#if OS_MAX_MAIL > 0
  DWORD       WaitMail;           // ����ȴ�����
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
