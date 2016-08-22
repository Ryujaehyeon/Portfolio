#include "StdAfx.h"
#include "Monster.h"
#include "Include.h"

CMonster::CMonster(void)
{
}

CMonster::CMonster( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageDynamic(Info, _ObjName, _ObjType)
{
		m_pObjKey = _ObjName;
		m_pObjName = _ObjName;
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Initialize()
{
	
	m_Info.vPos = D3DXVECTOR3((rand()%1600), (rand()%800), 0.f);
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_vTagetInfo = m_Info.vPos;
	m_Info.fCX = 50.f;
	m_Info.fCY = 75.f;

	m_sPlayInfo.iLevel = 1;
	m_sPlayInfo.fExp = m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel*10 + rand()%1000+1;
	m_sPlayInfo.fAttack = 20;
	m_sPlayInfo.fDefence = 5;
	m_sPlayInfo.fMight = 5;
	m_sPlayInfo.fDexterity = 5;
	m_sPlayInfo.fIntellect = 5;
	if(m_pObjName == SUMMONER)
		m_sPlayInfo.fConstitution = 700;
	else
		m_sPlayInfo.fConstitution = 50;

	m_sPlayInfo.fResolve = 5;
	m_sPlayInfo.fPerception = 5;
	m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX = (m_sPlayInfo.fConstitution * 10);
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.fPerception * 100);
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = (100.0f) + (m_sPlayInfo.fDexterity * 3.0f);

	// 기본적으로 캐릭터는 아래를 보고있게 설정.
	m_iDegree = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_vMovePoint = m_Info.vPos;
	m_pStateKey = L"Stand_D";
	m_pMotion = L"Stand";
	m_Crash = false;
	m_iDir = rand()%8;
	m_fWaitTime = 0.f;
	return S_OK;
}

SCENEID CMonster::Progress()
{
	
	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();
	
	//m_Info.vPos = m_Info.vPos + CObj::g_tScroll;

	RegenTime();

	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	// 입력받는 키를 체크해 상태를 변경
	CheckKey();

	// 거리를 구한다.
	float fDistance = D3DXVec3Length(&m_Info.vDir);

	// 벡터 정규화
	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	// 이동할 위치와 자신의 위치가 같으면 각도를 구하지 않음
	// 구하게 되면 위를 보거나 아래를 보게 만듬
	if (m_vTagetInfo != m_Info.vPos)
		// 각도값을 구한다
		m_fAngle = acosf(D3DXVec3Dot(&m_Info.vDir, &m_Info.vLook));

	// 내적을 구한다.
	if (m_vTagetInfo.y > m_Info.vPos.y)
	{
		m_fAngle = 2 * D3DX_PI - m_fAngle;
	}
	// 각도의 라디안 값을 디그리값(0~360)으로 변경
	m_iDegree = D3DXToDegree(m_fAngle);

	// 이동 방향이 정해지면 각도를 넣음
	m_fChaterDirect = m_iDegree;

	// 이동거리
	if ( m_pMotion == RUN && m_pMotion != CAST)
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.001f);

	// 자신의 위치에서 목표지점거리보다 이동거리가 크면 자신의 위치를 목표지점으로 지정
	if (fDistance < 1.0f && m_pMotion != DEATH)
		m_Info.vPos = m_vTagetInfo;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, 0.f);

	// 크기 자전 이동 공전 부모
	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	DirectAction(m_pMotion);

	//첫번째 인자값은 초당 몇프레임을 출력할건지, 두번째는 최대 장수
	FrameStatas();

	// 플레이어에게 죽을때 경험치를 줌
	ExpAcquired();

	return SCENEID_NONPASS;
}

