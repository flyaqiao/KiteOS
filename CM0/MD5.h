/* 
* Copyright (c) 2008 - 2009, ʢ�ǿƼ� 
* All rights reserved. 
*  
* �ļ����ƣ�$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/KnitSystem/MD5.h $ 
* ժ    Ҫ��
*  
* ��ǰ�汾��$Revision: 1861 $
* ����޸ģ�$Author: flyaqiao $
* �޸����ڣ�$Date:: 2011-06-11 15:52:55#$
*
*/ 
#ifndef _MD5_H_
#define _MD5_H_


int MD5String(const char* inStr, char* outStr, int outLen);
int MD5Calc(const LPBYTE inBuf, int inLen, DWORD out[4]);

#endif //_MD5_H_
