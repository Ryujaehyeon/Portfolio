#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{}

CMultiTexture::~CMultiTexture(void)
{}

const TEXINFO* CMultiTexture::GetTexture(int iCnt)
{ return m_vecTexture[iCnt]; }

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFileName, int iCnt)
{
	TCHAR szPath[256];
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
			D3DCOLOR_ARGB(255, 255, 255, 255),
			&m_pTexInfo->ImgInfo, NULL,
			&m_pTexInfo->pTexture)))
		{
			return E_FAIL;
		}

		m_vecTexture.push_back(m_pTexInfo);
	}
	return S_OK;
}

void CMultiTexture::Release()
{
	for(vector<TEXINFO*>::iterator iter = m_vecTexture.begin();
		iter != m_vecTexture.end(); ++iter)
	{
		SAFE_DELETE<TEXINFO>(&(*iter));
	}
	m_vecTexture.clear();
}