void CMonster::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// 프레임 값이 저장한 이미지 벡터크기를 벗어난 값이 들어가면 에러

	if(pTexInfo == NULL)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// 그림자
	if (m_pMotion != RUN)
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"Shadow", L"Shadow", 0);
		
		if(pTexInfo == NULL)
			return;

		m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + CObj::g_tScroll.x,
			(pTexInfo->ImgInfo.Height * 0.5) + CObj::g_tScroll.y, 0);

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"Shadow",L"Shadow",int(m_tFrame.fStart));
		
		if(pTexInfo == NULL)
			return;

		m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + CObj::g_tScroll.x,
			(pTexInfo->ImgInfo.Height * 0.5) + CObj::g_tScroll.y, 0);

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//////////////////////////////////////////////////////////////////////////

	// 본체 
	{
		m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + CObj::g_tScroll.x,
			(pTexInfo->ImgInfo.Height * 0.5) + CObj::g_tScroll.y, 0);

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_pMotion == CAST)
	{
		{
			const TEXINFO* pTexInfo 
				= GET_SINGLE(CTextureMgr)->GetTexture(L"CastEffect",L"CastEffect",int(m_tFrame.fStart));

			if(pTexInfo == NULL)
				return;

			m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + CObj::g_tScroll.x,
				(pTexInfo->ImgInfo.Height * 0.5) + CObj::g_tScroll.y, 0);

			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		//{
		//	if(m_tFrame.fStart <= PLAYER_FireWall)
		//		const TEXINFO* pTexInfo 
		//		= GET_SINGLE(CTextureMgr)->GetTexture(SKILL, FIREWALL, int(m_tFrame.fStart));

		//	if(pTexInfo == NULL)
		//		return;

		//	m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f) + CObj::g_tScroll.x,
		//		(pTexInfo->ImgInfo.Height * 0.5) + CObj::g_tScroll.y, 0);

		//	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		//	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		//		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		//}
	}
}
void CMonster::CheckKey()
{
	// 제어할 캐릭터 선택
	static float fTime = 0.0f;
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 제어할 캐릭터 선택
	static float fTimeSkill = 0.0f;
	fTimeSkill += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 서있을때 몇초후 걷게할지
	
	m_fWaitTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 이동할 거리
	float moveLength = rand()%100+1;
	// 몇초에 한번 이동할지
	
	//
	if(m_sPlayInfo.fHealthPoint <= 0)
	{

		// 봐야할 적을 타겟으로 잡고
		Tageting();

		// 바라보는 방향
		if(m_pTagetObj != NULL)
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

		m_pMotion = DEATH;
		
		return;
	}
	// 3초에 한번 방향과 이동을 정함, 충돌하지 않았을때
	if(m_Crash == false)
	{
		// 서기
		if( m_pMotion == STAND) 
		{	
			// 이동할 지점을 가져옴
			m_vTagetInfo = m_vMovePoint;

			// 이동할 지점을 바라보는 방향
			m_Info.vDir = m_vTagetInfo - m_Info.vPos;

			if (m_fWaitTime > 3.f)
			{
				m_pMotion = RUN;
				m_fWaitTime = 0.f;
			}

			if(m_pMotion == RUN && m_vTagetInfo == m_Info.vPos)
			{
				m_iDir = rand()%8;
				switch(m_iDir)
				{
					//  방향을 정함
				case 0:
					// RD
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y + moveLength, 0);
					break;												   
				case 1:	
					// RU
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y - moveLength, 0);
					break;												   
				case 2:	
					// LD
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y + moveLength, 0);
					break;													 		
				case 3:	
					// LU
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y - moveLength, 0);
					break;
				case 4:	
					// L
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y, 0);
					break;
				case 5:	
					// R
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y, 0);
					break;
				case 6:	
					// U
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y - moveLength, 0);
					break;
				case 7:	
					// D
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y + moveLength, 0);
					break;
				}
			}
		}
		// 걷기
		if( m_pMotion == RUN )
		{
			m_vTagetInfo = m_vMovePoint;
			m_Info.vDir = m_vTagetInfo - m_Info.vPos;
			if ( m_vTagetInfo == m_Info.vPos )
			{
				// 도달했을 시 서있는 상태
				m_pMotion = STAND;
			}
		}
		// 공격중
		if (m_pMotion == ATTACK)
		{
			m_pMotion = STAND;
		}
	}
	else if( m_Crash == true )
	{
		// 충돌했을때
		if(int(m_sPlayInfo.fHealthPoint) <= 0)
			m_pMotion = DEATH;
		
		// 바라보는 방향
		//m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;
		Tageting();
		float fDistance = (m_pTagetObj->Setinfo()->fCX * 0.5f) +
						  (m_Info.fCX * 0.5f);

		float fRealDistance = D3DXVec3Length(&m_Info.vDir);

		if (m_pObjName == SUMMONER)
		{
			// 거리가 기본공격 사거리보다 멀고 3배의 거리보다 가까우면 스킬사용
			if(fDistance * 1.2f < fRealDistance && fDistance * 5.f > fRealDistance && m_pMotion != DEATH)
			{
					fTimeSkill = 0.f;
					m_pMotion = CAST;
					MonSKill();
			}
			// 거리가 기본공격 사거리보다 가까우면
			else if (fDistance * 1.2f > fRealDistance)
			{
				m_pMotion = ATTACK;
				FuncAttack();
			}
			else if(fDistance * 5.0f < fRealDistance)
			{
				// 기본공격 사거리인 충돌거리보다 멀면 쫓아간다.
				m_pMotion = RUN;
			}
		}
		// 거리에 따라 다른 객체와 충돌했을 때 공격(자신의 크기보다 1.2배는 공격 사거리)
		else if(fDistance * 1.2f > fRealDistance && m_pMotion != DEATH)
		{
			m_pMotion = ATTACK;
			FuncAttack();
		}
		else if(fDistance < fRealDistance)
		{
			// 기본공격 사거리인 충돌거리보다 멀면 쫓아간다.
			m_pMotion = RUN;
		}
		

		if(fDistance * 5.f < fRealDistance)
		{
			// 충돌거리의 5배의 거리보다 크면 충돌상태를 푼다
			if(int(m_tFrame.fStart) < 0.f )
				m_pMotion = STAND;
			m_Crash = false;
			m_pTagetObj = NULL;
		}
		if(m_pMotion == RUN && m_fWaitTime > 5.f)
		{
			// 쫓아가는 상태에서 5초 이상 지나면 충돌상태를 푼다
			m_fWaitTime = 0.f;
			if(int(m_tFrame.fStart) < 0.f )
				m_pMotion = STAND;
			m_Crash = false;
			m_pTagetObj = NULL;
		}
	}

	if (m_Info.vPos.x <= 30.f || m_Info.vPos.x >= 1730 &&
		m_Info.vPos.y <= 30.f || m_Info.vPos.y >= 1200)
	{
		m_pMotion =STAND;
		m_vTagetInfo == m_Info.vPos;
	}
}

