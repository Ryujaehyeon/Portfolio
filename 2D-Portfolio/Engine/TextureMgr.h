#pragma once

#include "Defines.h"

class CTexture;				
class CTextureMgr			
{							
	SINGLETON(CTextureMgr);	
public:
	// Texture 관리 데이터
	map<wstring, CTexture*> m_MapTexture;
private:
	// 여러 장 텍스처를 불러올 때 투명하게 할 색 — 앱이 정한다(기본 검정, 툴은 흰색)
	D3DCOLOR m_MultiColorKey;
public:
	void SetMultiColorKey(const D3DCOLOR ColorKey) { m_MultiColorKey = ColorKey; }
public:
	const TEXINFO* GetTexture(const TCHAR* pObjKey,
		const TCHAR* pStateKey = 0, int iCnt = 0);
public:
	HRESULT InsertTexture(const TCHAR* pFileName
		, const TCHAR* pObjKey, TEXTYPE TypeID,
		const TCHAR* pStageKey = 0,int iCnt = 1);
	void Release(void);
private:					
	CTextureMgr(void);		
public:						
	~CTextureMgr(void);		
};							
							
