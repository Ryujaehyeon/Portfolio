#include "StdAfx.h"
#include "Logo.h"
#include "Include.h"

CLogo::CLogo(void)
{}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::Initialize()
{
	// 로고 페이지에서 로고에 필요한 객체의 원형을 가져온다.
	m_pPrototype = new CLogoObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	// 
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Back")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Ball")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Logo/LogoBack.png"
		, L"Back", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Logo/Ball.png"
		, L"Ball", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}

	return S_OK;
}

SCENEID CLogo::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// 다음 장면 교체는 CSceneMgr::Progress 가 한다(실행 중인 장면이 자신을 지우지 않게)
	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
}

void CLogo::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CLogo::Release()
{
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
	SAFE_DELETE<CPrototype>(&m_pPrototype);
}