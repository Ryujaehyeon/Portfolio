#include "StdAfx.h"
#include "InputDevice.h"

CInputDevice::CInputDevice(void)
	: m_pInput(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
	, m_byKState()
	, m_MState()
{
}

CInputDevice::~CInputDevice(void)
{
	Release();
}

HRESULT CInputDevice::Initialize(const HINSTANCE hInstance, const HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, reinterpret_cast<void**>(&m_pInput), nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
	{
		return E_FAIL;
	}
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		return E_FAIL;
	}
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDevice::SetKey()
{
	// 스모크 모드에서는 실제 키보드·마우스를 읽지 않는다 — 비활성 창에서도 입력을 받는 장치라서(intent/smoke-input-isolation)
	if (IsSmokeMode())
	{
		memset(m_byKState, 0, sizeof(m_byKState));
		memset(&m_MState, 0, sizeof(m_MState));
		return;
	}
	m_pKeyBoard->GetDeviceState(sizeof(m_byKState), m_byKState);
	m_pMouse->GetDeviceState(sizeof(m_MState), &m_MState);
}

void CInputDevice::Release()
{
	if (m_pMouse != nullptr)
	{
		m_pMouse->Release();
		m_pMouse = nullptr;
	}
	if (m_pKeyBoard != nullptr)
	{
		m_pKeyBoard->Release();
		m_pKeyBoard = nullptr;
	}
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}
