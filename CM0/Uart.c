/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2009, 凯特科技
 *   All rights reserved.
 *
 *   History
 *   2006.07.12  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <LPC11xx.H>  
#include <stdio.h>
#include "Target.h"
#include "Uart.h" 

__weak void PutByte(char ch);
__weak int  GetByte(char* ch)
{
	return 0;
}
// #define BUF_ISFULL(buf)						((buf).Count == UARTBUF_SIZE)
// #define BUF_ISEMPTY(buf)					((buf).Count == 0)
// #define BUF_NOTEMPTY(buf)					((buf).Count > 0)
// #define BUF_POINTERADD(pointer)		((pointer) = ((pointer) + 1) % UARTBUF_SIZE)
// UARTBUFDEF					RecvBuffer;
// #if SUPPORT_SENDBUF
// UARTBUFDEF					SendBuffer;
// #endif
// DWORD								RecvNewLineEvent = 0;
// __inline void WriteByte(PUARTBUFDEF pBuf, BYTE value)
// {
// 	if (!BUF_ISFULL(*pBuf))
// 	{
// 		pBuf->Buf[pBuf->Head] = value;
// 		BUF_POINTERADD(pBuf->Head);
// 		pBuf->Count++;
// 		if (RecvNewLineEvent != 0 && value == '\n')
// 			OsSetSignal(RecvNewLineEvent);
// 	}
// }						

// __inline BYTE ReadBuf(PUARTBUFDEF pBuf, BYTE *pBuffer, BYTE count)
// {
// 	BYTE iCount = count < pBuf->Count ? count : pBuf->Count;
// 	BYTE* p = pBuffer;
// 	if (pBuf->Count == 0)
// 		return 0;												// 队列空
// 	if (pBuf->Rear + iCount <= UARTBUF_SIZE)
// 	{
// 		memcpy(p, &pBuf->Buf[pBuf->Rear], iCount);	
// 		pBuf->Rear += iCount;
// 	}
// 	else
// 	{
// 		memcpy(p, &pBuf->Buf[pBuf->Rear], UARTBUF_SIZE - pBuf->Rear);
// 		p += (UARTBUF_SIZE - pBuf->Rear);
// 		pBuf->Rear = (pBuf->Rear + iCount) % UARTBUF_SIZE;
// 		memcpy(p, &pBuf->Buf[0], pBuf->Rear); 
// 	}							 		 
// 	pBuf->Count -= iCount;
// 	return iCount;
// }	

// BYTE ReadByte(PUARTBUFDEF pBuf)
// {
//   BYTE ret;
// 	ret = pBuf->Buf[pBuf->Rear];
// 	BUF_POINTERADD(pBuf->Rear);
// 	pBuf->Count--;
//   return ret;
// }

// #if SUPPORT_SENDBUF
// BYTE UartSendBuf(BYTE* pBuf, DWORD size)
// {
// 	int i = 0;
// 	NVIC_DisableIRQ(UART_IRQn);
// 	for (i = 0; i < size; i++)
// 		WriteByte(&g_Uart.SendBuffer, pBuf[i]);	
// 	if ((LPC_UART->LSR & 0x40) != 0)		
// 		LPC_UART->THR = ReadByte(&g_Uart.SendBuffer);							
// 	NVIC_EnableIRQ(UART_IRQn);
// 	return i;
// }   
// #endif

void UartSendByte(BYTE dat)
{                                                          
	while ((LPC_UART->LSR & 0x40) == 0);			// 等待数据发送完毕 
	LPC_UART->THR = dat;                                      
}
// BYTE UartReadBuf(BYTE *pBuf, DWORD count)
// {							
// 	BYTE iCount = 0;						 
// 	NVIC_DisableIRQ(UART_IRQn);			// 保护字符接收数组不被其它中断破坏
// 	iCount = ReadBuf(&RecvBuffer, pBuf, count);	 
// 	NVIC_EnableIRQ(UART_IRQn);
// 	return iCount;
// }

