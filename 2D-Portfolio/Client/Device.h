#pragma once

#include "Defines.h"

class CDevice
{
	SINGLETON(CDevice);
private:
	// ��ġ�� ������ �����ϴ� ��ü.
	LPDIRECT3D9 m_p3D;
	// ��ġ�� ��ǥ�ϴ� ��ü
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXSPRITE m_pSprite;

	LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pKeyBoard;
	LPDIRECTINPUTDEVICE8 m_pMouse;
	BYTE m_byKState[256];
	DIMOUSESTATE m_MState;

public:
	HRESULT InitDevice(WINMODE Mode);
public:
	void SetKey();
public:
	BYTE GetKeyBoardState(short nKey){return m_byKState[nKey];}
	const DIMOUSESTATE* GetMouseState(void) {return &m_MState;}
public:
	LPD3DXSPRITE GetSprite()
	{ return m_pSprite; }
	LPDIRECT3DDEVICE9 GetDevice()
	{ return m_pDevice; }
	void Render_Begin();
	void Render_End(HWND hWnd);
	void Release();
private:
	CDevice(void);
public:
	~CDevice(void);
};

