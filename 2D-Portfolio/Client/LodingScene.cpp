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
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Loding")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Loading/Loading_%02.png",
		L"Loding", TEXTYPE_MULTI, L"Loding", 10)))
	{
		ERR_MSG(g_hWnd, L"Loding 로드 실패" );
		return E_FAIL;
	}
	return S_OK;
}

SCENEID CLodingScene::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// 씬ID가 NONPASS가 아니면 씬매니져에서 해당ID의 씬을 생성 및 초기화
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CLodingScene::Render()
{
	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
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
