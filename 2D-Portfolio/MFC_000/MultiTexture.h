#pragma once

#include "Texture.h"

class CMultiTexture
	: public CTexture
{
private:
	vector<TEXINFO*> m_vecTexture;
public:
	virtual const TEXINFO* GetTexture(int iCnt);
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName, int iCnt);
	virtual void    Release(void);
public:
	CMultiTexture(void);
	virtual ~CMultiTexture(void);
};

