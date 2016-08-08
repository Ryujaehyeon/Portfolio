#include "StdAfx.h"
#include "MenuBackGround.h"
#include "Include.h"

CMenuBackGround::CMenuBackGround(void)
{
}

CMenuBackGround::CMenuBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:CMenuStatic(Info, _ObjType)
{

}


CMenuBackGround::~CMenuBackGround(void)
{
}

HRESULT CMenuBackGround::Initialize()
{
	m_eLayer = LAYER_TERRAIN;
	m_pObjKey = L"MenuBack";
	m_pStateKey = NULL;

	return S_OK;
}

SCENEID CMenuBackGround::Progress()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = matTrans;

	return SCENEID_NONPASS;
}

void CMenuBackGround::Render()
{
	const TEXINFO* pTexInfo = 
		GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);
	if(pTexInfo == NULL)
		return;
	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5f, 0.0f);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMenuBackGround::Release()
{

}

CObj* CMenuBackGround::Clone()
{
	return new CMenuBackGround(*this);
}

POINT CMenuBackGround::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CMenuBackGround::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
