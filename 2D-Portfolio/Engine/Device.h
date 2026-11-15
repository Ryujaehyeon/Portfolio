#pragma once

#include "Defines.h"

class CDevice
{
	SINGLETON(CDevice);
private:
	// 장치의 성능을 조사하는 객체.
	LPDIRECT3D9 m_p3D;
	// 장치를 대표하는 객체
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXSPRITE m_pSprite;
	// 다음 Render_End 에서 백버퍼를 저장할 경로(비어 있으면 저장 안 함, intent/render-capture)
	wstring m_strCapturePath;

public:
	// 장치를 만든다 — 그릴 창과 창 모드는 앱이 정한다(Client·툴 공용, intent/shared-render-lib)
	HRESULT InitDevice(const HWND hWnd, const WINMODE Mode);
public:
	LPD3DXSPRITE GetSprite()
	{ return m_pSprite; }
	LPDIRECT3DDEVICE9 GetDevice()
	{ return m_pDevice; }
	// 화면을 ClearColor 로 지우고 그리기를 시작한다
	void Render_Begin(const D3DCOLOR ClearColor);
	void Render_End(HWND hWnd);
	// 이번 프레임을 다 그린 뒤(Present 직전) 백버퍼를 PNG 로 저장하도록 걸어 둔다
	void RequestCapture(const TCHAR* pPath) { m_strCapturePath = pPath; }
	void Release();
private:
	bool SaveBackBuffer(const TCHAR* pPath);
private:
	CDevice(void);
public:
	~CDevice(void);
};

