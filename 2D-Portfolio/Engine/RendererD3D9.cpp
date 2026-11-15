#include "StdAfx.h"
#include "RendererD3D9.h"


CRendererD3D9::CRendererD3D9(void)
	: m_p3D(nullptr)
	, m_pDevice(nullptr)
	, m_pSprite(nullptr)
{}

CRendererD3D9::~CRendererD3D9(void)
{ Release(); }

HRESULT CRendererD3D9::Initialize(const HWND hWnd, const WINMODE Mode)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 장치의 성능을 조사한다.
	D3DCAPS9 devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	// 장치 성능조사
	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(hWnd, L"장치정보얻어오기 실패");
		return E_FAIL;
	}
	DWORD BehaviorFlag = 0;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		BehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		BehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WINSIZEX;
	d3dpp.BackBufferHeight = WINSIZEY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = true;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = Mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치 생성
	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd, BehaviorFlag, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(hWnd, L"장치생성 실패");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		return E_FAIL;

	return S_OK;
}

void CRendererD3D9::BeginFrame(const D3DCOLOR ClearColor)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET
		| D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, ClearColor, 1.0f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CRendererD3D9::EndFrame(void)
{
	m_pSprite->End();
	m_pDevice->EndScene();
}

void CRendererD3D9::Present(const HWND hWnd)
{
	m_pDevice->Present(NULL, NULL, hWnd, NULL);
}

// 원본 크기 그대로, 밉맵·형식은 파일 정보대로 만든다(기존 CSingleTexture·CMultiTexture 의 인자 그대로)
HRESULT CRendererD3D9::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
{
	D3DXIMAGE_INFO tImgInfo = {};
	if (FAILED(D3DXGetImageInfoFromFile(pPath, &tImgInfo)))
	{
		return E_FAIL;
	}
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
		pPath, tImgInfo.Width, tImgInfo.Height, tImgInfo.MipLevels,
		0, tImgInfo.Format, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT,
		ColorKey,
		&tImgInfo, nullptr,
		&pTexture)))
	{
		return E_FAIL;
	}
	rTexInfo.pTexture = new CTextureD3D9(pTexture);
	rTexInfo.ImgInfo.Width = tImgInfo.Width;
	rTexInfo.ImgInfo.Height = tImgInfo.Height;
	return S_OK;
}

void CRendererD3D9::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
{
	if (pTexInfo == nullptr || pTexInfo->pTexture == nullptr)
	{
		return;
	}
	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(static_cast<const CTextureD3D9*>(pTexInfo->pTexture)->m_pTexture,
		pSrcRect, &vCenter, nullptr, Color);
}

// 백버퍼 → 시스템 메모리 표면으로 복사 → PNG. 백버퍼는 직접 잠글 수 없어 복사본을 저장한다
bool CRendererD3D9::SaveBackBuffer(const TCHAR* pPath)
{
	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	if (FAILED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
	{
		return false;
	}
	D3DSURFACE_DESC tDesc = {};
	pBackBuffer->GetDesc(&tDesc);

	bool bSaved = false;
	LPDIRECT3DSURFACE9 pSysMem = nullptr;
	if (SUCCEEDED(m_pDevice->CreateOffscreenPlainSurface(tDesc.Width, tDesc.Height, tDesc.Format,
		D3DPOOL_SYSTEMMEM, &pSysMem, nullptr)))
	{
		bSaved = SUCCEEDED(m_pDevice->GetRenderTargetData(pBackBuffer, pSysMem))
			&& SUCCEEDED(D3DXSaveSurfaceToFile(pPath, D3DXIFF_PNG, pSysMem, nullptr, nullptr));
		pSysMem->Release();
	}
	pBackBuffer->Release();
	return bSaved;
}

void CRendererD3D9::Release()
{
	if (m_pSprite != nullptr)
	{
		m_pSprite->Release();
		m_pSprite = nullptr;
	}
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	if (m_p3D != nullptr)
	{
		m_p3D->Release();
		m_p3D = nullptr;
	}
}
