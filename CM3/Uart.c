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
#include <LPC17xx.H>  
#include <stdio.h>
#include "Target.h"
#include "NVIC.h"
#include "Uart.h"

#define BUF_ISFULL(buf)						((buf).Count == UARTBUF_SIZE)
#define BUF_ISEMPTY(buf)					((buf).Count == 0)
#define BUF_NOTEMPTY(buf)					((buf).Count > 0)
#define BUF_POINTERADD(pointer)		((pointer) = ((pointer) + 1) % UARTBUF_SIZE)

__inline void WriteByte(PUARTBUFDEF pBuf, BYTE value)
{
	if (!BUF_ISFULL(*pBuf))
	{
		pBuf->Buf[pBuf->Head] = value;
		BUF_POINTERADD(pBuf->Head);
		pBuf->Count++;
	}
}						

__inline BYTE ReadBuf(PUARTBUFDEF pBuf, BYTE *pBuffer, BYTE count)
{
	BYTE iCount = count < pBuf->Count ? count : pBuf->Count;
	BYTE* p = pBuffer;
	if (pBuf->Count == 0)
		return 0;												// ���п�
	if (pBuf->Rear + iCount <= UARTBUF_SIZE)
	{
		memcpy(p, &pBuf->Buf[pBuf->Rear], iCount);	
		pBuf->Rear += iCount;
	}
	else
	{
		memcpy(p, &pBuf->Buf[pBuf->Rear], UARTBUF_SIZE - pBuf->Rear);
		p += (UARTBUF_SIZE - pBuf->Rear);
		pBuf->Rear = (pBuf->Rear + iCount) % UARTBUF_SIZE;
		memcpy(p, &pBuf->Buf[0], pBuf->Rear); 
	}							 		 
	pBuf->Count -= iCount;
	return iCount;
}	

__inline ReadByte(PUARTBUFDEF pBuf)
{
  BYTE ret;
	ret = pBuf->Buf[pBuf->Rear];
	BUF_POINTERADD(pBuf->Rear);
	pBuf->Count--;
  return ret;
}

#if UART0_SETUP

UARTDEF g_Uart0;

__inline BYTE Uart0SendBuf(BYTE* pBuf, BYTE size)
{
	BYTE i = 0;
	NVIC_DisableIRQ(UART0_IRQn);
	for (i = 0; i < size; i++)
		WriteByte(&g_Uart0.SendBuffer, pBuf[i]);	
	if ((LPC_UART0->LSR & 0x40) != 0)		
		LPC_UART0->THR = ReadByte(&g_Uart0.SendBuffer);							
	NVIC_EnableIRQ(UART0_IRQn);
	return i;
}

__inline void Uart0SendByte(BYTE dat)
{
	LPC_UART0->THR = dat;
	while ((LPC_UART0->LSR & 0x40) == 0);			// �ȴ����ݷ������
//	NVIC_DisableIRQ(UART0_IRQn);
//	WriteByte(&g_Uart0.SendBuffer, dat);
//	if ((LPC_UART0->LSR & 0x40) != 0)		
//		LPC_UART0->THR = ReadByte(&g_Uart0.SendBuffer);										
//	NVIC_EnableIRQ(UART0_IRQn);
}

__inline BYTE Uart0ReadBuf(BYTE *pBuf, BYTE count)
{							
	BYTE iCount = 0;						 
	NVIC_DisableIRQ(UART0_IRQn);			// �����ַ��������鲻�������ж��ƻ�
	iCount = ReadBuf(&g_Uart0.RecvBuffer, pBuf, count);	 
	NVIC_EnableIRQ(UART0_IRQn);
	return iCount;
}

__inline BYTE Uart0ReadByte(void)
{
  BYTE ret;
	NVIC_DisableIRQ(UART0_IRQn);			// �����ַ��������鲻�������ж��ƻ�
	ret = ReadByte(&g_Uart0.RecvBuffer);
	NVIC_EnableIRQ(UART0_IRQn);
  return ret;
} 

__inline BYTE Uart0HasData(void)
{
  return BUF_NOTEMPTY(g_Uart0.RecvBuffer);
}	
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
	OsIntEnter();
	/* 			UART0�����жϴ��� 			*/
	while (((IIR = LPC_UART0->IIR) & 0x01) == 0)	    // �������жϴ�����
	{
		switch (IIR & 0x0E)
		{
			case IIR_THRE:		// �����жϴ���
				for (i = 0; i < 16; i++)
				{	
					if (BUF_NOTEMPTY(g_Uart0.SendBuffer))
						LPC_UART0->THR = ReadByte(&g_Uart0.SendBuffer);
					else
						break;
				}
				break;					
			/* RDA�жϴ������ */
			case IIR_RDA:			
			/* CTI�жϴ������ */
			case IIR_CTI:
				for (i = 0; i < 16; i++)	
				{
					if ((LPC_UART0->LSR & 0x01) == 0x01)
						WriteByte(&g_Uart0.RecvBuffer, LPC_UART0->RBR);	// ����FIFO�����CTI�ж�
					else
						break;
				}
		
			default:
				break;
		}
	}					
  OsIntExit();				
}

