#include "StdAfx.h"
#include "MainGame.h"
#include "Include.h"

CMainGame::CMainGame(void)
{}

CMainGame::~CMainGame(void)
{
	Release();
}

HRESULT CMainGame::Initialize()
{
//#if _DEBUG
//	if(FAILED(GET_SINGLE(CDevice)->InitDevice(WINMODE_WIN)))
//	{
//		ERR_MSG(g_hWnd, L"장치 초기화 실패");
//		return E_FAIL;
//	}
//#else
//	if(FAILED(GET_SINGLE(CDevice)->InitDevice(WINMODE_FULL)))
//	{
//		ERR_MSG(g_hWnd, L"장치 초기화 실패");
//		return E_FAIL;
//	}
//#endif

	if(FAILED(GET_SINGLE(CDevice)->InitDevice(WINMODE_WIN)))
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
	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CMainGame::Render()
{
	GET_SINGLE(CDevice)->Render_Begin();
	GET_SINGLE(CSceneMgr)->Render();
	GET_SINGLE(CDevice)->Render_End(g_hWnd);
}

void CMainGame::Release()
{
	GET_SINGLE(CTimeMgr)->DestroyInstance();
	GET_SINGLE(CSceneMgr)->DestroyInstance();
	GET_SINGLE(CDevice)->DestroyInstance();
	DESTROY_SINGLE(CTextureMgr);
}