// BYTE UartReadByte(void)
// {
//   BYTE ret;
// 	NVIC_DisableIRQ(UART_IRQn);			// 保护字符接收数组不被其它中断破坏
// 	ret = ReadByte(&RecvBuffer);
// 	NVIC_EnableIRQ(UART_IRQn);
//   return ret;
// } 

// BYTE UartHasData(void)
// {
//   return BUF_NOTEMPTY(RecvBuffer);
// }	
/*********************************************************************************************************
** 函数名称：IRQ_UART0
** 函数功能：串口0发送中断服务程序
** 入口参数：无
** 出口参数：无
*******************************************************************************************************/
void UART_IRQHandler(void)
{
	DWORD IIR	= 0;
	DWORD i	= 0;
	/* 			UART0发送中断处理 			*/
	while (((IIR = LPC_UART->IIR) & 0x01) == 0)	    // 若尚有中断待处理
	{
		switch (IIR & 0x0E)
		{                
// 			case IIR_THRE:		// 发送中断处理
// 				for (i = 0; i < 16; i++)
// 				{	
// 					char ch = 0;
// 					if (GetByte(&ch))
// 						LPC_UART->THR = ch;
// 					else
// 						break;
// 				}
// 				break;				
			/* RDA中断处理入口 */
			case IIR_RDA:			
			/* CTI中断处理入口 */
			case IIR_CTI:
				for (i = 0; i < 16; i++)	
				{
					if ((LPC_UART->LSR & 0x01) == 0x01)
						PutByte(LPC_UART->RBR);								// 读空FIFO，清除CTI中断
					else
						break;
				}
		
			default:
				break;
		}
	}				
}

/*********************************************************************************************************
** 函数名称：UART0_Ini
** 函数功能：按默认值初始化串口0的引脚和通讯参数。设置为8位数据位，1位停止位，无奇偶校验，波特率。
** 入口参数：无 
** 出口参数：无
**********************************************************************************************************/
void UartInit(DWORD baudRate, DWORD iRecvNewLineEvent)
{                           
	DWORD 	Fdiv = 0;
//	RecvNewLineEvent = iRecvNewLineEvent;
  NVIC_DisableIRQ(UART_IRQn);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 12);                                    /*  打开定时器模块              */
  LPC_SYSCON->UARTCLKDIV    = 0x01;

	/* 初始化相关的IO */
  LPC_IOCON->PIO1_6 = 0x01;       /* RxD0 and TxD0 */
  LPC_IOCON->PIO1_7 = 0x01;       /* RxD0 and TxD0 */
	
	/* 初始化串口通讯参数 */
	LPC_UART->LCR 	 = 0x83;							            // 设置为8位数据位，1位停止位，无奇偶校验，DLAB=1, 允许设置波特率
  Fdiv 	 = (FAHBCLK / 16) / baudRate;		          	// 设置波特率
	LPC_UART->DLM 	 = Fdiv / 256;
	LPC_UART->DLL 	 = Fdiv % 256;
	LPC_UART->LCR 	 = 0x03;							            // 令DLAB位为0
	
	/* 复位发送FIFO */
	LPC_UART->FCR 	|= 0xC7;					
	
	/* 小数分频器禁能 */
	LPC_UART->FDR   &= ~0x0F;						              // 令DIVADDVAL=0					        
	LPC_UART->FDR   |=  0x10;						              // MULVAL不可为0					        

	/* 初始化LPC_UART0->IER */
	LPC_UART->IER 	 = 0;
	
	NVIC_EnableIRQ(UART_IRQn);
	
  LPC_UART->IER 	= IER_RBR;							          // 使能THRE中断    
//  LPC_UART->IER 	= IER_THRE;
}
struct __FILE
{
  int handle;   /*用户需要的任何代码(如果使用文件仅是为了调试使用prinft在标准输出端输出信息，则不需要任何文件处理代码)*/
};
FILE __stdout;  /*FILE在stdio.h中定义为：typedef struct __FILE FILE；*/
int ferror(FILE* f)
{
  return EOF;            /*用户实现的ferror代码*/
}

int fputc(int ch, FILE *f)
{
  UartSendByte(ch);

  return ch;
} 
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

