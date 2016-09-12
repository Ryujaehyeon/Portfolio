#include "StdAfx.h"
#include "LoadingObjProto.h"
#include "Include.h"

LoadingObjProto::LoadingObjProto(void)
{
}


LoadingObjProto::~LoadingObjProto(void)
{
}

HRESULT LoadingObjProto::InitProtoInstance()
{
	// 아직 정해지지 않은 원형 객체의 기본 설정값(틀)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(objInfo));

	D3DXMatrixIdentity(&objInfo.matWorld);
	objInfo.vPos = D3DXVECTOR3(400.f, 300.f, 0);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	// 원형 객체를 생성해서 삽입해줌
	m_MapProto.insert(make_pair(L"Loading"
		, new Loading(objInfo, L"Loading", OBJ_BUILDING)));

	return S_OK;
}
