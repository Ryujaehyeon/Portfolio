#pragma once

#include "Renderer.h"

// Direct3D 9 + ID3DXSprite 렌더러(intent/render-backend). 기존 CDevice 를 옮긴 것이다.
class CRendererD3D9
	: public IRenderer
{
private:
	// D3D9 텍스처를 IRenderTexture 로 감싼다
	class CTextureD3D9
		: public IRenderTexture
	{
	public:
		LPDIRECT3DTEXTURE9 m_pTexture;
	public:
		explicit CTextureD3D9(LPDIRECT3DTEXTURE9 pTexture)
			: m_pTexture(pTexture)
		{
		}
		virtual ~CTextureD3D9(void) override
		{
			if (m_pTexture != nullptr)
			{
				m_pTexture->Release();
			}
		}
		CTextureD3D9(const CTextureD3D9&) = delete;
		CTextureD3D9& operator=(const CTextureD3D9&) = delete;
	};
private:
	// 장치의 성능을 조사하는 객체.
	LPDIRECT3D9 m_p3D;
	// 장치를 대표하는 객체
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXSPRITE m_pSprite;
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
	void Release(void);
public:
	CRendererD3D9(void);
	virtual ~CRendererD3D9(void) override;
};
