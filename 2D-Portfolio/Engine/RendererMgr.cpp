#include "StdAfx.h"
#include "RendererMgr.h"

CRenderer::CRenderer(void)
	: m_pBackend(nullptr)
{
}

CRenderer::~CRenderer(void)
{
	Release();
}

HRESULT CRenderer::Initialize(IRenderer* pBackend, const HWND hWnd, const WINMODE Mode)
{
	Release();
	m_pBackend = pBackend;
	if (m_pBackend == nullptr)
	{
		return E_FAIL;
	}
	return m_pBackend->Initialize(hWnd, Mode);
}

void CRenderer::BeginFrame(const D3DCOLOR ClearColor)
{
	if (m_pBackend != nullptr)
	{
		m_pBackend->BeginFrame(ClearColor);
	}
}

void CRenderer::EndFrame(const HWND hWnd)
{
	if (m_pBackend == nullptr)
	{
		return;
	}
	m_pBackend->EndFrame();
	// 백버퍼 내용은 Present 뒤에는 보장되지 않으므로 그 전에 저장한다
	if (!m_strCapturePath.empty())
	{
		const bool bSaved = m_pBackend->SaveBackBuffer(m_strCapturePath.c_str());
		SmokeCheck(bSaved, L"capture %s", m_strCapturePath.c_str());
		m_strCapturePath.clear();
	}
	m_pBackend->Present(hWnd);
}

HRESULT CRenderer::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
{
	if (m_pBackend == nullptr)
	{
		return E_FAIL;
	}
	return m_pBackend->CreateTexture(pPath, ColorKey, rTexInfo);
}

void CRenderer::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
{
	if (m_pBackend != nullptr)
	{
		m_pBackend->DrawSprite(pTexInfo, pSrcRect, vCenter, matWorld, Color);
	}
}

void CRenderer::Release(void)
{
	SAFE_DELETE<IRenderer>(&m_pBackend);
}
