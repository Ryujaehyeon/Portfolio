#pragma once

#include "Renderer.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

#include "SpriteBatch.h"
#include "CommonStates.h"

// Direct3D 11 + DirectXTK SpriteBatch 렌더러(intent/render-dxtk). 직접 구현 판(A)의 정답·비교 기준이다.
class CRendererD3D11Tk
	: public IRenderer
{
private:
	// D3D11 텍스처는 셰이더가 읽는 SRV 하나로 다룬다
	class CTextureD3D11
		: public IRenderTexture
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_pStates;
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
	void BeginSpriteBatch(void);
public:
	CRendererD3D11Tk(void);
	virtual ~CRendererD3D11Tk(void) override;
};
