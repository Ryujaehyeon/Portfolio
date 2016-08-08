#include "StdAfx.h"
#include "LogoObjProto.h"
#include "Include.h"

CLogoObjProto::CLogoObjProto(void)
{
}

CLogoObjProto::~CLogoObjProto(void)
{
}


HRESULT CLogoObjProto::InitProtoInstance()
{
	// 아직 정해지지 않은 원형 객체의 기본 설정값(틀)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(objInfo));

	D3DXMatrixIdentity(&objInfo.matWorld);
	objInfo.vPos = D3DXVECTOR3(400.f, 300.f, 0);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	// 원형 객체를 생성해서 삽입해줌
	m_MapProto.insert(make_pair(L"Back"
		, new CLogoBackGround(objInfo, OBJ_BUILDING)));

	m_MapProto.insert(make_pair(L"Ball"
		, new CBall(objInfo, OBJ_BUTTON)));

	return S_OK;
}