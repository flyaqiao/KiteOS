/* 
* Copyright (c) 2008 - 2009, 盛星科技 
* All rights reserved. 
*  
* 文件名称：$HeadURL: https://codeserver/svn/KnitSystem/NewSystem/KnitSystem/MD5.h $ 
* 摘    要：
*  
* 当前版本：$Revision: 1861 $
* 最后修改：$Author: flyaqiao $
* 修改日期：$Date:: 2011-06-11 15:52:55#$
*
*/ 
#ifndef _MD5_H_
#define _MD5_H_


int MD5String(const char* inStr, char* outStr, int outLen);
int MD5Calc(const LPBYTE inBuf, int inLen, DWORD out[4]);

#endif //_MD5_H_