/*********************************************************************************************************
** �������ƣ�UART0_Ini
** �������ܣ���Ĭ��ֵ��ʼ������0�����ź�ͨѶ����������Ϊ8λ����λ��1λֹͣλ������żУ�飬�����ʡ�
** ��ڲ������� 
** ���ڲ�������
**********************************************************************************************************/
void Uart0Init(DWORD Baudrate)
{
	DWORD 	Fdiv = 0;
	
	LPC_SC->PCONP       |= (1 << 3);						// ��UART0
	/* ��ʼ����ص�IO */
  LPC_PINCON->PINSEL0 &= ~(0xF << 4);
  LPC_PINCON->PINSEL0 |= 0x00000050;          /* RxD0 and TxD0 */
	
	/* ��ʼ������ͨѶ���� */
	LPC_UART0->LCR 	 = 0x83;							      // ����Ϊ8λ����λ��1λֹͣλ������żУ�飬DLAB=1, �������ò�����
	Fdiv 	 = (FAHBCLK / 16) / Baudrate;			    // ���ò�����
	LPC_UART0->DLM 	 = Fdiv / 256;
	LPC_UART0->DLL 	 = Fdiv % 256;
	LPC_UART0->LCR 	 = 0x03;							      // ��DLABλΪ0
	
	/* ��λ����FIFO */
	LPC_UART0->FCR 	|= 0xC7;					
	
	/* С����Ƶ������ */
	LPC_UART0->FDR   &= ~0x0F;						      // ��DIVADDVAL=0					        
	LPC_UART0->FDR   |=  0x10;						      // MULVAL����Ϊ0					        

	/* ��ʼ��LPC_UART0->IER */
	LPC_UART0->IER 	 = 0;
	
//  InstallIRQ(UART0_IRQn, (DWORD)IRQ_UART0);	
	NVIC_EnableIRQ(UART0_IRQn);
	
  LPC_UART0->IER 	= IER_RBR;							// ʹ��THRE�ж�
}
#endif

#if UART1_SETUP

UARTDEF g_Uart1;

__inline BYTE Uart1SendBuf(BYTE* pBuf, BYTE size)
{
	BYTE i = 0;
	NVIC_DisableIRQ(UART1_IRQn);
//	for (i = 0; i < size; i++)
//	{
//		LPC_UART1->THR = pBuf[i];
//		if ((i + 1) % 16 == 0)
//	  	while ((LPC_UART1->LSR & 0x40) == 0);			// �ȴ����ݷ������
//	}
//	if (size % 16 != 0)
//		while ((LPC_UART1->LSR & 0x40) == 0);			// �ȴ����ݷ������
	for (i = 0; i < size; i++)
		WriteByte(&g_Uart1.SendBuffer, pBuf[i]);
	if ((LPC_UART1->LSR & 0x40) != 0)		
		LPC_UART1->THR = ReadByte(&g_Uart1.SendBuffer);	
	NVIC_EnableIRQ(UART1_IRQn);
	return i;
}

__inline void Uart1SendByte(BYTE dat)
{
//	LPC_UART1->THR = dat;
//	while ((LPC_UART1->LSR & 0x40) == 0);			// �ȴ����ݷ������
	NVIC_DisableIRQ(UART1_IRQn);
	WriteByte(&g_Uart1.SendBuffer, dat);									
	NVIC_EnableIRQ(UART1_IRQn);
}

__inline BYTE Uart1ReadBuf(BYTE *pBuf, BYTE count)
{							
	BYTE iCount = 0;						 
	NVIC_DisableIRQ(UART1_IRQn);			// �����ַ��������鲻�������ж��ƻ�
	iCount = ReadBuf(&g_Uart1.RecvBuffer, pBuf, count);	 
	NVIC_EnableIRQ(UART1_IRQn);
	return iCount;
}

__inline BYTE Uart1ReadByte(void)
{
  BYTE ret;
	NVIC_DisableIRQ(UART1_IRQn);			// �����ַ��������鲻�������ж��ƻ�
	ret = ReadByte(&g_Uart1.RecvBuffer);
	NVIC_EnableIRQ(UART1_IRQn);
  return ret;
} 

__inline BYTE Uart1HasData(void)
{
  return BUF_NOTEMPTY(g_Uart1.RecvBuffer);
}	

