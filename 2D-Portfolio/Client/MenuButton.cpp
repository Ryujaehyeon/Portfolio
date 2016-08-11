#include "StdAfx.h"
#include "MenuButton.h"
#include "Include.h"

CMenuButton::CMenuButton(void)
{
}

CMenuButton::CMenuButton(const OBJINFO& Info, TCHAR* ButtonName, const OBJ_TYPE _ObjType)
	:CMenuDynamic(Info, _ObjType),
	m_DrawID(ButtonName)
{
}



CMenuButton::~CMenuButton(void)
{ Release(); }

HRESULT CMenuButton::Initialize()
{
	// 어떤 오브젝트와 이미지를 불러올지 결정하는 값들
	m_eLayer = LAYER_OBJECT;
	m_pObjKey = m_DrawID;
	m_pStateKey = NULL;
	m_Info.fCX = 270;
	m_Info.fCY = 35;
	m_bMouseOver = false;
	return S_OK;
}

SCENEID CMenuButton::Progress()
{
	D3DXMATRIX matTrans;

	// 자신의 위치를 지정
	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);
	//< 크기 * 자전 * 이동 * 공전 * 부모 >필요에 의해 구해놓은 값끼리만 곱하여 월드에 대입한다.

	m_Info.matWorld =  matTrans;

	SCENEID iScene;
	// &RealRect() 실제 충돌하는 좌표를 체크
	if(PtInRect(&RealRect(), MouseInfo()))
	{
		m_bMouseOver = true;
		if ( m_pObjKey == L"MenuButton_Start" && 
			GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			iScene = SCENEID_STAGE;
			return  SCENEID_STAGE;
		}
		else 
			 iScene = SCENEID_NONPASS;
		if (m_pObjKey == L"MenuButton_Exit"  && 
			GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			iScene = SCENEID_END;
			return  SCENEID_END;
		}
	}
	else
	{
		// 버튼을 마우스가 올라가지 않은 이미지로 바꿈
		m_bMouseOver = false;
		iScene = SCENEID_NONPASS;
	}

	// 씬을 넘기지 않는다.


	return iScene;

}

void CMenuButton::Render()
{
	// 텍스쳐매니저를 통해 텍스쳐를 가져온다.
	// 이미 Initialize에서 정해진 오브젝트의 이름으로 구해온다.
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	// 없으면 실패하여 리턴
	if(pTexInfo == NULL)
		return;

	if (m_pObjKey == L"MenuButton_Start")
	{
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.25f, 0);
	}
	else if (m_pObjKey == L"MenuButton_Exit")
	{
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.25f, 0);
	}
	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);

	// &GetRect() 그리는 이미지의 좌표
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		&GetRect(), &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMenuButton::Release()
{

}

CObj* CMenuButton::Clone()
{
	return new CMenuButton(*this);
}

POINT CMenuButton::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CMenuButton::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
