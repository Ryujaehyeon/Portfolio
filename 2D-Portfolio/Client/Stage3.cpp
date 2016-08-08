#include "StdAfx.h"
#include "Stage3.h"
#include "Include.h"

CStage3::CStage3(void)
{
}


CStage3::~CStage3(void)
{
}

HRESULT CStage3::Initialize()
{
	return S_OK;
}

SCENEID CStage3::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();


	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene((SCENEID)iScene);

	if(iScene == SCENEID_END)
		return SCENEID_END;
}

void CStage3::Render()
{

}

void CStage3::Release()
{

}
