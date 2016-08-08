#pragma once

#include "Defines.h"

class CTexture;				
class CTextureMgr			
{							
	SINGLETON(CTextureMgr);	
public:
	// Texture 관리 데이터
	map<const TCHAR*, CTexture*> m_MapTexture;
public:
	const TEXINFO* GetTexture(const TCHAR* pObjKey, int iCnt = 0);
public:
	HRESULT InsertTexture(const TCHAR* pFileName
		, const TCHAR* pObjKey, TEXTYPE TypeID, int iCnt = 1);
	void Release(void);
private:					
	CTextureMgr(void);		
public:						
	~CTextureMgr(void);		
};							
							
