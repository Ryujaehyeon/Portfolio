#pragma once

#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

// Direct3D 11 스프라이트 렌더러 — 직접 구현 판(A). -d3d11custom 으로 선택한다(intent/render-guide).
// 가이드 docs/render-guide/ 의 장 순서대로 채운다. 정답·비교 기준은 CRendererD3D11Tk(B).
// 지금은 뼈대다 — 빌드되고 게임 로직은 돌지만 아무것도 그리지 않는다.
class CRendererD3D11Custom
	: public IRenderer
{
private:
	// [2장] 정점 하나 — 셰이더의 VS_INPUT 과 같은 순서·형식(위치는 2장에서 클립 좌표, 3장부터 화면 픽셀)
	struct SPRITEVERTEX
	{
		DirectX::XMFLOAT2 vPos;
		DirectX::XMFLOAT2 vUV;
		DirectX::XMFLOAT4 vColor;
	};
private:
	// 이 백엔드가 만든 텍스처 — [4장] 셰이더가 읽는 SRV 하나
	class CTextureD3D11Custom
		: public IRenderTexture
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};
private:
	// [1장] 장치·컨텍스트·스왑체인·렌더 타깃 뷰
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
	// [2장] 셰이더·입력 레이아웃
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// [3장] 정점(CPU 가 매번 고쳐 쓰는 동적 버퍼)·인덱스·투영 행렬 상수 버퍼. [7장] 크기를 한 번에 그릴 최대 스프라이트 수만큼 키운다
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pProjectionBuffer;
	// [4장] 텍스처를 읽는 방법(필터·가장자리 처리)
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	// [5장] 그린 색과 이미 있는 색을 섞는 방법(알파 블렌드)
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
	// [6장] 뒤집힌(행렬식이 음수인) 스프라이트도 그리려고 컬링을 끈 래스터라이저 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
	// [7장] 배칭: 같은 텍스처가 이어지는 동안 정점을 모았다가 한 번에 그린다
	static constexpr size_t MAX_BATCH_SPRITES = 2048;	// 정점 4개씩 — 16비트 인덱스(최대 65535)에 들어간다
	vector<SPRITEVERTEX> m_vecBatch;
	ID3D11ShaderResourceView* m_pBatchSRV;	// 모으는 중인 텍스처(소유하지 않는다 — 텍스처는 그리는 동안 살아 있다)
	int m_iDrawCalls;						// 이번 프레임의 그리기 호출 수(확인용)
	int m_iLastDrawCalls;
public:
	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
	virtual void BeginFrame(const D3DCOLOR ClearColor) override;
	virtual void EndFrame(void) override;
	virtual void Present(const HWND hWnd) override;
	virtual HRESULT CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo) override;
	virtual void DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color) override;
	virtual bool SaveBackBuffer(const TCHAR* pPath) override;
private:
	void FlushBatch(void);
public:
	CRendererD3D11Custom(void);
	virtual ~CRendererD3D11Custom(void) override;
};
