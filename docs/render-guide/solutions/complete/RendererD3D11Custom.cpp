#include "StdAfx.h"
#include "RendererD3D11Custom.h"
#include "ImageLoader.h"
#include "Debug.hpp"

#include <dxgi.h>
#include <wincodec.h>

#include "ScreenGrab.h"

// 빌드 때 Shaders/*.hlsl 을 컴파일한 바이트코드(g_SpriteVS·g_SpritePS 배열). 셰이더 내용만 고치면 다시 만들어진다
#include "SpriteVS.h"
#include "SpritePS.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;

CRendererD3D11Custom::CRendererD3D11Custom(void)
	: m_pBatchSRV(nullptr)
	, m_iDrawCalls(0)
	, m_iLastDrawCalls(0)
{
}

CRendererD3D11Custom::~CRendererD3D11Custom(void)
{
}

HRESULT CRendererD3D11Custom::Initialize(const HWND hWnd, const WINMODE Mode)
{
	if (Mode != WINMODE_WIN)
	{
		ERR_MSG(hWnd, L"D3D11 백엔드는 창 모드만 지원합니다");
		return E_FAIL;
	}

	// [1장] 스왑체인: 화면에 내보낼 버퍼 2개(플립 모델), 백버퍼는 B8G8R8A8
	DXGI_SWAP_CHAIN_DESC tSwapDesc = {};
	tSwapDesc.BufferDesc.Width = WINSIZEX;
	tSwapDesc.BufferDesc.Height = WINSIZEY;
	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tSwapDesc.SampleDesc.Count = 1;
	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapDesc.BufferCount = 2;
	tSwapDesc.OutputWindow = hWnd;
	tSwapDesc.Windowed = TRUE;
	tSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// [1장] 장치·컨텍스트·스왑체인을 한 번에 만든다. Debug 는 디버그 레이어를 켠다(없으면 끄고 다시)
	const D3D_FEATURE_LEVEL arrLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	UINT uFlags = 0;
#ifdef _DEBUG
	uFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uFlags,
		arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tSwapDesc,
		m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
	if (FAILED(hr) && (uFlags & D3D11_CREATE_DEVICE_DEBUG) != 0)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tSwapDesc,
			m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
	}
	if (FAILED(hr))
	{
		ERR_MSG(hWnd, L"D3D11 장치 생성 실패(0x%08X)", static_cast<unsigned int>(hr));
		return E_FAIL;
	}

	// [1장] 백버퍼에 그리려면 「렌더 타깃 뷰」가 필요하다
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf())))
		|| FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRTV.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 렌더 타깃 생성 실패");
		return E_FAIL;
	}

	// [2장] 셰이더 — 빌드 때 컴파일된 바이트코드(g_SpriteVS·g_SpritePS)로 만든다
	if (FAILED(m_pDevice->CreateVertexShader(g_SpriteVS, sizeof(g_SpriteVS), nullptr, m_pVertexShader.GetAddressOf()))
		|| FAILED(m_pDevice->CreatePixelShader(g_SpritePS, sizeof(g_SpritePS), nullptr, m_pPixelShader.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 셰이더 생성 실패");
		return E_FAIL;
	}

	// [2장] 입력 레이아웃: 정점 버퍼의 바이트를 셰이더의 VS_INPUT 으로 읽는 방법
	const D3D11_INPUT_ELEMENT_DESC arrElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	if (FAILED(m_pDevice->CreateInputLayout(arrElements, _countof(arrElements), g_SpriteVS, sizeof(g_SpriteVS),
		m_pInputLayout.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 입력 레이아웃 생성 실패");
		return E_FAIL;
	}

	// [3장] 사각형 하나 = 정점 4개. CPU 가 고쳐 쓰므로 DYNAMIC + CPU 쓰기. [7장] 최대 스프라이트 수만큼
	D3D11_BUFFER_DESC tVBDesc = {};
	tVBDesc.ByteWidth = static_cast<UINT>(sizeof(SPRITEVERTEX) * 4 * MAX_BATCH_SPRITES);
	tVBDesc.Usage = D3D11_USAGE_DYNAMIC;
	tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(m_pDevice->CreateBuffer(&tVBDesc, nullptr, m_pVertexBuffer.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 정점 버퍼 생성 실패");
		return E_FAIL;
	}

	// [3장] 정점 4개(0 왼쪽 위, 1 오른쪽 위, 2 왼쪽 아래, 3 오른쪽 아래)를 삼각형 2개로 잇는 순서 — 바뀌지 않는다
	// [7장] 스프라이트 n 번째는 정점 4n ~ 4n+3 을 같은 순서로 잇는다
	vector<WORD> vecIndices;
	vecIndices.reserve(MAX_BATCH_SPRITES * 6);
	for (size_t i = 0; i < MAX_BATCH_SPRITES; ++i)
	{
		const WORD wBase = static_cast<WORD>(i * 4);
		const WORD arrQuadIndices[6] = { 0, 1, 2, 2, 1, 3 };
		for (const WORD wIndex : arrQuadIndices)
		{
			vecIndices.push_back(static_cast<WORD>(wBase + wIndex));
		}
	}
	D3D11_BUFFER_DESC tIBDesc = {};
	tIBDesc.ByteWidth = static_cast<UINT>(sizeof(WORD) * vecIndices.size());
	tIBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	const D3D11_SUBRESOURCE_DATA tIBData = { vecIndices.data(), 0, 0 };
	if (FAILED(m_pDevice->CreateBuffer(&tIBDesc, &tIBData, m_pIndexBuffer.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 인덱스 버퍼 생성 실패");
		return E_FAIL;
	}

	// [3장] 직교 투영: 화면 픽셀(왼쪽 위 0,0 / 오른쪽 아래 WINSIZEX,WINSIZEY) → 클립 좌표(-1~1, 위가 +)
	XMFLOAT4X4 matProjection;
	XMStoreFloat4x4(&matProjection, XMMatrixOrthographicOffCenterLH(
		0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 0.f, 1.f));
	D3D11_BUFFER_DESC tCBDesc = {};
	tCBDesc.ByteWidth = sizeof(matProjection);
	tCBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	const D3D11_SUBRESOURCE_DATA tCBData = { &matProjection, 0, 0 };
	if (FAILED(m_pDevice->CreateBuffer(&tCBDesc, &tCBData, m_pProjectionBuffer.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 상수 버퍼 생성 실패");
		return E_FAIL;
	}

	// [4장] 샘플러: 텍셀 사이는 선형 보간, UV 가 0~1 을 벗어나면 가장자리 색(ID3DXSprite 와 같은 설정)
	D3D11_SAMPLER_DESC tSamplerDesc = {};
	tSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	tSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	tSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	tSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	tSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(m_pDevice->CreateSamplerState(&tSamplerDesc, m_pSampler.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 샘플러 생성 실패");
		return E_FAIL;
	}

	// [5장] 알파 블렌드: 결과 = 그린 색 × 알파 + 이미 있는 색 × (1 − 알파). D3DXSPRITE_ALPHABLEND 와 같다(알파 채널도 같은 식)
	D3D11_BLEND_DESC tBlendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC& rTarget = tBlendDesc.RenderTarget[0];
	rTarget.BlendEnable = TRUE;
	rTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rTarget.BlendOp = D3D11_BLEND_OP_ADD;
	rTarget.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	rTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	rTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	if (FAILED(m_pDevice->CreateBlendState(&tBlendDesc, m_pBlendState.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 블렌드 상태 생성 실패");
		return E_FAIL;
	}

	// [6장] 기본 래스터라이저는 뒷면(화면에서 반시계 방향)을 버린다. 좌우로 뒤집는 행렬이면 사각형이 뒷면이 되므로 컬링을 끈다
	D3D11_RASTERIZER_DESC tRasterDesc = {};
	tRasterDesc.FillMode = D3D11_FILL_SOLID;
	tRasterDesc.CullMode = D3D11_CULL_NONE;
	tRasterDesc.DepthClipEnable = TRUE;
	if (FAILED(m_pDevice->CreateRasterizerState(&tRasterDesc, m_pRasterizerState.GetAddressOf())))
	{
		ERR_MSG(hWnd, L"D3D11 래스터라이저 상태 생성 실패");
		return E_FAIL;
	}

	m_vecBatch.reserve(MAX_BATCH_SPRITES * 4);
	return S_OK;
}

void CRendererD3D11Custom::BeginFrame(const D3DCOLOR ClearColor)
{
	// [1장] 플립 모델은 Present 뒤에 렌더 타깃 연결이 풀린다 — 프레임마다 다시 연결한다
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), nullptr);
	const D3D11_VIEWPORT tViewport = { 0.f, 0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 1.f };
	m_pContext->RSSetViewports(1, &tViewport);
	m_pContext->RSSetState(m_pRasterizerState.Get());

	// [1장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA
	const float arrColor[4] = { ((ClearColor >> 16) & 0xFF) / 255.f, ((ClearColor >> 8) & 0xFF) / 255.f,
		(ClearColor & 0xFF) / 255.f, ((ClearColor >> 24) & 0xFF) / 255.f };
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), arrColor);

	// [3장] 이번 프레임 동안 바뀌지 않는 파이프라인 설정은 한 번만 건다
	const UINT uStride = sizeof(SPRITEVERTEX);
	const UINT uOffset = 0;
	m_pContext->IASetInputLayout(m_pInputLayout.Get());
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &uStride, &uOffset);
	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pContext->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
	m_pContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);

	m_vecBatch.clear();
	m_pBatchSRV = nullptr;
	m_iDrawCalls = 0;
}

void CRendererD3D11Custom::EndFrame(void)
{
	// [7장] 모아 둔 스프라이트를 마저 그린다
	FlushBatch();
	if (m_iDrawCalls != m_iLastDrawCalls)
	{
		DebugOut(L"[D3D11 custom] draw calls/frame: %d", m_iDrawCalls);	// VS 출력 창에서 배칭 효과를 본다
		m_iLastDrawCalls = m_iDrawCalls;
	}
}

// [7장] 모아 둔 정점을 동적 버퍼에 한 번에 올리고 그리기 호출 한 번으로 그린다
void CRendererD3D11Custom::FlushBatch(void)
{
	if (m_vecBatch.empty())
	{
		return;
	}
	D3D11_MAPPED_SUBRESOURCE tMapped = {};
	if (SUCCEEDED(m_pContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapped)))
	{
		memcpy(tMapped.pData, m_vecBatch.data(), sizeof(SPRITEVERTEX) * m_vecBatch.size());
		m_pContext->Unmap(m_pVertexBuffer.Get(), 0);
		m_pContext->PSSetShaderResources(0, 1, &m_pBatchSRV);
		m_pContext->DrawIndexed(static_cast<UINT>(m_vecBatch.size() / 4 * 6), 0, 0);
		++m_iDrawCalls;
	}
	m_vecBatch.clear();
}

void CRendererD3D11Custom::Present(const HWND /*hWnd*/)
{
	// [1장] 스왑체인이 그릴 창을 이미 안다. 수직 동기 없이 내보낸다(D3D9 판과 같게)
	m_pSwapChain->Present(0, 0);
}

HRESULT CRendererD3D11Custom::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
{
	UINT uWidth = 0;
	UINT uHeight = 0;
	vector<BYTE> vecPixels;
	if (!LoadImagePixels(pPath, ColorKey, uWidth, uHeight, vecPixels))
	{
		return E_FAIL;
	}

	// [4장] 픽셀(BGRA, 컬러키 적용됨)로 텍스처를 만든다 — 원본 크기, 밉맵 1단계, 만든 뒤 바뀌지 않음
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = uWidth;
	tDesc.Height = uHeight;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	const D3D11_SUBRESOURCE_DATA tData = { vecPixels.data(), uWidth * 4, 0 };	// 한 줄의 바이트 수 = 너비 × 4
	ComPtr<ID3D11Texture2D> pTexture;
	if (FAILED(m_pDevice->CreateTexture2D(&tDesc, &tData, pTexture.GetAddressOf())))
	{
		return E_FAIL;
	}

	// [4장] 셰이더는 텍스처를 직접 읽지 않고 SRV 를 통해 읽는다
	CTextureD3D11Custom* const pRenderTexture = new CTextureD3D11Custom;
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

void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
{
	if (pTexInfo == nullptr || pTexInfo->pTexture == nullptr)
	{
		return;
	}
	// [4장] 이 스프라이트의 텍스처. [7장] 모으는 중인 텍스처와 다르거나 버퍼가 차면 지금까지 모은 것을 먼저 그린다
	ID3D11ShaderResourceView* const pSRV = static_cast<const CTextureD3D11Custom*>(pTexInfo->pTexture)->m_pSRV.Get();
	if (pSRV != m_pBatchSRV || m_vecBatch.size() >= MAX_BATCH_SPRITES * 4)
	{
		FlushBatch();
		m_pBatchSRV = pSRV;
	}

	// [6장] 그릴 범위: 원본 사각형(nullptr 이면 텍스처 전체), 텍셀 단위
	const float fTexWidth = static_cast<float>(pTexInfo->ImgInfo.Width);
	const float fTexHeight = static_cast<float>(pTexInfo->ImgInfo.Height);
	RECT rcSrc = { 0, 0, static_cast<LONG>(pTexInfo->ImgInfo.Width), static_cast<LONG>(pTexInfo->ImgInfo.Height) };
	if (pSrcRect != nullptr)
	{
		rcSrc = *pSrcRect;
	}
	const float fWidth = static_cast<float>(rcSrc.right - rcSrc.left);
	const float fHeight = static_cast<float>(rcSrc.bottom - rcSrc.top);

	// [6장] ID3DXSprite 와 같은 식: 화면 위치 = (꼭짓점 − 중심점) × 월드 행렬.
	// 중심점은 원본 사각형의 왼쪽 위 기준. D3DX 행렬은 「행 벡터 × 행렬」이라 x' = x·_11 + y·_21 + _41
	const float arrLocalX[4] = { 0.f, fWidth, 0.f, fWidth };
	const float arrLocalY[4] = { 0.f, 0.f, fHeight, fHeight };

	// [5장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA. 네 꼭짓점이 같은 색이다
	const XMFLOAT4 vColor(((Color >> 16) & 0xFF) / 255.f, ((Color >> 8) & 0xFF) / 255.f,
		(Color & 0xFF) / 255.f, ((Color >> 24) & 0xFF) / 255.f);

	for (int i = 0; i < 4; ++i)
	{
		const float fX = arrLocalX[i] - vCenter.x;
		const float fY = arrLocalY[i] - vCenter.y;
		SPRITEVERTEX tVertex;
		tVertex.vPos = XMFLOAT2(fX * matWorld._11 + fY * matWorld._21 + matWorld._41,
			fX * matWorld._12 + fY * matWorld._22 + matWorld._42);
		// [6장] UV = 텍셀 위치 ÷ 텍스처 크기
		tVertex.vUV = XMFLOAT2((rcSrc.left + arrLocalX[i]) / fTexWidth, (rcSrc.top + arrLocalY[i]) / fTexHeight);
		tVertex.vColor = vColor;
		m_vecBatch.push_back(tVertex);	// [7장] 바로 그리지 않고 모은다
	}
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
