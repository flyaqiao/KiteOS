/* 
* Copyright (c) 2011 - 2011, 凯特科技
* All rights reserved. 
* 
*  
* 文件名称：$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Sys.c $ 
* 摘    要：
*   运行在LPC1114上的操作系统，CPU内核CortexM0
*   主要特性：
*   1、支持时间片调度。
*   2、支持32个信号signal，32个邮箱，每个邮箱可以传递一个32位整数。
*   3、支持按需分配任务堆栈。
*   4、任务堆栈从最大RAM地址开始分配。
* 当前版本：V0.1 $Revision: 657 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$LastChangedDate:: 2011-11-01 13:24:13#$
*
*/
#include "Sys.h"  
#include "Target.h"  

//系统变量------------------------
DWORD OsvIdle;						        // 空闲计数,防止多任务死锁
BYTE  CurTaskNo;
//DWORD OsvTopStack;					      // 任务可分配的任务堆栈顶
DWORD OsIntNesting;               // 中断嵌套
#if OS_MAX_SIGNAL > 0
DWORD OsvSignal;						      // 32个signal  
#endif
#if OS_MAX_MAIL > 0
DWORD OsvMailSignal;						  // 32个邮箱信号
DWORD OsvMailBox[OS_MAX_MAIL];		// 32个邮箱
#endif
#if OS_TIMER_EN > 0
DWORD OsvTimer;					          // 系统时间
#endif

//#define SYSSTACKSIZE  (1024 * 4)
OS_TCB  g_osTCB[OS_MAX_TASK + 1];
DWORD   g_iActiveTCB = OS_MAX_TASK;
//DWORD   g_iBHighRdyTCB = OS_MAX_TASK;
void    OsHighStart(void);
void    OsIntSwitch(void);
void    OsTaskSwitch(void);
void    OsSched(void);
//BYTE   SysStack[SYSSTACKSIZE];
__weak void WDTInit(void);
__weak void Feed(void);
// 应用程序入口
void AppEntry(void); 
__weak void GpioConfig(void); 
DWORD AppEntryStack[64];       
DWORD OsIdleStack[64]; 
#ifdef __RELEASE
BOOL CheckUSN(void);
#endif

int main()
{       
  SystemInit();
#ifdef __RELEASE
	if (CheckUSN() == FALSE) while (1);
#endif
  GpioConfig();
  WDTInit();
  OsInit();
	OsCreateTask(AppEntry, AppEntryStack + 64, 6);
  CurTaskNo = 5;
  OsStart();
  while (1);
}   
/*
 * 空闲任务
 */
void OsIdle(void)
{        
	OsvIdle = 0;
	while (1)
	{
		//防止SysTick死机-------------
		OsvIdle++;
		if (OsvIdle > 0x00FF0000)
		{
      NVIC_SystemReset(); // 复位
		}
    OsSched();
	}
}
void OsSched(void)
{       
  BYTE i = 0;
  OsEnterCritical();
  if (OsIntNesting == 0)
  {
    for (i = 0; i < OS_MAX_TASK; i++)
    {
      if (g_osTCB[i].Status == TSNone)
        continue;  
      if (g_osTCB[i].Status == TSActive && i < g_iActiveTCB)
      {      
        OsExitCritical();
        OsIntSwitch();
        break;
      }
    }
  }
  OsExitCritical();
}
/*
 * 创建任务
 * 任务的基本堆栈R4-R7,R0-R3,R12,LR,return Addr,xCPSR	
 */
void OsCreateTask(void* addtsk, DWORD* stkTop, BYTE taskNo)
{
	//初始化任务堆栈--------------
	DWORD * stk = stkTop;
	if (taskNo > OS_MAX_TASK)
	{		
		return;
	}
	*(--stk) = 0x01000000;         // xPSP
	*(--stk) = (DWORD)addtsk;      // PC
  *(--stk) = (DWORD)addtsk;      // LR
  *(--stk) = 0;                  // R12
  *(--stk) = 0;                  // R3    
  *(--stk) = 0;                  // R2
  *(--stk) = 0;                  // R1  
  *(--stk) = 0;                  // R0  
  *(--stk) = 0;                  // R7    
  *(--stk) = 0;                  // R6
  *(--stk) = 0;                  // R5  
  *(--stk) = 0;                  // R4 
  *(--stk) = 0;                  // R11    
  *(--stk) = 0;                  // R10
  *(--stk) = 0;                  // R9  
  *(--stk) = 0;                  // R8
	g_osTCB[taskNo].Stack       = (DWORD)stk;
	g_osTCB[taskNo].Status      = TSActive;                     // 任务状态为激活   
#if OS_MAX_SIGNAL > 0
	g_osTCB[taskNo].WaitSignal  = 0;                            // 信号量
#endif
#if OS_MAX_MAIL > 0
  g_osTCB[taskNo].WaitMail    = 0;                            // 邮箱          
#endif
//	OsvTopStack                 = OsvTopStack - stkSize;        // 留出任务堆栈空间
}

/*
 * 初始化多任务系统
 */
void OsInit(void)
{
	BYTE i;
	OsIntNesting = 0;
//	OsvTopStack = (DWORD)(SysStack + SYSSTACKSIZE);
	for (i = 0; i < OS_MAX_TASK; i++)
  {
    g_osTCB[i].Status = TSNone; 
    g_osTCB[i].Stack  = 0;
    g_osTCB[i].Delay  = 0;
  }
#if OS_MAX_SIGNAL > 0
	OsvSignal     = 0;
#endif
#if OS_MAX_MAIL > 0
	OsvMailSignal = 0;
#endif
	OsCreateTask(OsIdle, OsIdleStack + 64, OS_MAX_TASK);
}

