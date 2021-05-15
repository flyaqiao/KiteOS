/*****************************************************************************
 *   uart.h:  Header file for NXP LPC23xx Family Microprocessors
 *
 *   Copyright(C) 2006, 凯特科技 
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __UART_H 
#define __UART_H	 		 
#include "string.h"	
/*  
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------  
*/ 

/*--------------------- UART Configuration ----------------------  
//  
// <E> UART0 Configuration  
//   <O1.0..31>  Baudrate  
//               <19200=> 19200 
//               <57600=> 57600 
//               <115200=> 115200
// </E>  
//  
// <E2> UART1 Configuration  
//   <O3.0..31>  Baudrate  
//               <9600=>  9600
//               <19200=> 19200 
//               <57600=> 57600 
//               <115200=> 115200
//   <O4.0>      Use 485
// </E>
// <O5.0..7> Uart Buffer Size 
//               <2-256:4>  
*/   
#define UART0_SETUP           	1
#define UART0_BAUDRATE          0x0001C200  
#define UART1_SETUP           	0
#define UART1_BAUDRATE          0x00002580 
#define UART1_485               1
#define UARTBUF_SIZE						200
/*  
//-------- <<< end of configuration section >>> ------------------------------  
*/
#define IER_RBR                 0x01
#define IER_THRE                0x02
#define IER_RLS                 0x04

#define IIR_PEND                0x01
#define IIR_RLS                 0x03
#define IIR_RDA                 0x04
#define IIR_CTI                 0x0C
#define IIR_THRE                0x02

#define LSR_RDR                 0x01
#define LSR_OE                  0x02
#define LSR_PE                  0x04
#define LSR_FE                  0x08
#define LSR_BI                  0x10
#define LSR_THRE                0x20
#define LSR_TEMT                0x40
#define LSR_RXFE                0x80


typedef BYTE (*UartBufHandle)(BYTE *pBuf, BYTE count);
typedef void (*UartSendByteHandle)(BYTE value);
typedef BYTE (*UartReadByteHandle)(void);

typedef struct UartBufDef
{			
	BYTE  							Buf[UARTBUF_SIZE];
	BYTE  							Head;
	BYTE  							Rear;
	BYTE  							Count;
}	UARTBUFDEF, *PUARTBUFDEF;

typedef struct UartDef
{
//	BYTE  							RevBuf[UARTBUF_SIZE];
//	BYTE  							RevHead;
//	BYTE  							RevRear;
//	BYTE  							RevCount;
	UARTBUFDEF					RecvBuffer;
	UARTBUFDEF					SendBuffer;
	UartBufHandle				ReadBuf;
	UartBufHandle				SendBuf;
	UartSendByteHandle 	SendByte;
	UartReadByteHandle 	ReadByte;
	UartReadByteHandle	HasData;
  UartReadByteHandle  ClearBuf;
}	UARTDEF, *PUARTDEF;
#if UART0_SETUP
extern UARTDEF g_Uart0;
#endif
#if UART1_SETUP
extern UARTDEF g_Uart1;
#endif

void UartInit(void);
/*********************************************************************************************************
** 函数名称 ：UART0_SendByte
** 函数功能 ：以查询方式发送一字节数据
** 入口参数 ：dat	要发送的数据
** 出口参数 ：无
*********************************************************************************************************/

__inline void SendString(PUARTDEF pUart, char *str)
{
	pUart->SendBuf((BYTE*)str, strlen(str));
}

__inline void SendHex(PUARTDEF pUart, BYTE* pBuf, BYTE size)
{
	BYTE i = 0;
	BYTE HexDef[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	for (i = 0; i < size; i++)
	{
		pUart->SendByte(HexDef[pBuf[i] >> 4]);
		pUart->SendByte(HexDef[pBuf[i] & 0x0F]);
	}
	pUart->SendBuf("\r\n", 2);
}
__inline void SendInt(PUARTDEF pUart, DWORD value)
{
	char buf[64];							 		 
	sprintf(buf, "%10d\r\n", value);
  SendString(pUart, buf);	
}
#ifdef _DEBUG	 
	#if	UART0_SETUP
		#define DebugString(str)				SendString(&g_Uart0, str)
		#define DebugHexStr(pBuf, size)	SendHex(&g_Uart0, (BYTE*)pBuf, size)
	#elif UART1_SETUP
		#define DebugString(str)				SendString(&g_Uart1, str)
		#define DebugHexStr(pBuf, size)	SendHex(&g_Uart1, (BYTE*)pBuf, size)
	#else	 
		#define DebugString(str)
		#define DebugHexStr(pBuf, size)
	#endif
#else
	#define DebugString(str)
	#define DebugHexStr(pBuf, size)
#endif
#if	UART0_SETUP
	#define TraceString(str)				SendString(&g_Uart0, str)
  #define TraceInt(val)				    SendInt(&g_Uart0, val)
#elif UART1_SETUP
	#define TraceString(str)				SendString(&g_Uart1, str)  
  #define TraceInt(val)				    SendInt(&g_Uart1, val)
#else	 
	#define TraceString(str)     
  #define TraceInt(val)				    
#endif

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
