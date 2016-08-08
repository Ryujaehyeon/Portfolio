#include "StdAfx.h"
#include "MenuD2.h"
#include "Include.h"

CMenuD2::CMenuD2(void)
{
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
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// 씬ID가 NONPASS가 아니면 씬매니져에서 해당ID의 씬을 생성 및 초기화
	if(iScene > -1)
		GET_SINGLE(CSceneMgr)->InitScene((SCENEID)iScene);
	if (iScene == SCENEID_END)
		return SCENEID_END;
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
