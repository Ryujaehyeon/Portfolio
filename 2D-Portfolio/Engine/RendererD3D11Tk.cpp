#include "StdAfx.h"
#include "RendererD3D11Tk.h"
#include "ImageLoader.h"

#include <dxgi.h>
#include <wincodec.h>

#include "ScreenGrab.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;

namespace
{
	// D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA
	XMVECTOR ToColorVector(const D3DCOLOR Color)
	{
		return XMVectorSet(((Color >> 16) & 0xFF) / 255.f, ((Color >> 8) & 0xFF) / 255.f,
			(Color & 0xFF) / 255.f, ((Color >> 24) & 0xFF) / 255.f);
	}
}

CRendererD3D11Tk::CRendererD3D11Tk(void)
{
}

CRendererD3D11Tk::~CRendererD3D11Tk(void)
{
}

HRESULT CRendererD3D11Tk::Initialize(const HWND hWnd, const WINMODE Mode)
{
	if (Mode != WINMODE_WIN)
	{
		ERR_MSG(hWnd, L"D3D11 백엔드는 창 모드만 지원합니다");
		return E_FAIL;
	}

	// 플립 모델 스왑체인 — 버퍼 2개, 지금의 D3D9 백버퍼(A8R8G8B8)와 같은 메모리 배치
	DXGI_SWAP_CHAIN_DESC tDesc = {};
	tDesc.BufferDesc.Width = WINSIZEX;
	tDesc.BufferDesc.Height = WINSIZEY;
	tDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferCount = 2;
	tDesc.OutputWindow = hWnd;
	tDesc.Windowed = TRUE;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// 스프라이트에는 10_0 기능으로 충분하다 — 오래된 장치에서도 돌게 아래 수준까지 연다
	const D3D_FEATURE_LEVEL arrLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	UINT uFlags = 0;
#ifdef _DEBUG
	uFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uFlags,
		arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tDesc,
		m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
	if (FAILED(hr) && (uFlags & D3D11_CREATE_DEVICE_DEBUG) != 0)
	{
		// 디버그 레이어(그래픽 도구)가 설치돼 있지 않으면 실패한다 — 없이 다시 만든다
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tDesc,
			m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
	}
	if (FAILED(hr))
	{
		ERR_MSG(hWnd, L"D3D11 장치 생성 실패(0x%08X)", static_cast<unsigned int>(hr));
		return E_FAIL;
	}

	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf())))
		|| FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRTV.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 렌더 타깃 생성 실패");
		return E_FAIL;
	}

	m_pSpriteBatch = std::make_unique<SpriteBatch>(m_pContext.Get());
	m_pStates = std::make_unique<CommonStates>(m_pDevice.Get());
	return S_OK;
}

void CRendererD3D11Tk::BeginFrame(const D3DCOLOR ClearColor)
{
	// 플립 모델은 Present 뒤에 렌더 타깃 연결이 풀리므로 프레임마다 다시 연결한다
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), nullptr);
	const D3D11_VIEWPORT tViewport = { 0.f, 0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 1.f };
	m_pContext->RSSetViewports(1, &tViewport);

	XMFLOAT4 vColor;
	XMStoreFloat4(&vColor, ToColorVector(ClearColor));
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), &vColor.x);

	BeginSpriteBatch();
}

// D3DXSPRITE_ALPHABLEND 와 같게 — 제출 순서대로(Deferred), 비 premultiplied 알파 블렌드
void CRendererD3D11Tk::BeginSpriteBatch(void)
{
	m_pSpriteBatch->Begin(SpriteSortMode_Deferred, m_pStates->NonPremultiplied());
}

void CRendererD3D11Tk::EndFrame(void)
{
	m_pSpriteBatch->End();
}

void CRendererD3D11Tk::Present(const HWND /*hWnd*/)
{
	// 스왑체인이 그릴 창을 이미 안다. 지금처럼 수직 동기 없이 내보낸다
	m_pSwapChain->Present(0, 0);
}

