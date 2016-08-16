#include "StdAfx.h"
#include "SkillTree.h"


CSkillTree::CSkillTree(void)
{
}


CSkillTree::~CSkillTree(void)
{
}

HRESULT CSkillTree::Initialize()
{
	return S_OK;
}

SCENEID CSkillTree::Progress()
{
	return SCENEID_NONPASS;
}

void CSkillTree::Render()
{

}

void CSkillTree::Release()
{

}

CObj* CSkillTree::Clone()
{
	return new CSkillTree(*this);
}

POINT CSkillTree::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x = (pt.x + CObj::g_tScroll.x);
	pt.y = (pt.y + CObj::g_tScroll.y);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CSkillTree::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3((pt.x + CObj::g_tScroll.x) , ( pt.y + CObj::g_tScroll.y) , 0);
}