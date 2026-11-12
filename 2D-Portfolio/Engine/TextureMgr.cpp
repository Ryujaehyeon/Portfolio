#include "StdAfx.h"
#include "TextureMgr.h"

#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

CTextureMgr::CTextureMgr(void)
	: m_MultiColorKey(D3DCOLOR_ARGB(255, 0, 0, 0))
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

HRESULT CTextureMgr::InsertTexture(const TCHAR* pFileName, const TCHAR* pObjKey, 
	TEXTYPE TypeID, const TCHAR* pStateKey, int iCnt)
{
	map<wstring, CTexture*>::iterator iter	= m_MapTexture.find(pObjKey);
	if(iter != m_MapTexture.end())
	{
		if(FAILED((iter->second)->InsertTexture(pFileName, pStateKey, iCnt)))
			return E_FAIL;
	}
	else
	{
		CTexture* pTexture = NULL;
		switch(TypeID)
		{
		case TEXTYPE_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEXTYPE_MULTI:
			pTexture = new CMultiTexture(m_MultiColorKey);
			break;
		}
		if(pTexture == NULL)
			return E_FAIL;
		if(FAILED(pTexture->InsertTexture(pFileName, pStateKey, iCnt)))
			return E_FAIL;
		m_MapTexture.insert(make_pair(pObjKey, pTexture));
	}
	return S_OK;
}

void CTextureMgr::Release()
{
	for(map<wstring, CTexture*>::iterator iter 
		= m_MapTexture.begin();
		iter != m_MapTexture.end(); ++iter)
	{
		SAFE_DELETE<CTexture>(&iter->second);
	}
	m_MapTexture.clear();
}

const TEXINFO* CTextureMgr::GetTexture(const TCHAR* pObjKey,
	const TCHAR* pStateKey, int iCnt)
{
	map<wstring, CTexture*>::iterator iter
		= m_MapTexture.find(pObjKey);
	if(iter == m_MapTexture.end())
		return NULL;
	return iter->second->GetTexture(pStateKey, iCnt);
}