#include "StdAfx.h"
#include "MenuD2.h"
#include "Include.h"

CMenuD2::CMenuD2(void)
{
	GameSound Sound;
	Sound->Init(40);
	Sound->LoadSound(L"Wilderness", L"Wilderness.mp3", true);
	Sound->SetVolume(10);
	Sound->Play(L"Wilderness");       //����
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
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuBack")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuButton_Start")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MenuButton_Exit")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuBack.png"
		, L"MenuBack", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuButtonStart.png"
		, L"MenuButton_Start", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Menu/MenuButtonEnd.png"
		, L"MenuButton_Exit", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}
	return S_OK;
}

SCENEID CMenuD2::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);
	TCHAR Info[128];
	wsprintf(Info, L"Scene : %d", iScene);
	DebugMsg(Info);
	if (iScene == SCENEID_END)
		return SCENEID_END;
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
