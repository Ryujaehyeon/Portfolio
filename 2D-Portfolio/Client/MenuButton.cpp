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
	// � ������Ʈ�� �̹����� �ҷ����� �����ϴ� ����
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

	// �ڽ��� ��ġ�� ����
	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);
	//< ũ�� * ���� * �̵� * ���� * �θ� >�ʿ信 ���� ���س��� �������� ���Ͽ� ���忡 �����Ѵ�.

	m_Info.matWorld =  matTrans;

	SCENEID iScene;
	// &RealRect() ���� �浹�ϴ� ��ǥ�� üũ
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
		// ��ư�� ���콺�� �ö��� ���� �̹����� �ٲ�
		m_bMouseOver = false;
		iScene = SCENEID_NONPASS;
	}

	// ���� �ѱ��� �ʴ´�.


	return iScene;

}

void CMenuButton::Render()
{
	// �ؽ��ĸŴ����� ���� �ؽ��ĸ� �����´�.
	// �̹� Initialize���� ������ ������Ʈ�� �̸����� ���ؿ´�.
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	// ������ �����Ͽ� ����
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

	// &GetRect() �׸��� �̹����� ��ǥ
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
