#include "StdAfx.h"
#include "Stage2.h"
#include "Include.h"

CStage2::CStage2(void)
{
}


CStage2::~CStage2(void)
{
}

HRESULT CStage2::Initialize()
{
	return S_OK;
}

SCENEID CStage2::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();


	// 씬ID가 NONPASS가 아니면 씬매니져에서 해당ID의 씬을 생성 및 초기화
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene((SCENEID)iScene);

	if(iScene == SCENEID_END)
		return SCENEID_END;
}

void CStage2::Render()
{

}

void CStage2::Release()
{

}