__inline BYTE Uart1ClearBuf(void)
{
  memset(&g_Uart1.RecvBuffer, 0, sizeof(UARTBUFDEF));  
  return 1;
}
/*********************************************************************************************************
** �������ƣ�IRQ_UART1
** �������ܣ�����0�����жϷ������
** ��ڲ�������
** ���ڲ�������
*******************************************************************************************************/
void IRQ_UART1(void)
{
	DWORD IIR	= 0;
	DWORD i	= 0;
	
	/* 			UART1�����жϴ��� 			*/
	while (((IIR = LPC_UART1->IIR) & 0x01) == 0)	    // �������жϴ�����
	{
		switch (IIR & 0x0E)
		{
			case 0x02:		// �����жϴ���
				for (i = 0; i < 16; i++)
				{	
					if (BUF_NOTEMPTY(g_Uart1.SendBuffer))
						LPC_UART1->THR = ReadByte(&g_Uart1.SendBuffer);
					else
						break;
				}
				break;					
			/* RDA�жϴ������ */
			case 0x04:			
				for (i = 0; i < 4; i++)	
					WriteByte(&g_Uart1.RecvBuffer, LPC_UART1->RBR);	// ����FIFO�����CTI�ж�
				break;
			/* CTI�жϴ������ */
			case 0x0C:
				for (i = 0; i < 16; i++)	
				{
					if ((LPC_UART1->LSR & 0x01) == 0x01)
						WriteByte(&g_Uart1.RecvBuffer, LPC_UART1->RBR);	// ����FIFO�����CTI�ж�
					else
						break;
				}
		
			default:
				break;
		}
	}					
}

/*********************************************************************************************************
** �������ƣ�UART1_Ini
** �������ܣ���Ĭ��ֵ��ʼ������0�����ź�ͨѶ����������Ϊ8λ����λ��1λֹͣλ������żУ�飬�����ʡ�
** ��ڲ������� 
** ���ڲ�������
**********************************************************************************************************/
void Uart1Init(DWORD Baudrate)
{
	DWORD 	Fdiv = 0;
	
	/* ��ʼ������ͨѶ���� */
	LPC_UART1->LCR 	 = 0x83;							// ����Ϊ8λ����λ��1λֹͣλ������żУ�飬DLAB=1, �������ò�����
	Fdiv 	 = (Fpclk / 16) / Baudrate;			// ���ò�����
	LPC_UART1->DLM 	 = Fdiv / 256;
	LPC_UART1->DLL 	 = Fdiv % 256;
	LPC_UART1->LCR 	 = 0x03;							// ��DLABλΪ0
	
	/* ��λ����FIFO */
	LPC_UART1->FCR 	|= 0xC7;					
	
	/* С����Ƶ������ */
	LPC_UART1->FDR   &= ~0x0F;						// ��DIVADDVAL=0					        
	LPC_UART1->FDR   |=  0x10;						// MULVAL����Ϊ0					        

	/* ��ʼ��LPC_UART0->IER */
	LPC_UART1->IER 	 = 0;
  LPC_UART1->RS485CTRL = 1;
//  LPC_UART1->RS485CTRL = (1 << 4) | (
//	rs485cfg.AutoDirCtrl_State = ENABLE;
//	rs485cfg.DirCtrlPin = UART1_RS485_DIRCTRL_DTR;
//	rs485cfg.DirCtrlPol_Level = SET;
//	rs485cfg.DelayValue = 50;
//	rs485cfg.NormalMultiDropMode_State = ENABLE;
//	rs485cfg.AutoAddrDetect_State = ENABLE;
//	rs485cfg.MatchAddrValue = SLAVE_ADDR;
//	rs485cfg.Rx_State = DISABLE;
	
  InstallIRQ(UART1_IRQn, (DWORD)IRQ_UART1);	
	NVIC_EnableIRQ(UART1_IRQn);
	
  LPC_UART1->IER 	|= 0x03;							// ʹ��THRE�ж�
}
#endif

void UartInit(void)
{
#if UART0_SETUP
	memset(&g_Uart0, 0, sizeof(UARTDEF));
	g_Uart0.ReadBuf 	= Uart0ReadBuf;
	g_Uart0.SendBuf 	= Uart0SendBuf;
	g_Uart0.SendByte 	= Uart0SendByte;
	g_Uart0.HasData		= Uart0HasData;
	g_Uart0.ReadByte	= Uart0ReadByte;
	Uart0Init(UART0_BAUDRATE);
#endif
#if UART1_SETUP
	memset(&g_Uart1, 0, sizeof(UARTDEF));
	g_Uart1.ReadBuf 	= Uart1ReadBuf;
	g_Uart1.SendBuf 	= Uart1SendBuf;
	g_Uart1.SendByte 	= Uart1SendByte;
	g_Uart1.HasData		= Uart1HasData;
	g_Uart1.ReadByte	= Uart1ReadByte;
  g_Uart1.ClearBuf  = Uart1ClearBuf;
	Uart1Init(UART1_BAUDRATE);
#endif
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
  Uart0SendByte(ch);

  return ch;
} 

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

