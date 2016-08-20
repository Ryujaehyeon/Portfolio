#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{}

CMultiTexture::~CMultiTexture(void)
{}

const TEXINFO* CMultiTexture::GetTexture(
	const TCHAR* pStateKey, int iCnt)
{	
	map<const TCHAR*, vector<TEXINFO*>>::iterator iter = m_MapTexture.find(pStateKey);
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

		ZeroMemory(m_pTexInfo, sizeof(TEXINFO));
		if(FAILED(D3DXGetImageInfoFromFile(szPath
			,&m_pTexInfo->ImgInfo)))
		{
			return E_FAIL;
		}
		if(FAILED(D3DXCreateTextureFromFileEx(
			GET_SINGLE(CDevice)->GetDevice(),
			szPath, m_pTexInfo->ImgInfo.Width,
			m_pTexInfo->ImgInfo.Height, m_pTexInfo->ImgInfo.MipLevels,
			0, m_pTexInfo->ImgInfo.Format, D3DPOOL_MANAGED, 
			D3DX_DEFAULT, D3DX_DEFAULT,
			D3DCOLOR_ARGB(255, 0, 0, 0),
			&m_pTexInfo->ImgInfo, NULL,
			&m_pTexInfo->pTexture)))
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
	for (map<const TCHAR*, vector<TEXINFO*>>::iterator iter = m_MapTexture.begin();
		iter != m_MapTexture.end(); ++iter)
	{
		for(vector<TEXINFO*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			// 로드된 데이터를 해제
			(*iter2)->pTexture->Release();
			SAFE_DELETE<TEXINFO>(&(*iter2));
		}
		iter->second.clear();
	}
	m_MapTexture.clear();
}