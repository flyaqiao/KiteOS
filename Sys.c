/* 
* Copyright (c) 2011 - 2011, ���ؿƼ�
* All rights reserved. 
* 
*  
* �ļ����ƣ�$HeadURL: svn://www.svnhost.cn/Others/Trunk/UcosTest/Sys.c $ 
* ժ    Ҫ��
*   ������LPC1114�ϵĲ���ϵͳ��CPU�ں�CortexM0
*   ��Ҫ���ԣ�
*   1��֧��ʱ��Ƭ���ȡ�
*   2��֧��32���ź�signal��32�����䣬ÿ��������Դ���һ��32λ������
*   3��֧�ְ�����������ջ��
*   4�������ջ�����RAM��ַ��ʼ���䡣
* ��ǰ�汾��V0.1 $Revision: 657 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$LastChangedDate:: 2011-11-01 13:24:13#$
*
*/
#include "Sys.h"  
#include "Target.h"  

//ϵͳ����------------------------
DWORD OsvIdle;						        // ���м���,��ֹ����������
BYTE  CurTaskNo;
//DWORD OsvTopStack;					      // ����ɷ���������ջ��
DWORD OsIntNesting;               // �ж�Ƕ��
#if OS_MAX_SIGNAL > 0
DWORD OsvSignal;						      // 32��signal  
#endif
#if OS_MAX_MAIL > 0
DWORD OsvMailSignal;						  // 32�������ź�
DWORD OsvMailBox[OS_MAX_MAIL];		// 32������
#endif
#if OS_TIMER_EN > 0
DWORD OsvTimer;					          // ϵͳʱ��
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
// Ӧ�ó������
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
 * ��������
 */
void OsIdle(void)
{        
	OsvIdle = 0;
	while (1)
	{
		//��ֹSysTick����-------------
		OsvIdle++;
		if (OsvIdle > 0x00FF0000)
		{
      NVIC_SystemReset(); // ��λ
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
 * ��������
 * ����Ļ�����ջR4-R7,R0-R3,R12,LR,return Addr,xCPSR	
 */
void OsCreateTask(void* addtsk, DWORD* stkTop, BYTE taskNo)
{
	//��ʼ�������ջ--------------
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
	g_osTCB[taskNo].Status      = TSActive;                     // ����״̬Ϊ����   
#if OS_MAX_SIGNAL > 0
	g_osTCB[taskNo].WaitSignal  = 0;                            // �ź���
#endif
#if OS_MAX_MAIL > 0
  g_osTCB[taskNo].WaitMail    = 0;                            // ����          
#endif
//	OsvTopStack                 = OsvTopStack - stkSize;        // ���������ջ�ռ�
}

/*
 * ��ʼ��������ϵͳ
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
 * ���ҵ�ǰ�������񣬲��Ұ����ȼ���ߵ�������Ϊ��ǰ����
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
 * ��ʼ��������
 */
void OsStart()
{
	OsHighStart();	
}

/*
 * ϵͳtick����
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
 * ���������ӳ�dlyTime��ticks
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
 * �ѵ�ǰ�����sp���浽�������
 */
void OsSaveNowSp(DWORD nowsp)
{
	g_osTCB[g_iActiveTCB].Stack = nowsp;	
}
/*
 * ��ȡ��ǰ�����SP
 */
DWORD OsReadNowSp(void)
{
	return g_osTCB[g_iActiveTCB].Stack;
} 
#if OS_TIMER_EN > 0
/*
 * ����ĳ��ϵͳʱ��
 */
void OsResetTimer(void)
{
	OsvTimer = 0;
}        
/*
 * ����ϵͳʱ��ֵ
 */
DWORD OsGetTimer(void)
{
	return OsvTimer;
}
#endif
#if OS_MAX_SIGNAL > 0
/*
 * �ȴ�ĳһ���ź���
 */
BYTE OsWaitSignal(DWORD signal, WORD dlyTime)
{
	//���ȼ���ź����Ƿ���Ч------
	if ((OsvSignal & signal) == signal)
	{
		OsvSignal = OsvSignal & (~signal);
		return (1);
	}
	if (dlyTime == 0) return(0);
	//�ź���Ч�л�����------------
	g_osTCB[g_iActiveTCB].Status         = TSWaitSignal;
	g_osTCB[g_iActiveTCB].WaitSignal     = signal;
	g_osTCB[g_iActiveTCB].Delay          = dlyTime;
	OsTaskSwitch();
	//Ȼ���ټ���ź����Ƿ���Ч----
	if ((OsvSignal & signal) == signal)
	{
		OsvSignal = OsvSignal & (~signal);
		return (1);
	}
	return (0);
}

/*
 * �ͷ�һ���ź���
 */
void OsSetSignal(DWORD signal)
{
	OsvSignal = OsvSignal | signal;
}
#endif
#if OS_MAX_MAIL > 0
/*
 * �ȴ�һ������
 */
BYTE OsWaitMail(BYTE mailNo, WORD dlyTime)
{
	DWORD tmp32 = (1 << mailNo);

	//���ȼ�������Ƿ���Ч--------
	if ((OsvMailSignal & tmp32) != 0)
	{
		tmp32 = ~tmp32;
		OsvMailSignal = OsvMailSignal & tmp32;
		return (1);
	}
	if (dlyTime == 0) return(0);
	//�ź���Ч�л�����------------
	g_osTCB[g_iActiveTCB].Status         = TSWaitMail;
	g_osTCB[g_iActiveTCB].WaitMail       = tmp32;
	g_osTCB[g_iActiveTCB].Delay          = dlyTime;
	OsTaskSwitch();
	//Ȼ���ټ���ź����Ƿ���Ч----
	if ((OsvMailSignal & tmp32) != 0)
	{
		tmp32 = ~tmp32;
		OsvMailSignal = OsvMailSignal & tmp32;
		return (1);
	}
	return (0);
}

/*
 * �õ���������� 
 */
DWORD OsGetMail(BYTE mailNo)
{
	return (OsvMailBox[mailNo]);
}

/*
 * ����һ���ʼ�
 */
void OsSendMail(BYTE mailNo, DWORD mail)
{
	DWORD tmp32 = 1 << mailNo;

	OsvMailSignal = OsvMailSignal | tmp32;
	OsvMailBox[mailNo] = mail;
}
#endif
/*
 * ARM ��ش���
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
