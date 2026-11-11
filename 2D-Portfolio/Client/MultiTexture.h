#pragma once

#include "Texture.h"

class CMultiTexture
	: public CTexture
{
private:
	map<const TCHAR*, vector<TEXINFO*>> m_MapTexture;
public:
	virtual const TEXINFO* GetTexture(
		const TCHAR* pStateKey = 0, int iCnt = 0);
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName,
		const TCHAR* pStateKey = 0, int iCnt = 0);
	virtual void    Release(void);
public:
	CMultiTexture(void);
	virtual ~CMultiTexture(void);
};

