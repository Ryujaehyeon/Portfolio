#include "StdAfx.h"
#include "Skill.h"
#include "Include.h"

CSkill::CSkill( void )
{

}

CSkill::CSkill(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CObj(Info, _ObjName, _ObjType)
{
}

CSkill::~CSkill(void)
{
}

HRESULT CSkill::Initialize()
{
	
	return S_OK;
}

SCENEID CSkill::Progress()
{
	return SCENEID_NONPASS;
}

void CSkill::Render()
{

}

void CSkill::Release()
{

}

CObj* CSkill::Clone()
{
	return new CSkill(*this);
}

POINT CSkill::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x = (pt.x + CObj::g_tScroll.x);
	pt.y = (pt.y + CObj::g_tScroll.y);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CSkill::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3((pt.x + CObj::g_tScroll.x) , ( pt.y + CObj::g_tScroll.y) , 0);
}