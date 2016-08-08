#pragma once

#include "Texture.h"

class CSingleTexture
	: public CTexture
{
private:
	TEXINFO* m_pTexInfo;
public:
	virtual const TEXINFO* GetTexture(int iCnt);
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName, int iCnt);
	virtual void    Release(void);
public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};

