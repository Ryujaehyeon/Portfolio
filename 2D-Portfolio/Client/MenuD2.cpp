#include "StdAfx.h"
#include "MenuD2.h"
#include "Include.h"

CMenuD2::CMenuD2(void)
{
	GameSound Sound;
	Sound->Init(40);
	Sound->LoadSound(L"Wilderness", L"Wilderness.mp3", true);
	Sound->SetVolume(10);
	Sound->Play(L"Wilderness");       //사운드
}


CMenuD2::~CMenuD2(void)
{
	Release();
}

HRESULT CMenuD2::Initialize()
{
	m_pPrototype = new CMenuObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuBack")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuButton_Start")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuButton_Exit")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuBack.png"
		, L"MenuBack", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuButtonStart.png"
		, L"MenuButton_Start", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuButtonEnd.png"
		, L"MenuButton_Exit", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}
	return S_OK;
}

SCENEID CMenuD2::Progress()
{
	const SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// 다음 장면 교체는 CSceneMgr::Progress 가 한다(실행 중인 장면이 자신을 지우지 않게)
	if (iScene == SCENEID_END)
	{
		return SCENEID_END;
	}

	return iScene;
}

void CMenuD2::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CMenuD2::Release()
{
	
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
	SAFE_DELETE<CPrototype>(&m_pPrototype);
}
