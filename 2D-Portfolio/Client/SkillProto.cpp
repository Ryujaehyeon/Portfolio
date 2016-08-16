#include "StdAfx.h"
#include "SkillProto.h"
#include "Include.h"

CSkillProto::CSkillProto(void)
{
}


CSkillProto::~CSkillProto(void)
{
}

HRESULT CSkillProto::InitProtoInstance()
{
	// 아직 정해지지 않은 원형 객체의 기본 설정값(틀)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(OBJINFO));

	D3DXMatrixIdentity(&objInfo.matWorld);
	objInfo.vPos = D3DXVECTOR3(400.f, 300.f, 0.f);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);

	
	m_MapProto.insert(make_pair(L"BoneSpear"
		, new CBoneSpear(objInfo, L"BoneSpear", OBJ_SKILL)));

	m_MapProto.insert(make_pair(L"FireWall"
		, new CFireWall(objInfo, L"FireWall", OBJ_SKILL)));

	m_MapProto.insert(make_pair(L"Blizzard"
		, new CBlizzard(objInfo, L"Blizzard", OBJ_SKILL)));

	m_MapProto.insert(make_pair(L"Heal"
		, new CBlizzard(objInfo, L"Heal", OBJ_SKILL)));

	m_MapProto.insert(make_pair(L"Aura"
		, new CBlizzard(objInfo, L"Aura", OBJ_SKILL)));

	m_MapProto.insert(make_pair(L"Rivive"
		, new CBlizzard(objInfo, L"Rivive", OBJ_SKILL)));

	return S_OK;
}
