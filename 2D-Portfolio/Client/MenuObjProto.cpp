#include "StdAfx.h"
#include "MenuObjProto.h"
#include "Include.h"

CMenuObjProto::CMenuObjProto(void)
{
}


CMenuObjProto::~CMenuObjProto(void)
{
}

HRESULT CMenuObjProto::InitProtoInstance()
{
	// ���� �������� ���� ���� ��ü�� �⺻ ������(Ʋ)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(OBJINFO));

	D3DXMatrixIdentity(&objInfo.matWorld);

	// �̹��� 
	objInfo.vPos = D3DXVECTOR3(400, 300, 0);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	// ���� ��ü�� �����ؼ� ��������
	m_MapProto.insert(make_pair(L"MenuBack"
		, new CMenuBackGround(objInfo, OBJ_BUILDING)));

	objInfo.vPos = D3DXVECTOR3(400, 227, 0);
	m_MapProto.insert(make_pair(L"MenuButton_Start"
		, new CMenuButton(objInfo,L"MenuButton_Start", OBJ_BUTTON)));


	objInfo.vPos = D3DXVECTOR3(400, 550, 0);
	m_MapProto.insert(make_pair(L"MenuButton_Exit"
		, new CMenuButton(objInfo,L"MenuButton_Exit", OBJ_BUTTON)));

	return S_OK;
}
