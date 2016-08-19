#include "StdAfx.h"
#include "Stage2.h"
#include "Include.h"

CStage2::CStage2(void)
{
	GameSound Sound;
	Sound->Stop(L"Tristram");
	Sound->LoadSound(L"Cave", L"Cave.mp3", true);
	Sound->SetVolume(10);
	Sound->Play(L"Cave");       //사운드
}


CStage2::~CStage2(void)
{
	Release();
}

HRESULT CStage2::Initialize()
{
	return S_OK;
}

SCENEID CStage2::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	if(GET_SINGLE(CObjMgr)->GetMonsterCnt() < 10)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, VIPER)))
		{
			ERR_MSG(g_hWnd, L"Viper 몬스터 객체 생성 실패");
		}
	}
	
	//// 아직 몬스터 텍스쳐 추가 코드가 없음
	//if(GET_SINGLE(CObjMgr)->GetBossCnt() < 1)
	//{
	//	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
	//		, IZUAL)))
	//	{
	//		ERR_MSG(g_hWnd, L"IZUAL 몬스터 객체 생성 실패");
	//	}

	//	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
	//		, ANDARIEL)))
	//	{
	//		ERR_MSG(g_hWnd, L"ANDARIEL 몬스터 객체 생성 실패");
	//	}
	//}

	// 씬ID가 NONPASS가 아니면 씬매니져에서 해당ID의 씬을 생성 및 초기화
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if(iScene == SCENEID_END)
		return SCENEID_END;
}

void CStage2::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CStage2::Release()
{
	//DESTROY_SINGLE(CTextureMgr);
	//GET_SINGLE(CObjMgr)->DestroyInstance();
	//SAFE_DELETE<CPrototype>(&m_pPrototype);
	//SAFE_DELETE<CPrototype>(&m_pSkillPrototype);
}
