#include "StdAfx.h"
#include "StageObjProto.h"
#include "Include.h"

CStageObjProto::CStageObjProto(void)
{
}


CStageObjProto::~CStageObjProto(void)
{
}

HRESULT CStageObjProto::InitProtoInstance()
{
	// 아직 정해지지 않은 원형 객체의 기본 설정값(틀)
	OBJINFO objInfo;
	ZeroMemory(&objInfo, sizeof(OBJINFO));

	D3DXMatrixIdentity(&objInfo.matWorld);
	objInfo.vPos = D3DXVECTOR3(400.f, 300.f, 0.f);
	objInfo.vDir = D3DXVECTOR3(0, 0, 0);
	objInfo.vLook = D3DXVECTOR3(1, 0, 0);
	
	// 원형 객체를 생성해서 삽입해줌
	m_MapProto.insert(make_pair(L"StageBack"
		, new CStageBackGround(objInfo, OBJ_BUILDING)));
	
	m_MapProto.insert(make_pair(L"HP"
		, new CUIObj(objInfo, L"HP", OBJ_UI)));

	m_MapProto.insert(make_pair(L"HPBarBack"
		, new CUIObj(objInfo, L"HPBarBack", OBJ_UI)));

	m_MapProto.insert(make_pair(L"HPBar"
		, new CUIObj(objInfo, L"HPBar", OBJ_UI)));

	m_MapProto.insert(make_pair(L"MP"
		, new CUIObj(objInfo, L"MP", OBJ_UI)));

	m_MapProto.insert(make_pair(L"Stamina"
		, new CUIObj(objInfo, L"Stamina", OBJ_UI)));

	m_MapProto.insert(make_pair(L"Run"
		, new CUIObj(objInfo, L"Run", OBJ_UI)));

	m_MapProto.insert(make_pair(L"SkillPointTrue"
		, new CUIObj(objInfo, L"SkillPointTrue", OBJ_UI)));

	m_MapProto.insert(make_pair(L"SkillPointFalse"
		, new CUIObj(objInfo, L"SkillPointFalse", OBJ_UI)));

	m_MapProto.insert(make_pair(L"StatPointTrue"
		, new CUIObj(objInfo, L"StatPointTrue", OBJ_UI)));

	m_MapProto.insert(make_pair(L"StatPointFalse"
		, new CUIObj(objInfo, L"StatPointFalse", OBJ_UI)));

	m_MapProto.insert(make_pair(L"StatusBar"
		, new CUIObj(objInfo, L"StatusBar", OBJ_UI)));

	m_MapProto.insert(make_pair(L"ExpBar"
		, new CUIObj(objInfo, L"ExpBar", OBJ_UI)));

	m_MapProto.insert(make_pair(L"Inven"
		, new CUIObj(objInfo, L"Inven", OBJ_UI)));

	m_MapProto.insert(make_pair(L"SkillTree"
		, new CUIObj(objInfo, L"SkillTree", OBJ_UI)));

	m_MapProto.insert(make_pair(L"Bank"
		, new CUIObj(objInfo, L"Bank", OBJ_UI)));

	m_MapProto.insert(make_pair(L"Character"
		, new CUIObj(objInfo, L"Character", OBJ_UI)));


	//m_MapProto.insert(make_pair(L"ItemInfoRect"
	//	, new CUIObj(objInfo, L"ItemInfoRect", OBJ_UI)));
	//------------------------------------------------------------//
	// 플레이어
	m_MapProto.insert(make_pair(L"Player"
		, new CPlayer(objInfo, L"Player", OBJ_PLAYER)));
	
	// 동료 1
	objInfo.vPos = D3DXVECTOR3(450.f, 350.f, 0.f);
	m_MapProto.insert(make_pair(L"Fellow_First"
		, new CPlayer(objInfo, L"Fellow_First", OBJ_PLAYER)));

	// 동료 2
	objInfo.vPos = D3DXVECTOR3(350.f, 250.f, 0.f);
	m_MapProto.insert(make_pair(L"Fellow_Second"
		, new CPlayer(objInfo, L"Fellow_Second", OBJ_PLAYER)));


	
	// 스테이지 일반 몹
	// 1
	m_MapProto.insert(make_pair(L"Skeleton"
		, new CMonster(objInfo, L"Skeleton", OBJ_MONSTER)));
	// 2
	m_MapProto.insert(make_pair(L"SandRaider"
		, new CMonster(objInfo, L"SandRaider", OBJ_MONSTER)));
	// 3
	m_MapProto.insert(make_pair(L"Viper"
		, new CMonster(objInfo, L"Viper", OBJ_MONSTER)));

	// 스테이지 보스몹
	// 1
	m_MapProto.insert(make_pair(L"Griswold"
		, new CMonster(objInfo, L"Griswold", OBJ_MONSTER)));

	m_MapProto.insert(make_pair(L"Summoner"
		, new CMonster(objInfo, L"Summoner", OBJ_MONSTER)));
	// 2
	m_MapProto.insert(make_pair(L"Andariel"
		, new CMonster(objInfo, L"Andariel", OBJ_MONSTER)));

	m_MapProto.insert(make_pair(L"Izual"
		, new CMonster(objInfo, L"Izual", OBJ_MONSTER)));
	// 3
	m_MapProto.insert(make_pair(L"Diablo"
		, new CMonster(objInfo, L"Diablo", OBJ_MONSTER)));

	return S_OK;
}
