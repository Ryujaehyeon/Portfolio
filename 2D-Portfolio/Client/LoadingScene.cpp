#include "StdAfx.h"
#include "LoadingScene.h"
#include "Include.h"

CLoadingScene::CLoadingScene(void)
{
}


CLoadingScene::~CLoadingScene(void)
{
		Release();
}

HRESULT CLoadingScene::Initialize()
{
	m_pPrototype = new LoadingObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Loading")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Loading/Loading_%02d.png",
		L"Loading", TEXTYPE_MULTI, L"Loading", 10.0f)))
	{
		ERR_MSG(g_hWnd, L"Loading �ε� ����" );
		return E_FAIL;
	}
	return S_OK;
}

SCENEID CLoadingScene::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CLoadingScene::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CLoadingScene::Release()
{
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
	SAFE_DELETE<CPrototype>(&m_pPrototype);
}
