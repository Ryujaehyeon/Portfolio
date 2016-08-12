#include "StdAfx.h"
#include "UIObj.h"
#include "Include.h"

CUIObj::CUIObj(void)
{
}

CUIObj::CUIObj( const OBJINFO& Info, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType),
	SelectCount(0)
{

}

CUIObj::CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType),
	SelectCount(0)
{
	m_pObjKey = _ObjName;
}

CUIObj::~CUIObj(void)
{
	Release();
}

HRESULT CUIObj::Initialize()
{
	TCHAR* UI[12]=
	{
		L"StatusBar",
		L"HP",
		L"HPBarBack",
		L"HPBar",
		L"MP",
		L"Stamina",
		L"Run",
		L"SkillPointTrue",
		L"SkillPointFalse",
		L"StatPointTrue",
		L"StatPointFalse",
		L"ExpBar"
	};

	if(m_pObjKey == UI[0])
	{
		m_Info.vPos  = D3DXVECTOR3(512.f, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[1])
	{
		m_Info.vPos  = D3DXVECTOR3(93.f, 635.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 128.f;
	}
	if(m_pObjKey == UI[2])
	{
		m_Info.vPos  = D3DXVECTOR3(400.f, 20.f, 0.f);
		m_Info.fCX = 100.f;
		m_Info.fCY = 15.f;
	}
	if(m_pObjKey == UI[3])
	{
		m_Info.vPos  = D3DXVECTOR3(400.f, 20.f, 0.f);
		m_Info.fCX = 100.f;
		m_Info.fCY = 15.f;
	}
	if(m_pObjKey == UI[4])
	{
		m_Info.vPos  = D3DXVECTOR3(755.f, 635.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 128.f;
	}
	if(m_pObjKey == UI[5])
	{	
		m_Info.vPos  = D3DXVECTOR3(336.f, 589.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 32.f;
	}
	if(m_pObjKey == UI[6])
	{	
		m_Info.vPos  = D3DXVECTOR3(263.f, 581.f, 0.f);
		m_Info.fCX = 32.f;
		m_Info.fCY = 32.f;
	}
	if(m_pObjKey == UI[7])
	{	
		m_Info.vPos  = D3DXVECTOR3(578.f, 578.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[8])	
	{	
		m_Info.vPos  = D3DXVECTOR3(578.f, 578.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[9])				 
	{	
		m_Info.vPos  = D3DXVECTOR3(221.f, 577.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[10])				 
	{	
		m_Info.vPos  = D3DXVECTOR3(221.f, 577.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[11])				 
	{
		m_Info.vPos  = D3DXVECTOR3(316, 564.f, 0.f);
		m_Info.fCX = 120.f;
		m_Info.fCY = 3.f;
	}

	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_pStateKey = NULL;
	m_eLayer = LAYER_UI;
	return S_OK;
}

SCENEID CUIObj::Progress()
{
	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;
	return SCENEID_NONPASS;
}

void CUIObj::Render()
{
	// �ؽ��ĸŴ����� ���� �ؽ��ĸ� �����´�.
	// �̹� Initialize���� ������ ������Ʈ�� �̸����� ���ؿ´�.
	D3DXVECTOR3 Mouse = MouseInfoDX();

	list<CObj*>::iterator iter = m_PlayerData->begin();
	for (iter; iter != m_PlayerData->end(); ++iter)
	{
		if((*iter)->GetSelect())
		{
			if(SelectCount < 3)
				++SelectCount;
			else
				SelectCount = 3;
			m_pTagetObj = (*iter);
		}
		else
			--SelectCount;
	}
	if (SelectCount == 3)
	{
		m_pTagetObj = (*m_PlayerData->begin());
	}
	
	//m_pTagetObj = (*m_PlayerData->begin());

	//DebugLogClear;
	
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	if (m_pObjKey == L"SkillPoint")
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, NULL, 1);
	}

	// ������ �����Ͽ� ����
	if(pTexInfo == NULL)
		return;

	// ���â���� �ش� �Լ��� ���ƴ��� Ȯ�� ����
	// DEBUG_ENTRY_EXIT;
	// �ش� ������ ��������
	// DEBUG_LINE;


	// �̹����� ũ�⸦ ������ �Ͽ� �߾Ӱ��� �����Ѵ�.
	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5f, 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);

	//// &GetRect() �׸��� �̹����� ��ǥ

	if(m_pObjKey == L"HP" ||m_pObjKey == L"MP" )
	{
		RECT rc = {
					float(0), 
					float(m_Info.fCY - (m_Info.fCY * VelueToPercentage(m_pObjKey))),
					float(m_Info.fCX), 
					float(m_Info.fCY)
					};

		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * VelueToPercentage(m_pObjKey), 0);

		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if ( m_pObjKey == L"ExpBar" )
	{
		RECT rc = {
			float(0), 
			float(0),
			float(m_Info.fCX * VelueToPercentage(m_pObjKey)), 
			float(m_Info.fCY)
		};
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height*0.5f, 0);
		
		

		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
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

void CUIObj::Setlist( list<CObj*>* _Player )
{
	m_PlayerData = _Player;
}

float CUIObj::VelueToPercentage(TCHAR* VelueName)
{
	if(L"HP" == VelueName)
		return m_pTagetObj->GetStatas().fHealthPoint/m_pTagetObj->GetStatas().fHealthPointMAX;

	if (L"MP" == VelueName)
		return m_pTagetObj->GetStatas().fMagikaPoint/m_pTagetObj->GetStatas().fMagikaPointMAX;

	if (L"ExpBar" == VelueName)
		return m_pTagetObj->GetStatas().fExp/m_pTagetObj->GetStatas().fMaxExp;
}

