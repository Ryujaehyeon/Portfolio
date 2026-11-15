#include "StdAfx.h"
#include "LogoBackGround.h"
#include "Include.h"

CLogoBackGround::CLogoBackGround(void)
{
}

CLogoBackGround::CLogoBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:CLogoStatic(Info, _ObjType)
{

}

CLogoBackGround::~CLogoBackGround(void)
{
	Release();
}

HRESULT CLogoBackGround::Initialize()
{
	m_eLayer = LAYER_TERRAIN;
	m_pObjKey = L"Back";
	m_pStateKey = NULL;

	return S_OK;
}

SCENEID CLogoBackGround::Progress()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = matTrans;

	return SCENEID_NONPASS;
}

void CLogoBackGround::Render()
{
	const TEXINFO* pTexInfo = 
		GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);
	if(pTexInfo == NULL)
		return;
	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5f, 0.0f);

	GET_SINGLE(CRenderer)->DrawSprite(pTexInfo, nullptr,
		m_Info.vCenter, m_Info.matWorld, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLogoBackGround::Release()
{

}

CObj* CLogoBackGround::Clone()
{
	return new CLogoBackGround(*this);
}
