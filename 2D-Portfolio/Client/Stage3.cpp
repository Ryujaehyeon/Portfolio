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

	if(GET_SINGLE(CObjMgr)->GetMonsterCnt() < 10)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, SANDRAIDER)))
		{
			ERR_MSG(g_hWnd, L"SANDRAIDER ���� ��ü ���� ����");
		}
	}
	
	//// ���� ���� �ؽ��� �߰� �ڵ尡 ����
	//if(GET_SINGLE(CObjMgr)->GetBossCnt() < 1)
	//{
	//	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
	//		, DIABLO)))
	//	{
	//		ERR_MSG(g_hWnd, L"DIABLO ���� ��ü ���� ����");
	//	}
	//}

	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CStage3::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CStage3::Release()
{
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
	SAFE_DELETE<CPrototype>(&m_pPrototype);
	SAFE_DELETE<CPrototype>(&m_pSkillPrototype);
}
