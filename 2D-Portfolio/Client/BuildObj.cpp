#include "StdAfx.h"
#include "BuildObj.h"


CBuildObj::CBuildObj(void)
{
}

CBuildObj::CBuildObj(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:CStageStatic(Info, _ObjType)
{

}


CBuildObj::~CBuildObj(void)
{
	Release();
}

HRESULT CBuildObj::Initialize()
{
	return S_OK;
}

SCENEID CBuildObj::Progress()
{
	return SCENEID_NONPASS;
}

void CBuildObj::Render()
{

}

void CBuildObj::Release()
{

}

CObj* CBuildObj::Clone()
{
	return new CBuildObj(*this);
}

POINT CBuildObj::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CBuildObj::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
