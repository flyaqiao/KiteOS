#include "Sys.h"
/*定义CCLK值大小，单位为KHz*/
#define  IAP_FCCLK            (48000)
#define  IAP_ENTER_ADR        0x1FFF1FF1                                  /* IAP入口地址定义              */
#define	 IAP_SELECTOR		  		50		//选择扇区		【起始扇区号、结束扇区号】
#define	 IAP_ERASESECTOR			52		//擦除扇区		【起始扇区号、结束扇区号、系统时钟频率】
#define  IAP_READSN        		58
/*
 *  定义函数指针  
 */
extern DWORD  VERSION;
extern DWORD  ID_0;
extern DWORD  ID_1;
extern DWORD  ID_2;
extern DWORD  ID_3;
extern DWORD  ID_4;
DWORD	paramin[8];		//IAP入口参数缓冲区
DWORD	paramout[8];	//IAP出口参数缓冲区
/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        扇区查空，命令代码54
** input parameters:    无
** output parameters:   paramout[0]:    IAP操作状态码,IAP返回值
** Returned value:      paramout[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
DWORD ReadUSN(DWORD sn[4])
{  
  paramin[0] = IAP_READSN;                                        /* 设置命令字                   */
  (*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									/* 调用IAP服务程序              */
  sn[0] = paramout[1];
  sn[1] = paramout[2];
  sn[2] = paramout[3];
  sn[3] = paramout[4];
  return (paramout[0]);																						/* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称：SelSector()
**函数功能：IAP操作缓冲区选择，代码为50
**入口参数：sec1	起始扇区
**			sec2	终止扇区
**出口参数：IAP 操作状态码
**			IAP返回值（paramout缓冲区）
********************************************************************************************************/
DWORD	SelSector(BYTE	sec1, BYTE	sec2)
{
	paramin[0] = IAP_SELECTOR;																			//设置命令字
	paramin[1] = sec1;																							//设置参数
	paramin[2] = sec2;
	(*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									//调用IAP服务程序
	return(paramout[0]);																						//返回状态码
}
/*********************************************************************************************************
**函数名称：EraseSector()
**函数功能：擦除扇区，命令代码52
**入口参数：sec1	起始扇区
**			sec2	终止扇区
**出口参数：IAP 操作状态码
**			IAP返回值（paramout缓冲区）
********************************************************************************************************/
DWORD	EraseSector(DWORD	sec1, DWORD	sec2)
{
	paramin[0] = IAP_ERASESECTOR;																		//设置命令字
	paramin[1] = sec1;																							//设置参数
	paramin[2] = sec2;
	paramin[3] = IAP_FCCLK;
	(*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									//调用IAP服务程序
	return(paramout[0]);																						//返回状态码
}
DWORD Encrypt(DWORD src, DWORD key)
{
  BYTE* bstr = (BYTE*)&src;
  DWORD des = (bstr[2] << 24) | (bstr[0] << 16) | (bstr[1] << 8) | bstr[3];
  des = (des >> key) | (des << key);
  return des;
}
void EraseClip(void)
{
  OsEnterCritical();
	SelSector(0, 21);			  // 选择扇区     
	EraseSector(0, 21);			// 擦除扇区
  OsExitCritical();
}
BOOL CheckUSN(void)
{
	DWORD sn[4];
  ReadUSN(sn);
  if (Encrypt(sn[0], 3) != ID_1 
    || Encrypt(sn[1], 5) != ID_2  
    || Encrypt(sn[2], 7) != ID_3
    || Encrypt(sn[3], 9) != ID_4)
   {
    EraseClip();                 // 自我销
    return FALSE;
   }
	return TRUE;
}