void CMonster::MonSKill()
{
	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// 리스트 내에 충돌한 녀석이 있을때
		if ((*iter)->GetCrash() == true)
		{
			// 해당 객체를 타겟팅
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// 타겟을 보고 공격하게 방향을 정해줌
			m_vTagetInfo = m_pTagetObj->GetInfo().vPos;

			// 공격 모션 프레임이 끝날때 데미지가 적용
			if (m_tFrame.fStart <= 0.f)
			{
				// 몬스터 방어력이 플레이어 공격력보다 크면 데미지 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// 플레이어 공격력이 더 크면 몬스터 방어력을 뺀 나머지 만큼의 데미지만 적용 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack*1.5f - (*iter)->GetStatas().fDefence;

				// 대상이 죽으면 타겟팅 해제
				if ((*iter)->GetStatas().fHealthPoint <= 0)
				{
					m_pTagetObj = nullptr;
					m_sPlayInfo.fExp += (*iter)->GetStatas().fExp;
				}
			}
		}
	}
}


void CMonster::FuncAttack()
{
	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// 리스트 내에 충돌한 녀석이 있을때
		if ((*iter)->GetCrash() == true)
		{
			// 해당 몬스터의 객체를 타겟팅
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// 타겟을 보고 공격하게 방향을 정해줌
			m_vTagetInfo = m_pTagetObj->GetInfo().vPos;

			// 공격 모션 프레임이 끝날때 데미지가 적용
			if (m_tFrame.fStart <= 0.f)
			{
				// 몬스터 방어력이 플레이어 공격력보다 크면 데미지 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// 플레이어 공격력이 더 크면 몬스터 방어력을 뺀 나머지 만큼의 데미지만 적용 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack - (*iter)->GetStatas().fDefence;

				// 대상이 죽으면 타겟팅 해제
				if ((*iter)->GetStatas().fHealthPoint <= 0)
				{
					m_pTagetObj = nullptr;
					m_sPlayInfo.fExp += (*iter)->GetStatas().fExp;
				}
			}
		}
	}
}

