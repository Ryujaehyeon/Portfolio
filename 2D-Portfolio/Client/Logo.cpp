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
	// �ΰ� ���������� �ΰ� �ʿ��� ��ü�� ������ �����´�.
	m_pPrototype = new CLogoObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	// 
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Back")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Ball")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Logo/LogoBack.png"
		, L"Back", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Logo/Ball.png"
		, L"Ball", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}

	return S_OK;
}

SCENEID CLogo::Progress()
{
	//int iScene = GET_SINGLE(CObjMgr)->Progress();

	//// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	//if(iScene > -1)
	//	GET_SINGLE(CSceneMgr)->InitScene((SCENEID)iScene);


	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();
	
	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

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