HRESULT CRendererD3D11Tk::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
{
	UINT uWidth = 0;
	UINT uHeight = 0;
	vector<BYTE> vecPixels;
	if (!LoadImagePixels(pPath, ColorKey, uWidth, uHeight, vecPixels))
	{
		return E_FAIL;
	}

	// 원본 크기 그대로, 밉맵 1단계, 바뀌지 않는 텍스처
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = uWidth;
	tDesc.Height = uHeight;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	const D3D11_SUBRESOURCE_DATA tData = { vecPixels.data(), uWidth * 4, 0 };

	ComPtr<ID3D11Texture2D> pTexture;
	if (FAILED(m_pDevice->CreateTexture2D(&tDesc, &tData, pTexture.GetAddressOf())))
	{
		return E_FAIL;
	}
	CTextureD3D11* const pRenderTexture = new CTextureD3D11;
	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture.Get(), nullptr, pRenderTexture->m_pSRV.GetAddressOf())))
	{
		delete pRenderTexture;
		return E_FAIL;
	}
	rTexInfo.pTexture = pRenderTexture;
	rTexInfo.ImgInfo.Width = uWidth;
	rTexInfo.ImgInfo.Height = uHeight;
	return S_OK;
}

// ID3DXSprite: 화면 위치 = (p − 중심점) × 월드 행렬
// SpriteBatch: 화면 위치 = 회전((p − origin) × 크기) + 위치
// 월드 행렬이 「크기 × Z 회전 × 이동」이면 아래처럼 분해해 두 식이 같아진다(intent/render-dxtk spec 「변환」).
void CRendererD3D11Tk::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
{
	if (pTexInfo == nullptr || pTexInfo->pTexture == nullptr)
	{
		return;
	}
	ID3D11ShaderResourceView* const pSRV = static_cast<const CTextureD3D11*>(pTexInfo->pTexture)->m_pSRV.Get();
	const XMVECTOR vColor = ToColorVector(Color);
	const XMFLOAT2 vOrigin(vCenter.x, vCenter.y);

	// 행렬식이 음수(뒤집힘)면 크기·회전으로 나눌 수 없다 — 이 스프라이트만 행렬을 그대로 써서 따로 그린다
	const float fDet = matWorld._11 * matWorld._22 - matWorld._12 * matWorld._21;
	if (fDet < 0.f)
	{
		m_pSpriteBatch->End();
		const XMMATRIX matTransform = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&matWorld));
		m_pSpriteBatch->Begin(SpriteSortMode_Deferred, m_pStates->NonPremultiplied(),
			nullptr, nullptr, nullptr, nullptr, matTransform);
		m_pSpriteBatch->Draw(pSRV, XMFLOAT2(0.f, 0.f), pSrcRect, vColor, 0.f, vOrigin);
		m_pSpriteBatch->End();
		BeginSpriteBatch();
		return;
	}

	const XMFLOAT2 vPosition(matWorld._41, matWorld._42);
	const XMFLOAT2 vScale(sqrtf(matWorld._11 * matWorld._11 + matWorld._12 * matWorld._12),
		sqrtf(matWorld._21 * matWorld._21 + matWorld._22 * matWorld._22));
	const float fRotation = atan2f(matWorld._12, matWorld._11);
	m_pSpriteBatch->Draw(pSRV, vPosition, pSrcRect, vColor, fRotation, vOrigin, vScale);
}

// EndFrame 뒤·Present 전 — 플립 모델의 백버퍼 내용은 Present 뒤에 보장되지 않는다
bool CRendererD3D11Tk::SaveBackBuffer(const TCHAR* pPath)
{
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()))))
	{
		return false;
	}
	return SUCCEEDED(SaveWICTextureToFile(m_pContext.Get(), pBackBuffer.Get(), GUID_ContainerFormatPng, pPath));
}
