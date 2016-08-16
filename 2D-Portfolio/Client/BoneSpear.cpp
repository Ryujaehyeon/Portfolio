#include "StdAfx.h"
#include "BoneSpear.h"
#include "Include.h"

CBoneSpear::CBoneSpear(void)
{

}

CBoneSpear::CBoneSpear(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CSkill(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
}


CBoneSpear::~CBoneSpear(void)
{
}

HRESULT CBoneSpear::Initialize()
{
	m_Info.vPos.x = WINSIZEX / 2;
	m_Info.vPos.y = WINSIZEY / 2;
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);

	m_vTagetInfo = m_Info.vPos;

	m_Info.fCX = 40;
	m_Info.fCY = 40;

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
	m_sPlayInfo.fSpeed = 300.0f;

	m_fChaterDirect = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_bRun = true;
	m_pStateKey = L"Missile_D";
	m_pMotion = L"Missile";
	m_fAngle = D3DXToRadian(280.f);
	m_pTagetObj = nullptr;

	return S_OK;
}

SCENEID CBoneSpear::Progress()
{
	//m_Info.vPos.x += int(cosf((m_fAngle) * PI / 180.0f) * m_sPlayInfo.fSpeed);
	//m_Info.vPos.y += int(-sinf((m_fAngle) * PI / 180.0f) * m_sPlayInfo.fSpeed);

	m_vMousePos = MouseInfoDX();

	// 마우스를 바라보는 방향
	m_Info.vDir = m_vMousePos - m_Info.vPos; 
	// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다 
	m_fChaterDirect = m_iDegree;

	// 거리를 구한다.
	float fDistance = D3DXVec3Length(&m_Info.vDir);

	// 벡터 정규화
	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	// 자신의 위치와 마우스의 위치가 같지 않을때, 
	// 같으면 위를 보기 떄문 dir값이 0이되 90도인 위를 보기때문
	if(m_Info.vPos != m_vMousePos)
		// 각도값을 구한다
		m_fAngle = acosf(D3DXVec3Dot(&m_Info.vDir, &m_Info.vLook));

	// 내적을 구한다.
	if (m_vMousePos.y > m_Info.vPos.y)
	{
		m_fAngle = 2 * D3DX_PI - m_fAngle;
	}
	// 각도의 라디안 값을 디그리값(0~360)으로 변경
	m_iDegree = D3DXToDegree(m_fAngle);

	if (fDistance < 2.0f)
		m_Info.vPos = m_vTagetInfo;

    m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.008f);

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, m_Info.vPos.z );

	DirectAction(m_pMotion);

	FrameStatas();
	return SCENEID_NONPASS;
}

void CBoneSpear::Render()
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

void CBoneSpear::FrameStatas()
{
	FrameMove(PLAYER_BoneSpear_Missile, PLAYER_BoneSpear_Missile);
	//FrameMove(PLAYER_BoneSpear_Tail, PLAYER_BoneSpear_Tail);
}

void CBoneSpear::Release()
{

}

CObj* CBoneSpear::Clone()
{
	return new CBoneSpear(*this);
}


void CBoneSpear::DirectAction( TCHAR* _pObjStatas )
{
	// 0808 09:00 m_pMotion만 바꾸고 _pObjStatas바꾸지 않아서 에러
	m_pMotion = _pObjStatas;
	// 서있기
	if(_pObjStatas == L"Missile")
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Missile_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Missile_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Missile_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Missile_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Missile_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Missile_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Missile_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Missile_RU";
		}
	}
}
