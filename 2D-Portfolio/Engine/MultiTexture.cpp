#include "StdAfx.h"
#include "MultiTexture.h"
#include "RendererMgr.h"

CMultiTexture::CMultiTexture(const D3DCOLOR ColorKey)
	: m_ColorKey(ColorKey)
{}

CMultiTexture::~CMultiTexture(void)
{}

const TEXINFO* CMultiTexture::GetTexture(
	const TCHAR* pStateKey, int iCnt)
{	
	map<wstring, vector<TEXINFO*>>::iterator iter = m_MapTexture.find(pStateKey);
	if (iter == m_MapTexture.end())
		return NULL;

	return iter->second[iCnt]; 
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFileName, 
	const TCHAR* pStateKey, int iCnt)
{
	TCHAR szPath[256];

	vector<TEXINFO*> vecTexture;

	for(int i = 0; i < iCnt; ++i)
	{
		TEXINFO* m_pTexInfo = new TEXINFO;

		ZeroMemory(szPath, sizeof(szPath));
		wsprintf(szPath, pFileName, i);

		if(FAILED(GET_SINGLE(CRenderer)->CreateTexture(szPath, m_ColorKey, *m_pTexInfo)))
		{
			return E_FAIL;
		}

		vecTexture.push_back(m_pTexInfo);
	}
	m_MapTexture.insert(make_pair(pStateKey, vecTexture));
	return S_OK;
}

void CMultiTexture::Release()
{
	for (map<wstring, vector<TEXINFO*>>::iterator iter = m_MapTexture.begin();
		iter != m_MapTexture.end(); ++iter)
	{
		for(vector<TEXINFO*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			// 로드된 데이터를 해제
			SAFE_DELETE<IRenderTexture>(&(*iter2)->pTexture);
			SAFE_DELETE<TEXINFO>(&(*iter2));
		}
		iter->second.clear();
	}
	m_MapTexture.clear();
}