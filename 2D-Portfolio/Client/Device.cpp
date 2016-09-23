#include "StdAfx.h"
#include "Device.h"


CDevice::CDevice(void)
	:m_p3D(0)
	,m_pDevice(0)
{}

CDevice::~CDevice(void)
{ Release(); }

HRESULT CDevice::InitDevice(WINMODE Mode)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 장치의 성능을 조사한다.
	D3DCAPS9 devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	// 장치 성능조사
	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(g_hWnd, L"장치정보얻어오기 실패");
		return E_FAIL;
	}
	DWORD BehaviorFlag = 0;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		BehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		BehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WINSIZEX;
	d3dpp.BackBufferHeight = WINSIZEY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = true;

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = Mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치 생성
	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd, BehaviorFlag, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(g_hWnd, L"장치생성 실패");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	DirectInput8Create(hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL);

	m_pInput->CreateDevice(
		GUID_SysKeyboard, &m_pKeyBoard, NULL);
	m_pKeyBoard->SetCooperativeLevel(
		g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->SetDataFormat(
		&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();

	m_pInput->CreateDevice(
		GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetCooperativeLevel(
		g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->SetDataFormat(
		&c_dfDIMouse);
	m_pMouse->Acquire();

	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET
		| D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, hWnd, NULL);
}

void CDevice::Release()
{
	m_pMouse->Release();
	m_pKeyBoard->Release();
	m_pInput->Release();

	m_pSprite->Release();
	m_pDevice->Release();
	m_p3D->Release();
}
void CDevice::SetKey()
{
	m_pKeyBoard->GetDeviceState(
		sizeof(m_byKState), m_byKState);
	m_pMouse->GetDeviceState(
		sizeof(m_MState), &m_MState);
}
