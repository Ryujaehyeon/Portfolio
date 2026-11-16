#include "StdAfx.h"
#include "MainGame.h"
#include "Include.h"
#include "RendererD3D9.h"
#include "RendererD3D11Tk.h"
#include "RendererD3D11Custom.h"

// 렌더러 백엔드는 앱이 골라 만든다 — Engine 은 어떤 백엔드가 있는지 모른다(intent/render-backend).
// -d3d11 이면 D3D11 + DirectXTK(intent/render-dxtk), -d3d11custom 이면 직접 구현 판(intent/render-guide), 아니면 D3D9
static IRenderer* CreateRendererBackend()
{
	for (int i = 1; i < __argc; ++i)
	{
		if (wcscmp(__wargv[i], L"-d3d11") == 0)
		{
			SmokeLog(L"renderer D3D11 (DirectXTK)");
			return new CRendererD3D11Tk;
		}
		if (wcscmp(__wargv[i], L"-d3d11custom") == 0)
		{
			SmokeLog(L"renderer D3D11 (custom)");
			return new CRendererD3D11Custom;
		}
	}
	SmokeLog(L"renderer D3D9");
	return new CRendererD3D9;
}

CMainGame::CMainGame(void)
{}

CMainGame::~CMainGame(void)
{
	Release();
}

HRESULT CMainGame::Initialize()
{
	if(FAILED(GET_SINGLE(CRenderer)->Initialize(CreateRendererBackend(), g_hWnd, WINMODE_WIN)))
	{
		ERR_MSG(g_hWnd, L"장치 초기화 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CSceneMgr)->InitScene(SCENEID_LOGO)))
	{
		ERR_MSG(g_hWnd, L"장면 초기화 실패");
		return E_FAIL;
	}

	GET_SINGLE(CTimeMgr)->InitTimeMgr();

	return S_OK;
}

SCENEID CMainGame::Progress()
{
	GET_SINGLE(CKeyMgr)->CheckKey();
	GET_SINGLE(CTimeMgr)->SetTime();
	SCENEID iScene = GET_SINGLE(CSceneMgr)->Progress();
	GET_SINGLE(CStateRecorder)->Record();
	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CMainGame::Render()
{
	GET_SINGLE(CRenderer)->BeginFrame(D3DCOLOR_ARGB(255, 0, 0, 0));
	GET_SINGLE(CSceneMgr)->Render();
	GET_SINGLE(CRenderer)->EndFrame(g_hWnd);
}

void CMainGame::Release()
{
	GET_SINGLE(CTimeMgr)->DestroyInstance();
	GET_SINGLE(CStateRecorder)->DestroyInstance();
	GET_SINGLE(CSceneMgr)->DestroyInstance();
	GET_SINGLE(CRenderer)->DestroyInstance();
	DESTROY_SINGLE(CTextureMgr);
}