/*
 * 查找当前激活任务，并且把优先级最高的任务设为当前任务
 */
DWORD OsFindActTask(void)
{
	BYTE i;

	for (i = 0; i < OS_MAX_TASK; i++)
	{
		if (g_osTCB[i].Status == TSActive)
			break;
	}
  g_iActiveTCB = i;
	return (g_osTCB[i].Stack);
}
/*
 * 开始运行任务
 */
void OsStart()
{
	OsHighStart();	
}

/*
 * 系统tick处理
 */
void OsTick(void)
{
  BYTE i = 0;
//  BYTE needSw = 0;
	OsvIdle = 0;
#if OS_TIMER_EN > 0	
  OsvTimer++;
#endif
  Feed();
  for (i = 0; i < OS_MAX_TASK; i++)
  {
    if (g_osTCB[i].Status == TSNone)
      continue;  
    if (g_osTCB[i].Delay != 0)
    {
      if (--g_osTCB[i].Delay == 0)
        g_osTCB[i].Status = TSActive;
    }
#if OS_MAX_SIGNAL > 0
    if (g_osTCB[i].Status == TSWaitSignal && (g_osTCB[i].WaitSignal & OsvSignal) == g_osTCB[i].WaitSignal)
      g_osTCB[i].Status = TSActive; 
#endif
#if OS_MAX_MAIL > 0
    if (g_osTCB[i].Status == TSWaitMail && (g_osTCB[i].WaitMail & OsvMailSignal) == g_osTCB[i].WaitMail)
      g_osTCB[i].Status = TSActive;      
#endif
  }         
}

/*
 * 挂起任务延迟dlyTime个ticks
 */
void OsWaitDelay(WORD dlyTime)
{
  OsEnterCritical();
	g_osTCB[g_iActiveTCB].Status = TSDelay;
	g_osTCB[g_iActiveTCB].Delay  = dlyTime;
  OsExitCritical();
	OsTaskSwitch();
}
/*
 * 把当前任务的sp保存到任务块中
 */
void OsSaveNowSp(DWORD nowsp)
{
	g_osTCB[g_iActiveTCB].Stack = nowsp;	
}
/*
 * 读取当前任务的SP
 */
DWORD OsReadNowSp(void)
{
	return g_osTCB[g_iActiveTCB].Stack;
} 
#if OS_TIMER_EN > 0
/*
 * 清零某个系统时间
 */
void OsResetTimer(void)
{
	OsvTimer = 0;
}        
/*
 * 返回系统时间值
 */
DWORD OsGetTimer(void)
{
	return OsvTimer;
}
#endif
#if OS_MAX_SIGNAL > 0
/*
 * 等待某一个信号量
 */
BYTE OsWaitSignal(DWORD signal, WORD dlyTime)
{
	//首先检查信号量是否有效------
	if ((OsvSignal & signal) == signal)
	{
		OsvSignal = OsvSignal & (~signal);
		return (1);
	}
	if (dlyTime == 0) return(0);
	//信号无效切换任务------------
	g_osTCB[g_iActiveTCB].Status         = TSWaitSignal;
	g_osTCB[g_iActiveTCB].WaitSignal     = signal;
	g_osTCB[g_iActiveTCB].Delay          = dlyTime;
	OsTaskSwitch();
	//然后再检查信号量是否有效----
	if ((OsvSignal & signal) == signal)
	{
		OsvSignal = OsvSignal & (~signal);
		return (1);
	}
	return (0);
}

/*
 * 释放一个信号量
 */
void OsSetSignal(DWORD signal)
{
	OsvSignal = OsvSignal | signal;
}
#endif
#if OS_MAX_MAIL > 0
/*
 * 等待一个邮箱
 */
BYTE OsWaitMail(BYTE mailNo, WORD dlyTime)
{
	DWORD tmp32 = (1 << mailNo);

	//首先检查邮箱是否有效--------
	if ((OsvMailSignal & tmp32) != 0)
	{
		tmp32 = ~tmp32;
		OsvMailSignal = OsvMailSignal & tmp32;
		return (1);
	}
	if (dlyTime == 0) return(0);
	//信号无效切换任务------------
	g_osTCB[g_iActiveTCB].Status         = TSWaitMail;
	g_osTCB[g_iActiveTCB].WaitMail       = tmp32;
	g_osTCB[g_iActiveTCB].Delay          = dlyTime;
	OsTaskSwitch();
	//然后再检查信号量是否有效----
	if ((OsvMailSignal & tmp32) != 0)
	{
		tmp32 = ~tmp32;
		OsvMailSignal = OsvMailSignal & tmp32;
		return (1);
	}
	return (0);
}

/*
 * 得到邮箱的内容 
 */
DWORD OsGetMail(BYTE mailNo)
{
	return (OsvMailBox[mailNo]);
}

/*
 * 发送一个邮件
 */
void OsSendMail(BYTE mailNo, DWORD mail)
{
	DWORD tmp32 = 1 << mailNo;

	OsvMailSignal = OsvMailSignal | tmp32;
	OsvMailBox[mailNo] = mail;
}
#endif
/*
 * ARM 相关代码
 */
void OsInitTick(void)
{
  SysTick_Config(SystemFrequency / OS_TICKS_PER_SEC);
}
void SysTick_Handler(void)
{         
  OsIntEnter();
  OsTick();  
  OsIntExit();
}

void OsIntEnter(void)
{         
  OsEnterCritical();
  OsIntNesting++;    
  OsExitCritical();
}
void OsIntExit(void)
{
  OsEnterCritical();
  OsIntNesting--;
  OsExitCritical();
  OsSched();
}
