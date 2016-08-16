#include "StdAfx.h"
#include "BoneSpear.h"
#include "Include.h"

CBoneSpear::CBoneSpear(void)
{
}


CBoneSpear::~CBoneSpear(void)
{
}

HRESULT CBoneSpear::Initialize()
{
	//------------------------Skill--------------------------------//

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_D_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_D", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear D �ε� ����" , L"BoneSpear_D");
		return E_FAIL;
	}

	// ���� �Ʒ�
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_LD_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_LD", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear LD �ε� ����" ,L"BoneSpear_LD");
		return E_FAIL;
	}

	// ������ �Ʒ�
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_RD_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_RD", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear RD �ε� ����" , L"BoneSpear_RD");
		return E_FAIL;
	}

	// ��
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_U_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_U", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear U �ε� ����" , L"BoneSpear_U");
		return E_FAIL;
	}

	// ���� ��
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_LU_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_LU", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear LU �ε� ����" , L"BoneSpear_LU");
		return E_FAIL;
	}

	// ������ ��
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_RU_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_RU", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear RU �ε� ����" , L"BoneSpear_RU");
		return E_FAIL;
	}

	// ����
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_L_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_L", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear L �ε� ����" , L"BoneSpear_L");
		return E_FAIL;
	}

	// ������
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_R_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_R", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear R �ε� ����" , L"BoneSpear_R");
		return E_FAIL;
	}

	//-------------------------------------------------------------//
	return S_OK;
}

SCENEID CBoneSpear::Progress()
{
	return SCENEID_NONPASS;
}

void CBoneSpear::Render()
{

}

void CBoneSpear::Release()
{

}

CObj* CBoneSpear::Clone()
{
	return new CBoneSpear(*this);
}

POINT CBoneSpear::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x = (pt.x + CObj::g_tScroll.x);
	pt.y = (pt.y + CObj::g_tScroll.y);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CBoneSpear::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3((pt.x + CObj::g_tScroll.x) , ( pt.y + CObj::g_tScroll.y) , 0);
}