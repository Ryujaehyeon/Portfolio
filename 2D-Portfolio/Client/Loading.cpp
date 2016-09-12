#include "StdAfx.h"
#include "Loading.h"
#include "Include.h"

Loading::Loading(void)
{
}

Loading::Loading( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:LoadingView(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
	m_pTagetList = NULL;
}


Loading::~Loading(void)
{
}

HRESULT Loading::Initialize()
{
	m_eLayer = LAYER_TERRAIN;
	m_pObjKey = L"Loading";
	m_pStateKey = L"Loading";

	return S_OK;
}

SCENEID Loading::Progress()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = matTrans;

	return SCENEID_NONPASS;
}

void Loading::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(L"Loading",L"Loading",int(m_tFrame.fStart));

	if(pTexInfo == NULL)
		return;

	m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f),
		(pTexInfo->ImgInfo.Height * 0.5), 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void Loading::Release()
{

}

CObj* Loading::Clone()
{
	return new Loading(*this);
}
