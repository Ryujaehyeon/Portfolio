#include "StdAfx.h"
#include "MainGame.h"
#include "Include.h"

HANDLE CMainGame::hEvent = NULL;

DWORD WINAPI ThreadFunc(LPVOID temp)
{
	// 로딩데이터
	Sleep(10000);
	SetEvent(CMainGame::hEvent);
	return 0;
}

CMainGame::CMainGame(void)
{
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hdc = GetDC(g_hWnd);
	DWORD ID;
	CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ID);
}

CMainGame::~CMainGame(void)
{
	ReleaseDC(g_hWnd, hdc);
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
	
	GET_SINGLE(CTimeMgr)->SetTime();
	GET_SINGLE(CKeyMgr)->CheckKey();
	//if(WaitForSingleObject(hEvent, 1) == WAIT_OBJECT_0)
	//{
	//	SCENEID iScene  = GET_SINGLE(CSceneMgr)->Progress();
	//	if (iScene == SCENEID_END)
	//		return SCENEID_END;
	//	return iScene;
	//}
	//else
	//{
	//	SCENEID iScene  = GET_SINGLE(CSceneMgr)->Progress();
	//	////if (iScene == SCENEID_END)
	//	////	return SCENEID_END;
	//	////return iScene;
	//	if(iScene > SCENEID_NONPASS)
	//		GET_SINGLE(CSceneMgr)->InitScene(SCENEID_LODING);
	//	return iScene;
	//}

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
