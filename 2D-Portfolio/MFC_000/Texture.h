#pragma once

#include "Defines.h"

class CTexture
{
public:
	virtual const TEXINFO* GetTexture(int iCnt)PURE;
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName, int iCnt)PURE;
	virtual void    Release(void)PURE;
public:
	CTexture(void);
	virtual ~CTexture(void);
};

