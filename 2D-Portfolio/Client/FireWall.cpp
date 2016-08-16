#include "StdAfx.h"
#include "FireWall.h"
#include "Include.h"

CFireWall::CFireWall(void)
{
}

CFireWall::CFireWall(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CSkill(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
}


CFireWall::~CFireWall(void)
{
}

HRESULT CFireWall::Initialize()
{
	m_Info.vPos.x = WINSIZEX / 2;
	m_Info.vPos.y = WINSIZEY / 2;
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);

	m_vTagetInfo = m_Info.vPos;

	m_Info.fCX = 10;
	m_Info.fCY = 10;

	m_sPlayInfo.iLevel = 0;
	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMight = 0;
	m_sPlayInfo.fDexterity = 0;
	m_sPlayInfo.fIntellect = 0;
	m_sPlayInfo.fConstitution = 0;
	m_sPlayInfo.fResolve = 0;
	m_sPlayInfo.fPerception = 0;
	m_sPlayInfo.fExp = m_sPlayInfo.fMaxExp = 0;
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 10 + m_sPlayInfo.fMight * 1;
	m_sPlayInfo.fDefence = 0;
	m_sPlayInfo.iSKillPoint = 1;
	m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX = 0;
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = 10;
	m_sPlayInfo.fSpeed = 100.0f;

	m_fChaterDirect = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_bRun = true;
	m_pStateKey = L"FireWall";
	m_pMotion = L"FireWall";
	m_fAngle = D3DXToRadian(280.f);
	m_pTagetObj = nullptr;

	return S_OK;
}

SCENEID CFireWall::Progress()
{
	DirectAction(m_pMotion);

	FrameStatas();
	return SCENEID_NONPASS;
}

void CFireWall::Render()
{

}

void CFireWall::Release()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// 프레임 값이 저장한 이미지 벡터크기를 벗어난 값이 들어가면 에러

	if(pTexInfo == NULL)
		return;

	m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f),
		(pTexInfo->ImgInfo.Height * 0.5), 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CObj* CFireWall::Clone()
{
	return new CFireWall(*this);
}

void CFireWall::DirectAction(TCHAR* _pObjStatas)
{
	m_pStateKey = m_pMotion = _pObjStatas;
}

void CFireWall::FrameStatas()
{
	FrameMove(PLAYER_FireWall, PLAYER_FireWall);
}
