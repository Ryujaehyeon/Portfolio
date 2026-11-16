#pragma once

#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>

// Direct3D 11 스프라이트 렌더러 — 직접 구현 판(A). -d3d11custom 으로 선택한다(intent/render-guide).
// 가이드 docs/render-guide/ 의 장 순서대로 채운다. 정답·비교 기준은 CRendererD3D11Tk(B).
// 지금은 뼈대다 — 빌드되고 게임 로직은 돌지만 아무것도 그리지 않는다.
class CRendererD3D11Custom
	: public IRenderer
{
private:
	// 이 백엔드가 만든 텍스처 — TODO(4장): 셰이더가 읽을 SRV 를 담는다
	class CTextureD3D11Custom
		: public IRenderTexture
	{
	public:
	};
private:
	// TODO(1장): 장치·컨텍스트·스왑체인·렌더 타깃 뷰를 만든다
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
	// TODO(2장~7장): 장마다 필요한 멤버를 여기에 더한다
public:
	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
	virtual void BeginFrame(const D3DCOLOR ClearColor) override;
	virtual void EndFrame(void) override;
	virtual void Present(const HWND hWnd) override;
	virtual HRESULT CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo) override;
	virtual void DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color) override;
	virtual bool SaveBackBuffer(const TCHAR* pPath) override;
public:
	CRendererD3D11Custom(void);
	virtual ~CRendererD3D11Custom(void) override;
};
