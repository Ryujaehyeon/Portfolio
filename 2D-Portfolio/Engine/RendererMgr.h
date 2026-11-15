#pragma once

#include "Renderer.h"

// 게임 코드가 쓰는 렌더러 싱글턴(intent/render-backend). 앱이 만든 백엔드(IRenderer)를 받아 소유하고 그대로 전달한다.
// Engine 은 어떤 백엔드가 있는지 모른다 — 백엔드 생성은 앱이 한다.
class CRenderer
{
	SINGLETON(CRenderer);
private:
	IRenderer* m_pBackend;
	// 다음 EndFrame 에서 백버퍼를 저장할 경로(비어 있으면 저장 안 함, intent/render-capture)
	wstring m_strCapturePath;
public:
	// pBackend 의 소유권을 넘겨받는다(실패해도 이 객체가 지운다)
	HRESULT Initialize(IRenderer* pBackend, const HWND hWnd, const WINMODE Mode);
	void BeginFrame(const D3DCOLOR ClearColor);
	// 그리기를 끝내고, 캡처가 걸려 있으면 저장한 뒤, hWnd 에 내보낸다
	void EndFrame(const HWND hWnd);
	HRESULT CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo);
	void DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color);
	// 이번 프레임을 다 그린 뒤(Present 직전) 백버퍼를 PNG 로 저장하도록 걸어 둔다
	void RequestCapture(const TCHAR* pPath) { m_strCapturePath = pPath; }
	void Release(void);
private:
	CRenderer(void);
public:
	~CRenderer(void);
};
