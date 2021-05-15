#include "Sys.h"
/*����CCLKֵ��С����λΪKHz*/
#define  IAP_FCCLK            (48000)
#define  IAP_ENTER_ADR        0x1FFF1FF1                                  /* IAP��ڵ�ַ����              */
#define	 IAP_SELECTOR		  		50		//ѡ������		����ʼ�����š����������š�
#define	 IAP_ERASESECTOR			52		//��������		����ʼ�����š����������š�ϵͳʱ��Ƶ�ʡ�
#define  IAP_READSN        		58
/*
 *  ���庯��ָ��  
 */
extern DWORD  VERSION;
extern DWORD  ID_0;
extern DWORD  ID_1;
extern DWORD  ID_2;
extern DWORD  ID_3;
extern DWORD  ID_4;
DWORD	paramin[8];		//IAP��ڲ���������
DWORD	paramout[8];	//IAP���ڲ���������
/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        ������գ��������54
** input parameters:    ��
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
DWORD ReadUSN(DWORD sn[4])
{  
  paramin[0] = IAP_READSN;                                        /* ����������                   */
  (*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									/* ����IAP�������              */
  sn[0] = paramout[1];
  sn[1] = paramout[2];
  sn[2] = paramout[3];
  sn[3] = paramout[4];
  return (paramout[0]);																						/* ����״̬��                   */
}
/*********************************************************************************************************
**�������ƣ�SelSector()
**�������ܣ�IAP����������ѡ�񣬴���Ϊ50
**��ڲ�����sec1	��ʼ����
**			sec2	��ֹ����
**���ڲ�����IAP ����״̬��
**			IAP����ֵ��paramout��������
********************************************************************************************************/
DWORD	SelSector(BYTE	sec1, BYTE	sec2)
{
	paramin[0] = IAP_SELECTOR;																			//����������
	paramin[1] = sec1;																							//���ò���
	paramin[2] = sec2;
	(*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									//����IAP�������
	return(paramout[0]);																						//����״̬��
}
/*********************************************************************************************************
**�������ƣ�EraseSector()
**�������ܣ������������������52
**��ڲ�����sec1	��ʼ����
**			sec2	��ֹ����
**���ڲ�����IAP ����״̬��
**			IAP����ֵ��paramout��������
********************************************************************************************************/
DWORD	EraseSector(DWORD	sec1, DWORD	sec2)
{
	paramin[0] = IAP_ERASESECTOR;																		//����������
	paramin[1] = sec1;																							//���ò���
	paramin[2] = sec2;
	paramin[3] = IAP_FCCLK;
	(*(void(*)())IAP_ENTER_ADR)(paramin, paramout);									//����IAP�������
	return(paramout[0]);																						//����״̬��
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
	SelSector(0, 21);			  // ѡ������     
	EraseSector(0, 21);			// ��������
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
    EraseClip();                 // ������
    return FALSE;
   }
	return TRUE;
}
