#include "StdAfx.h"
#include "UIObj.h"
#include "Include.h"

CUIObj::CUIObj(void)
{
}

CUIObj::CUIObj( const OBJINFO& Info, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType)
{

}

CUIObj::CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType)
{
	m_pObjKey = _ObjName;
}

CUIObj::~CUIObj(void)
{
	Release();
}

HRESULT CUIObj::Initialize()
{
	TCHAR* UI[6]=
	{
		L"StatusBar",
		L"HP",
		L"MP",
		L"Stamina",
		L"Run",
		L"SkillPoint"
	};

	if(m_pObjKey == UI[0])
		m_Info.vPos  = D3DXVECTOR3(512.f, 512.f, 0.f);
	if(m_pObjKey == UI[1])
		m_Info.vPos  = D3DXVECTOR3(80.f, 600.f, 0.f);
	if(m_pObjKey == UI[2])
		m_Info.vPos  = D3DXVECTOR3(750.f, 600.f, 0.f);
	if(m_pObjKey == UI[3])
		m_Info.vPos  = D3DXVECTOR3(315.f, 500.f, 0.f);
	if(m_pObjKey == UI[4])
		m_Info.vPos  = D3DXVECTOR3(260.f, 400.f, 0.f);
	if(m_pObjKey == UI[5])
		m_Info.vPos  = D3DXVECTOR3(570.f, 400.f, 0.f);

	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_pStateKey = NULL;
	m_eLayer = LAYER_UI;
	return S_OK;
}

SCENEID CUIObj::Progress()
{
	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;
	return SCENEID_NONPASS;
}

void CUIObj::Render()
{
	// 텍스쳐매니저를 통해 텍스쳐를 가져온다.
	// 이미 Initialize에서 정해진 오브젝트의 이름으로 구해온다.
	//TCHAR Info[128];
	//wsprintf(Info, L"X : %d Y : %d", int(m_Info.vPos.x), int(m_Info.vPos.y));
	//DebugMsg(Info);

	D3DXVECTOR3 Mouse = MouseInfoDX();

	TCHAR Info[128] = {};
	if(m_pObjKey == L"HP")
	wsprintf(Info, L"Mouse[X:%d Y:%d], vPos[X:%d Y:%d]",
			int(Mouse.x), int(Mouse.y),int(m_Info.vPos.x),int(m_Info.vPos.y));
	DebugMsg(Info);

	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	// 없으면 실패하여 리턴
	if(pTexInfo == NULL)
		return;

	// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5f, 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);

	// &GetRect() 그리는 이미지의 좌표
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CUIObj::Release()
{

}

CObj* CUIObj::Clone()
{
	return new CUIObj(*this);
}

POINT CUIObj::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CUIObj::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
