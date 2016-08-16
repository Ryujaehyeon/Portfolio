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
		ERR_MSG(g_hWnd, L"%d BoneSpear D 로드 실패" , L"BoneSpear_D");
		return E_FAIL;
	}

	// 왼쪽 아래
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_LD_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_LD", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear LD 로드 실패" ,L"BoneSpear_LD");
		return E_FAIL;
	}

	// 오른쪽 아래
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_RD_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_RD", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear RD 로드 실패" , L"BoneSpear_RD");
		return E_FAIL;
	}

	// 위
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_U_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_U", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear U 로드 실패" , L"BoneSpear_U");
		return E_FAIL;
	}

	// 왼쪽 위
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_LU_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_LU", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear LU 로드 실패" , L"BoneSpear_LU");
		return E_FAIL;
	}

	// 오른쪽 위
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_RU_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_RU", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear RU 로드 실패" , L"BoneSpear_RU");
		return E_FAIL;
	}

	// 왼쪽
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_L_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_L", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear L 로드 실패" , L"BoneSpear_L");
		return E_FAIL;
	}

	// 오른쪽
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_R_%02d.png",
		ATTACKSKILL, TEXTYPE_MULTI, L"BoneSpear_R", PLAYER_BoneSpear_Missile)))
	{
		ERR_MSG(g_hWnd, L"%d BoneSpear R 로드 실패" , L"BoneSpear_R");
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