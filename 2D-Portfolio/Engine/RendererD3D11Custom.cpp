#include "StdAfx.h"
#include "RendererD3D11Custom.h"
#include "ImageLoader.h"

#include <dxgi.h>
#include <wincodec.h>

#include "ScreenGrab.h"

// 빌드 때 Shaders/*.hlsl 을 컴파일한 바이트코드(g_SpriteVS·g_SpritePS 배열). 셰이더 내용만 고치면 다시 만들어진다
#include "SpriteVS.h"
#include "SpritePS.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

CRendererD3D11Custom::CRendererD3D11Custom(void)
{
}

CRendererD3D11Custom::~CRendererD3D11Custom(void)
{
}

// TODO(1장): 플립 모델 스왑체인과 장치를 만들고, 백버퍼로 렌더 타깃 뷰를 만든다
// TODO(2장~7장): 셰이더·입력 레이아웃·버퍼·상태 객체를 만든다
HRESULT CRendererD3D11Custom::Initialize(const HWND /*hWnd*/, const WINMODE /*Mode*/)
{
	return S_OK;
}

// TODO(1장): 렌더 타깃을 연결하고 뷰포트를 정한 뒤 ClearColor 로 지운다
void CRendererD3D11Custom::BeginFrame(const D3DCOLOR /*ClearColor*/)
{
}

// TODO(2장): 첫 삼각형을 여기서 그려 본다(장 끝에서 지운다)
// TODO(7장): 모아 둔 스프라이트를 마저 그린다
void CRendererD3D11Custom::EndFrame(void)
{
}

// TODO(1장): 스왑체인으로 화면에 내보낸다
void CRendererD3D11Custom::Present(const HWND /*hWnd*/)
{
}

// 뼈대는 크기만 채운다 — 게임 코드가 텍스처 크기로 위치를 계산하므로 이것이 있어야 로직이 돈다.
// TODO(4장): 픽셀로 Texture2D 와 SRV 를 만들어 CTextureD3D11Custom 에 담는다
HRESULT CRendererD3D11Custom::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
{
	UINT uWidth = 0;
	UINT uHeight = 0;
	vector<BYTE> vecPixels;
	if (!LoadImagePixels(pPath, ColorKey, uWidth, uHeight, vecPixels))
	{
		return E_FAIL;
	}
	rTexInfo.pTexture = new CTextureD3D11Custom;
	rTexInfo.ImgInfo.Width = uWidth;
	rTexInfo.ImgInfo.Height = uHeight;
	return S_OK;
}

// TODO(3장): 스프라이트마다 사각형 하나 — 이동만 반영
// TODO(4장~6장): 텍스처·색·월드 행렬·원본 사각형을 반영
// TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
void CRendererD3D11Custom::DrawSprite(const TEXINFO* /*pTexInfo*/, const RECT* /*pSrcRect*/,
	const D3DXVECTOR3& /*vCenter*/, const D3DXMATRIX& /*matWorld*/, const D3DCOLOR /*Color*/)
{
}

// 캡처(비교 도구용) — 학습 범위 밖이라 미리 채워 둔다. 1장에서 스왑체인을 만들면 동작한다
bool CRendererD3D11Custom::SaveBackBuffer(const TCHAR* pPath)
{
	if (m_pSwapChain == nullptr)
	{
		return false;
	}
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()))))
	{
		return false;
	}
	return SUCCEEDED(DirectX::SaveWICTextureToFile(m_pContext.Get(), pBackBuffer.Get(), GUID_ContainerFormatPng, pPath));
}
