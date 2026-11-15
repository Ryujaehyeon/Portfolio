#include "StdAfx.h"
#include "SingleTexture.h"
#include "RendererMgr.h"

CSingleTexture::CSingleTexture(void)
	:m_pTexInfo(0)
{}

CSingleTexture::~CSingleTexture(void)
{ Release(); }

const TEXINFO* CSingleTexture::GetTexture(
	const TCHAR* pStateKey,	int iCnt)
{ return m_pTexInfo; }

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFileName,
	const TCHAR* pStateKey, int iCnt)
{
	if(m_pTexInfo != NULL)
		return E_FAIL;
	m_pTexInfo = new TEXINFO;
	// 단장 텍스처는 흰색을 투명하게 한다
	if(FAILED(GET_SINGLE(CRenderer)->CreateTexture(pFileName,
		D3DCOLOR_ARGB(255, 255, 255, 255), *m_pTexInfo)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	if (m_pTexInfo == nullptr)
	{
		return;
	}
	SAFE_DELETE<IRenderTexture>(&m_pTexInfo->pTexture);
	SAFE_DELETE<TEXINFO>(&m_pTexInfo);
}