#pragma once

#include "Texture.h"

class CSingleTexture
	: public CTexture
{
private:
	TEXINFO* m_pTexInfo;
public:
	virtual const TEXINFO* GetTexture(
		const TCHAR* pStateKey = 0,int iCnt = 0);
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName,
		const TCHAR* pStateKey = 0, int iCnt = 0);
	virtual void    Release(void);
public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};

