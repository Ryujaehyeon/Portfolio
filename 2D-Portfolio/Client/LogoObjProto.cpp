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
	// ���� �������� ���� ���� ��ü�� �⺻ ������(Ʋ)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(objInfo));

	D3DXMatrixIdentity(&objInfo.matWorld);
	objInfo.vPos = D3DXVECTOR3(400.f, 300.f, 0);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	// ���� ��ü�� �����ؼ� ��������
	m_MapProto.insert(make_pair(L"Back"
		, new CLogoBackGround(objInfo, OBJ_BUILDING)));

	m_MapProto.insert(make_pair(L"Ball"
		, new CBall(objInfo, OBJ_BUTTON)));

	return S_OK;
}