#pragma once

#include "Texture.h"

class CMultiTexture
	: public CTexture
{
private:
	map<wstring, vector<TEXINFO*>> m_MapTexture;
	// 불러올 때 투명하게 할 색 — 앱마다 다르다(Client 검정, 툴 흰색)
	D3DCOLOR m_ColorKey;
public:
	virtual const TEXINFO* GetTexture(
		const TCHAR* pStateKey = 0, int iCnt = 0);
public:
	virtual HRESULT InsertTexture(const TCHAR* pFileName,
		const TCHAR* pStateKey = 0, int iCnt = 0);
	virtual void    Release(void);
public:
	explicit CMultiTexture(const D3DCOLOR ColorKey);
	virtual ~CMultiTexture(void);
};

