/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2009, ���ؿƼ�
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
// 		return 0;												// ���п�
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
	while ((LPC_UART->LSR & 0x40) == 0);			// �ȴ����ݷ������ 
	LPC_UART->THR = dat;                                      
}
// BYTE UartReadBuf(BYTE *pBuf, DWORD count)
// {							
// 	BYTE iCount = 0;						 
// 	NVIC_DisableIRQ(UART_IRQn);			// �����ַ��������鲻�������ж��ƻ�
// 	iCount = ReadBuf(&RecvBuffer, pBuf, count);	 
// 	NVIC_EnableIRQ(UART_IRQn);
// 	return iCount;
// }

// BYTE UartReadByte(void)
// {
//   BYTE ret;
// 	NVIC_DisableIRQ(UART_IRQn);			// �����ַ��������鲻�������ж��ƻ�
// 	ret = ReadByte(&RecvBuffer);
// 	NVIC_EnableIRQ(UART_IRQn);
//   return ret;
// } 

// BYTE UartHasData(void)
// {
//   return BUF_NOTEMPTY(RecvBuffer);
// }	
/*********************************************************************************************************
** �������ƣ�IRQ_UART0
** �������ܣ�����0�����жϷ������
** ��ڲ�������
** ���ڲ�������
*******************************************************************************************************/
void UART_IRQHandler(void)
{
	DWORD IIR	= 0;
	DWORD i	= 0;
	/* 			UART0�����жϴ��� 			*/
	while (((IIR = LPC_UART->IIR) & 0x01) == 0)	    // �������жϴ�����
	{
		switch (IIR & 0x0E)
		{                
// 			case IIR_THRE:		// �����жϴ���
// 				for (i = 0; i < 16; i++)
// 				{	
// 					char ch = 0;
// 					if (GetByte(&ch))
// 						LPC_UART->THR = ch;
// 					else
// 						break;
// 				}
// 				break;				
			/* RDA�жϴ������ */
			case IIR_RDA:			
			/* CTI�жϴ������ */
			case IIR_CTI:
				for (i = 0; i < 16; i++)	
				{
					if ((LPC_UART->LSR & 0x01) == 0x01)
						PutByte(LPC_UART->RBR);								// ����FIFO�����CTI�ж�
					else
						break;
				}
		
			default:
				break;
		}
	}				
}

/*********************************************************************************************************
** �������ƣ�UART0_Ini
** �������ܣ���Ĭ��ֵ��ʼ������0�����ź�ͨѶ����������Ϊ8λ����λ��1λֹͣλ������żУ�飬�����ʡ�
** ��ڲ������� 
** ���ڲ�������
**********************************************************************************************************/
void UartInit(DWORD baudRate, DWORD iRecvNewLineEvent)
{                           
	DWORD 	Fdiv = 0;
//	RecvNewLineEvent = iRecvNewLineEvent;
  NVIC_DisableIRQ(UART_IRQn);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 12);                                    /*  �򿪶�ʱ��ģ��              */
  LPC_SYSCON->UARTCLKDIV    = 0x01;

	/* ��ʼ����ص�IO */
  LPC_IOCON->PIO1_6 = 0x01;       /* RxD0 and TxD0 */
  LPC_IOCON->PIO1_7 = 0x01;       /* RxD0 and TxD0 */
	
	/* ��ʼ������ͨѶ���� */
	LPC_UART->LCR 	 = 0x83;							            // ����Ϊ8λ����λ��1λֹͣλ������żУ�飬DLAB=1, �������ò�����
  Fdiv 	 = (FAHBCLK / 16) / baudRate;		          	// ���ò�����
	LPC_UART->DLM 	 = Fdiv / 256;
	LPC_UART->DLL 	 = Fdiv % 256;
	LPC_UART->LCR 	 = 0x03;							            // ��DLABλΪ0
	
	/* ��λ����FIFO */
	LPC_UART->FCR 	|= 0xC7;					
	
	/* С����Ƶ������ */
	LPC_UART->FDR   &= ~0x0F;						              // ��DIVADDVAL=0					        
	LPC_UART->FDR   |=  0x10;						              // MULVAL����Ϊ0					        

	/* ��ʼ��LPC_UART0->IER */
	LPC_UART->IER 	 = 0;
	
	NVIC_EnableIRQ(UART_IRQn);
	
  LPC_UART->IER 	= IER_RBR;							          // ʹ��THRE�ж�    
//  LPC_UART->IER 	= IER_THRE;
}
struct __FILE
{
  int handle;   /*�û���Ҫ���κδ���(���ʹ���ļ�����Ϊ�˵���ʹ��prinft�ڱ�׼����������Ϣ������Ҫ�κ��ļ��������)*/
};
FILE __stdout;  /*FILE��stdio.h�ж���Ϊ��typedef struct __FILE FILE��*/
int ferror(FILE* f)
{
  return EOF;            /*�û�ʵ�ֵ�ferror����*/
}

int fputc(int ch, FILE *f)
{
  UartSendByte(ch);

  return ch;
} 
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

