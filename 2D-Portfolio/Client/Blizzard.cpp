#include "StdAfx.h"
#include "Blizzard.h"
#include "Include.h"

CBlizzard::CBlizzard(void)
{
}

CBlizzard::CBlizzard(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CSkill(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
}


CBlizzard::~CBlizzard(void)
{
}

HRESULT CBlizzard::Initialize()
{
	m_Info.vPos.x = WINSIZEX / 2;
	m_Info.vPos.y = WINSIZEY / 2;
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);

	m_vTagetInfo = m_Info.vPos;

	m_Info.fCX = 200;
	m_Info.fCY = 200;


	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMight = 5;
	m_sPlayInfo.fDexterity = 5;
	m_sPlayInfo.fIntellect = 5;
	m_sPlayInfo.fConstitution = 5;
	m_sPlayInfo.fResolve = 5;
	m_sPlayInfo.fPerception = 5;
	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel*20;
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 10 + m_sPlayInfo.fMight * 1;
	m_sPlayInfo.fDefence = 0;
	m_sPlayInfo.iSKillPoint = 0;
	m_sPlayInfo.iStatPoint = 0;
	m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX = 1;
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = 1;
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = 300;

	m_fChaterDirect = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_bRun = true;
	m_pStateKey = BLIZZARD;
	m_pMotion = BLIZZARD;
	m_fAngle = D3DXToRadian(280.f);
	m_pTagetObj = nullptr;

	m_fTime = 0.f;

	return S_OK;
}

SCENEID CBlizzard::Progress()
{
	m_dwKey = GET_SINGLE(CKeyMgr)->GetKey();
	// 시간

	if(m_dwKey & KEY_RBUTTON && m_fTime == 0)
		m_Info.vPos = MouseInfoDX();

	m_fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	if(m_sPlayInfo.fHealthPoint <= 0)
		m_pMotion = DEATH;
	if (m_fTime >= 3.5f)
		m_pMotion = DEATH;

	if (m_vTagetInfo == m_Info.vPos)
		m_vTagetInfo = m_Info.vPos;

	// 이미지 크기 변경해야함
	D3DXMatrixScaling(&m_Info.matScale, 1.f, 1.f, 1.f);

	// 위치를 마우스가 있는 위치로 놓는다.
	m_Info.vDir = m_Info.vPos;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, m_Info.vPos.z );

	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	DirectAction(m_pMotion);

	FrameStatas();
	return SCENEID_NONPASS;
}

void CBlizzard::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(SKILL, m_pStateKey, int(m_tFrame.fStart));
	// 프레임 값이 저장한 이미지 벡터크기를 벗어난 값이 들어가면 에러

	if(pTexInfo == NULL)
		return;
	for (int i = 0 ; i < 15 ; ++i)
	{
		//블리자드 이펙트를 원하는 갯수(i)만큼 그리면 됨, 난수값에 따라 (i)개를 랜덤하게 그려줌
		m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + (CObj::g_tScroll.x + (rand()%200 - 100)) ,
			(pTexInfo->ImgInfo.Height * 0.5) + (CObj::g_tScroll.y + (rand()%200 - 100)) , 0);

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CBlizzard::Release()
{

}

CObj* CBlizzard::Clone()
{
	return new CBlizzard(*this);
}

void CBlizzard::DirectAction(TCHAR* _pObjStatas)
{
	m_pStateKey = m_pMotion = _pObjStatas;
}

void CBlizzard::FrameStatas()
{
	FrameMove(PLAYER_Blizzard, PLAYER_Blizzard);
}
void CBlizzard::StateSkill()
{
	m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel*20;
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 50 + m_sPlayInfo.fMight * 10;
	m_sPlayInfo.iSKillPoint = 1;
	m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX = 1;
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.iLevel*1 * 0.5f);
}