void CMonster::RegenTime()
{
	if (m_pMotion == DEATH)
		return;

	static float i = 0.f;
	i += GET_SINGLE(CTimeMgr)->DeltaTime();
	if ( i > 1)
	{
		if (m_sPlayInfo.fHealthPoint < m_sPlayInfo.fHealthPointMAX)
			m_sPlayInfo.fHealthPoint += 0.1f;
		if (m_sPlayInfo.fMagikaPoint < m_sPlayInfo.fMagikaPointMAX)
			m_sPlayInfo.fMagikaPoint += 0.1f;
		i = 0.f;
	}
}

void CMonster::Release()
{

}

CObj* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::FrameStatas()
{
	if(m_pObjName == SKELETON)
	{
		if (m_pMotion == ATTACK)
			FrameMove(SKELETON_ATTACK, SKELETON_ATTACK);
		else if(m_pMotion == STAND)
			FrameMove(SKELETON_STAND, SKELETON_STAND);
		else if (m_pMotion == RUN)
			FrameMove(SKELETON_RUN, SKELETON_RUN);
		else if (m_pMotion == DEATH)
			FrameMove(SKELETON_DEATH, SKELETON_DEATH);
	}
	else if(m_pObjName == SUMMONER)
	{
		if (m_pMotion == ATTACK)
			FrameMove(SUMMONER_ATTACK, SUMMONER_ATTACK);
		else if(m_pMotion == STAND)
			FrameMove(SUMMONER_STAND, SUMMONER_STAND);
		else if (m_pMotion == RUN)
			FrameMove(SUMMONER_RUN, SUMMONER_RUN);
		else if (m_pMotion == DEATH)
			FrameMove(SUMMONER_DEATH, SUMMONER_DEATH);
		else if (m_pMotion == CAST)
			FrameMove(SUMMONER_CAST, SUMMONER_CAST);
	}
}

bool CMonster::VecterInRect( D3DXVECTOR3& _Taget )
{
	POINT pt;
	pt.x = _Taget.x;
	pt.y = _Taget.y;

	return PtInRect(&RealRect(), pt);
}

void CMonster::DirectAction( TCHAR* _pObjStatas )
{
	m_pMotion = _pObjStatas;
	if(_pObjStatas == STAND)
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Stand_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Stand_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Stand_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Stand_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Stand_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Stand_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Stand_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Stand_RU";
		}
	}
	else if (_pObjStatas == RUN)
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Run_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Run_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Run_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Run_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Run_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Run_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Run_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Run_RU";
		}
	}	
	else if (_pObjStatas == ATTACK)
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == ATTACKSKILL)
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == DEATH)
	{
		// 오른쪽 아래 RD
		{
			m_pStateKey = L"Death_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Death_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Death_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Death_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Death_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Death_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Death_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Death_RU";
		}
	}
	else if (_pObjStatas == CAST)
	{
		// 오른쪽 아래 RD
		{
			m_pStateKey = L"Cast_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Cast_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Cast_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Cast_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Cast_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Cast_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Cast_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Cast_RU";
		}
	}
}


void CMonster::Tageting()
{
	// 봐야할 적을 타겟으로 잡고
	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// 리스트 내에 충돌하고 마우스가 몬스터 위에 있을때
		if ((*iter)->GetCrash() == true)
		{
			// 해당 몬스터의 객체를 타겟팅
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// 타겟을 보고 공격하게 방향을 정해줌
			m_vTagetInfo = m_pTagetObj->GetInfo().vPos;
		}
		else 
			m_pTagetObj = this;
	}
}

void CMonster::ExpAcquired()
{
	if(m_pMotion == DEATH && m_tFrame.fStart <= 0)
	{
		for (list<CObj*>::iterator iter = m_pTagetList->begin();
			iter != m_pTagetList->end(); ++iter)
		{
			(*iter)->SetStatas()->fExp += m_sPlayInfo.fExp;
		}
	}
}
