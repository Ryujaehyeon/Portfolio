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
	if(FAILED(GET_SINGLE(CDevice)->InitDevice(g_hWnd, WINMODE_WIN)))
	{
		ERR_MSG(g_hWnd, L"장치 초기화 실패");
		return E_FAIL;
	}
	// 그리기 장치는 Client·툴 공용(Engine)이라 Client 의 렌더 상태는 여기서 켠다
	GET_SINGLE(CDevice)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	if(FAILED(GET_SINGLE(CInputDevice)->Initialize(hInst, g_hWnd)))
	{
		ERR_MSG(g_hWnd, L"입력 장치 초기화 실패");
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
	GET_SINGLE(CDevice)->Render_Begin(D3DCOLOR_ARGB(255, 0, 0, 0));
	GET_SINGLE(CSceneMgr)->Render();
	GET_SINGLE(CDevice)->Render_End(g_hWnd);
}

void CMainGame::Release()
{
	GET_SINGLE(CTimeMgr)->DestroyInstance();
	GET_SINGLE(CSceneMgr)->DestroyInstance();
	GET_SINGLE(CInputDevice)->DestroyInstance();
	GET_SINGLE(CDevice)->DestroyInstance();
	DESTROY_SINGLE(CTextureMgr);
}
