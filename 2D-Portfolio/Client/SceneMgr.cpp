#include "StdAfx.h"
#include "SceneMgr.h"
#include "Include.h"

CSceneMgr::CSceneMgr(void)
:m_pSceneObj(0)
{}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

HRESULT CSceneMgr::InitScene(SCENEID ID)
{
	DebugOut(L"Scene : %d", ID);

	// Logo에서 받은 씬ID를 해당 씬으로 바꿈(생성 및 초기화) 
	SAFE_DELETE<CSceneObj>(&m_pSceneObj);
	switch(ID)
	{
	case SCENEID_LOGO:
		m_pSceneObj = new CLogo;
		break;

	case SCENEID_MENU:
		m_pSceneObj = new CMenuD2;
		break;

	case SCENEID_STAGE:
		m_pSceneObj = new CStage;
		break;

	default:
		return E_FAIL;
		break;
	}

	const HRESULT hr = m_pSceneObj->Initialize();
	SmokeCheck(SUCCEEDED(hr), L"scene %d init", ID);
	if(FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

SCENEID CSceneMgr::Progress()
{
	SCENEID iScene = m_pSceneObj->Progress();



	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CSceneMgr::Render()
{
	m_pSceneObj->Render();
}

void CSceneMgr::Release()
{
	SAFE_DELETE<CSceneObj>(&m_pSceneObj);
}