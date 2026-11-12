#pragma once

#include "Defines.h"

// Client 전용 입력 장치 — DirectInput 키보드·마우스.
// 그리기 장치(Engine 의 CDevice)는 Client·툴 공용이라 입력을 떼어 여기에 둔다(intent/shared-render-lib).
class CInputDevice
{
	SINGLETON(CInputDevice);
private:
	LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pKeyBoard;
	LPDIRECTINPUTDEVICE8 m_pMouse;
	BYTE m_byKState[256];
	DIMOUSESTATE m_MState;
public:
	HRESULT Initialize(const HINSTANCE hInstance, const HWND hWnd);
	// 이번 프레임의 키보드·마우스 상태를 읽는다
	void SetKey();
	BYTE GetKeyBoardState(const short nKey) const { return m_byKState[nKey]; }
	const DIMOUSESTATE* GetMouseState(void) const { return &m_MState; }
	void Release();
private:
	CInputDevice(void);
public:
	~CInputDevice(void);
};
