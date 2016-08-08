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
	// 아직 정해지지 않은 원형 객체의 기본 설정값(틀)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(OBJINFO));

	D3DXMatrixIdentity(&objInfo.matWorld);

	// 이미지 
	objInfo.vPos = D3DXVECTOR3(400, 300, 0);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	// 원형 객체를 생성해서 삽입해줌
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
