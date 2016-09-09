#include "StdAfx.h"
#include "LodingScene.h"
#include "Include.h"

CLodingScene::CLodingScene(void)
{
}


CLodingScene::~CLodingScene(void)
{
		Release();
}

HRESULT CLodingScene::Initialize()
{
	m_pPrototype = new CLogoObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Loding")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Loading/Loading_%02.png",
		L"Loding", TEXTYPE_MULTI, L"Loding", 10)))
	{
		ERR_MSG(g_hWnd, L"Loding �ε� ����" );
		return E_FAIL;
	}
	return S_OK;
}

SCENEID CLodingScene::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CLodingScene::Render()
{
	// �ð�
	static float fTime = 0.0f;
	// �ð��� ����
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(L"CastEffect",L"CastEffect",int(fTime));
	//
	//GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	//GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
	//	NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLodingScene::Release()
{
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
}
