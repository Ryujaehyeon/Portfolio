#include "StdAfx.h"
#include "Player.h"
#include "Include.h"

// 스모크 모드에서 BoneSpear 시전이 한 번 끝났는지
static bool s_bSmokeSkillCast = false;

CPlayer::CPlayer(void)
{
	// 프로토타입 원본은 Initialize 를 거치지 않으므로 소멸자의 SAFE_DELETE 가 쓰레기 값을 지우지 않게 한다
	m_pSkillPrototype = nullptr;
}

CPlayer::CPlayer( const OBJINFO& Info, const TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageDynamic(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
	m_pTagetList = NULL;
	m_pSkillPrototype = nullptr;
}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	m_pSkillPrototype = new CSkillProto;
	if(FAILED(m_pSkillPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	//-------BoneSpear------------------------------------------------------------//
	m_SkillTree.sBoneSpear.iLevel = 0;
	m_SkillTree.sBoneSpear.iMinLevel = 0;
	m_SkillTree.sBoneSpear.iLimitLevel = 99;
	m_SkillTree.sBoneSpear.fCastTime = 1.f;
	m_SkillTree.sBoneSpear.fHpCost = 0.f;
	m_SkillTree.sBoneSpear.fMpCost = 5;

	//-------FireWall-------------------------------------------------------------//
	m_SkillTree.sFireWall.iLevel = 0;
	m_SkillTree.sFireWall.iMinLevel = 3;
	m_SkillTree.sFireWall.iLimitLevel = 99;
	m_SkillTree.sBoneSpear.fCastTime = 1.f;
	m_SkillTree.sBoneSpear.fHpCost = 0.f;
	m_SkillTree.sBoneSpear.fMpCost = 10;

	//-------Blizzard-------------------------------------------------------------//
	m_SkillTree.sBlizzard.iLevel = 0;
	m_SkillTree.sBlizzard.iMinLevel = 5;
	m_SkillTree.sBlizzard.iLimitLevel = 99;
	m_SkillTree.sBoneSpear.fCastTime = 1.f;
	m_SkillTree.sBoneSpear.fHpCost = 0.f;
	m_SkillTree.sBoneSpear.fMpCost = 15;

	//----------------------------------------------------------------------------//
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_vTagetInfo = m_Info.vPos;
	m_Info.fCX = 50.f;
	m_Info.fCY = 75.f;
	//----------------------------------------------------------------------------//
	m_sPlayInfo.iLevel = 1;
	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMight = 5;
	m_sPlayInfo.fDexterity = 5;
	m_sPlayInfo.fIntellect = 5;
	m_sPlayInfo.fConstitution = 5;
	m_sPlayInfo.fResolve = 5;
	m_sPlayInfo.fPerception = 5;
	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMaxExp = static_cast<float>(m_sPlayInfo.iLevel*20);
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 10 + m_sPlayInfo.fMight * 1;
	m_sPlayInfo.fDefence = 5;
	m_sPlayInfo.iSKillPoint = 12;
	m_sPlayInfo.iStatPoint = 2;
	m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX = (m_sPlayInfo.fConstitution * 100);
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.fPerception * 100);
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = 150.0f;
	//----------------------------------------------------------------------------//
	// 기본적으로 캐릭터는 아래를 보고있게 설정.
	m_vTagetInfo.z = 0;
	m_fChaterDirect = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_bRun = true;
	m_pStateKey = L"Stand_D";
	m_pMotion = L"Stand";
	m_SkillActiveName = NULL;
	m_fAngle = D3DXToRadian(280.f);
	m_pTagetObj = nullptr;
	return S_OK;
}

SCENEID CPlayer::Progress()
{
	DebugLogClear;
	DebugLog(L"마우스 : %8.3f, %8.3f \n좌표 : %5.1f, %5.1f \n스크롤 : %5.1f, %5.1f", 
		m_vMousePos.x, m_vMousePos.y,
		m_Info.vPos.x, m_Info.vPos.y,
		CUIObj::g_tScroll.x,CUIObj::g_tScroll.y);

	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	StatesChange();
	// 입력받는 키를 체크해 상태를 변경
	CheckKey();

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


	ScrollChange();

	// 이동거리
	// Run 또는 Walk 상태일때만 
	if (IsSameKey(m_pMotion, RUN))
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.008f);
	else if (IsSameKey(m_pMotion, WALK))
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.005f);

	// 자신의 위치에서 목표지점거리보다 이동거리가 크면 자신의 위치를 목표지점으로 지정
	if (fDistance < 2.0f)
		m_Info.vPos = m_vTagetInfo;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, m_Info.vPos.z );

	// 크기 자전 이동 공전 부모
	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	// 캐릭터가 그려질 방향을 정해줌
	DirectAction(m_pMotion);

	//첫번째 인자값은 초당 몇프레임을 출력할건지, 두번째는 최대 장수
	FrameStatas();

	return SCENEID_NONPASS;
}

void CPlayer::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// 프레임 값이 저장한 이미지 벡터크기를 벗어난 값이 들어가면 에러

	if(pTexInfo == NULL)
		return;

	m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f)+CObj::g_tScroll.x,
		static_cast<float>((pTexInfo->ImgInfo.Height * 0.5)+CObj::g_tScroll.y), 0);

	//////////////////////////////////////////////////////////////////////////
	// 그림자
	if (!IsSameKey(m_pMotion, RUN))
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"Shadow",L"Shadow",0);

		if(pTexInfo == NULL)
			return;

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

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//////////////////////////////////////////////////////////////////////////

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (IsSameKey(m_pMotion, CAST))
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"CastEffect",L"CastEffect",int(m_tFrame.fStart));

		if(pTexInfo == NULL)
			return;

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}


void CPlayer::CheckKey()
{


	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 제어할 캐릭터 선택
	CharacterSelect();

	// 스킬 단축키
	SkillHotKey();

	// 키보드 입력
	(*CDevice::GetInstance())->SetKey();

	// 마우스 좌표 받아옴
	m_vMousePos = MouseInfoDX();

	// 키매니저로 키값 받아옴
	m_dwKey = GET_SINGLE(CKeyMgr)->GetKey();

	// 스모크 모드: 스테이지 2초 뒤부터 BoneSpear 가 한 번 생성될 때까지 시전한다(시전은 아래 우클릭 자리에서)
	bool bSmokeCast = false;
	if (IsSmokeMode() && !s_bSmokeSkillCast)
	{
		static const ULONGLONG s_ullSmokeStart = GetTickCount64();
		if (GetTickCount64() - s_ullSmokeStart >= 2000)
		{
			m_SkillTree.sBoneSpear.iLevel = 1;
			bSmokeCast = true;
		}
	}

	if (m_dwKey & KEY_TAB && m_bSelect == true && fTime > 3.0f)
	{
		fTime = 0;
		m_bRun = !m_bRun;
	}
	if((*CDevice::GetInstance())->GetMouseState()->rgbButtons[0] & 0x80 && m_bSelect == true) 
	{	
		// 우클릭시 클릭된 지점을 가져옴
		m_vTagetInfo = m_vMousePos;
		
		// 마우스를 바라보는 방향
		m_Info.vDir = m_vTagetInfo - m_Info.vPos; 
		// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다 
		m_fChaterDirect = m_iDegree;
	}
	else 
	{
		m_Info.vDir = m_vTagetInfo - m_Info.vPos; 
	}
	if ( m_bSelect == true )
	{
		// 캐릭터가 움직일 목표위치에 도달했는지
		if ( m_vTagetInfo == m_Info.vPos )
		{
			// 도달했을 시 서있는 상태
			m_pMotion = STAND;
			// 클릭 누르고 있을때
			if(m_dwKey & KEY_SPACE) 
			{
				FuncAttack();
			}
			if( (*CDevice::GetInstance())->GetMouseState()->rgbButtons[1] & 0x80)
			{
				// 마우스를 바라보는 방향
				m_Info.vDir = m_vMousePos - m_Info.vPos;
				// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다.
				m_fChaterDirect = m_iDegree;
				m_vTagetInfo = m_Info.vPos;
				SkillActive(m_SkillActiveName);
			}
			else if (bSmokeCast)
			{
				SkillActive(BONESPEAR);
			}
		}
		// 목표위치에 도달하지 못했을때
		else if ( m_vTagetInfo != m_Info.vPos )
		{
			if((*CDevice::GetInstance())->GetMouseState()->rgbButtons[0] & 0x80)
			{
				// 마우스를 바라보는 방향
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다.
				m_fChaterDirect = m_iDegree;
				// 취할 모션이미지를 바꿈
				m_pMotion = RUN;
				// 이동 중 공격시 이동을 멈춤
				m_vTagetInfo = m_Info.vPos;

			}
			else if((*CDevice::GetInstance())->GetMouseState()->rgbButtons[1] & 0x80)
			{
				// 마우스를 바라보는 방향
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다.
				m_fChaterDirect = m_iDegree;
				m_vTagetInfo = m_Info.vPos;
				SkillActive(m_SkillActiveName);
			}
			else if (bSmokeCast)
			{
				// 우클릭 시전과 같이 그 자리에 멈춰 시전한다 — 몬스터에 밀려도 시전 애니메이션이 이어지게(intent/smoke-stabilize)
				m_vTagetInfo = m_Info.vPos;
				SkillActive(BONESPEAR);
			}
			else
			{
				if (m_bRun == true)
					m_pMotion = RUN;
				else
					m_pMotion = WALK;
			}
		}
	}
	else if ( m_bSelect == false )
	{
		if ( m_vTagetInfo == m_Info.vPos )
		{
			// 도달했을 시 서있는 상태
			m_pMotion = STAND;
		}
		if ( m_vTagetInfo != m_Info.vPos )
		{
			if (m_bRun == true)
				m_pMotion = RUN;
			else
				m_pMotion = WALK;
		}
	}
	return;
}
void CPlayer::FuncAttack()
{
	DEBUG_ENTRY_EXIT;
	DEBUG_LINE;
	// 마우스를 바라보는 방향
	m_Info.vDir = m_vMousePos - m_Info.vPos; 
	// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다 
	m_fChaterDirect = m_iDegree;
	// 취할 모션이미지를 바꿈
	m_pMotion = ATTACK;

	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// 리스트 내에 충돌하고 마우스가 몬스터 위에 있을때
		const RECT rcTaget = (*iter)->RealRect();
		if (PtInRect(&rcTaget, MouseInfo())
			&& (*iter)->GetCrash() == true)
		{
			// 해당 몬스터의 객체를 타겟팅
			this->m_pTagetObj = (*iter);
			// 공격 모션 프레임이 끝날때 데미지가 적용
			if (m_tFrame.fStart >= PLAYER_ATTACK-0.5f)
			{
				// 몬스터 방어력이 플레이어 공격력보다 크면 데미지 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// 플레이어 공격력이 더 크면 몬스터 방어력을 뺀 나머지 만큼의 데미지만 적용 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack - (*iter)->GetStatas().fDefence;

				// 몬스터가 죽으면 타겟팅 해제
				if ((*iter)->GetStatas().fHealthPoint <= 0)
				{
					this->m_pTagetObj = nullptr;
					m_sPlayInfo.fExp += (*iter)->GetStatas().fExp;
				}
			}
		}
		else
			m_pMotion = ATTACK;
	}
}

void CPlayer::FrameStatas()
{
	if (IsSameKey(m_pMotion, ATTACK))
		FrameMove(PLAYER_ATTACK, PLAYER_ATTACK);
	else if(IsSameKey(m_pMotion, STAND))
		FrameMove(PLAYER_STAND, PLAYER_STAND);
	else if (IsSameKey(m_pMotion, WALK))
		FrameMove(PLAYER_WALK, PLAYER_WALK);
	else if (IsSameKey(m_pMotion, RUN))
		FrameMove(PLAYER_RUN, PLAYER_RUN);
	else if (IsSameKey(m_pMotion, CAST))
		FrameMove(PLAYER_CAST, PLAYER_CAST);
}

bool CPlayer::VecterInRect( D3DXVECTOR3& _Taget )
{
	POINT pt;
	pt.x = static_cast<LONG>(_Taget.x);
	pt.y = static_cast<LONG>(_Taget.y);
	
	const RECT rcReal = RealRect();
	return PtInRect(&rcReal, pt);
}

void CPlayer::DirectAction( const TCHAR* _pObjStatas )
{
	// 0808 09:00 m_pMotion만 바꾸고 _pObjStatas바꾸지 않아서 에러
	m_pMotion = _pObjStatas;
	// 서있기
	if(IsSameKey(_pObjStatas, STAND))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Stand_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Stand_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Stand_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Stand_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Stand_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Stand_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Stand_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Stand_RU";
		}
	}
	// 걷기
	if(IsSameKey(_pObjStatas, WALK))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Walk_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Walk_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Walk_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Walk_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Walk_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Walk_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Walk_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Walk_RU";
		}
	}
	else if (IsSameKey(_pObjStatas, RUN))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Run_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Run_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Run_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Run_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Run_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Run_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Run_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Run_RU";
		}
	}	
	else if (IsSameKey(_pObjStatas, ATTACK))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (IsSameKey(_pObjStatas, ATTACKSKILL))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (IsSameKey(_pObjStatas, CAST))
	{
		// 오른쪽 아래 RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Cast_RD";
		}
		// 아래 D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Cast_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Cast_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Cast_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Cast_LU";
		}
		// 위 U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Cast_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Cast_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Cast_RU";
		}
	}
}

void CPlayer::CharacterSelect()
{
	if (IsSameKey(m_pObjKey, PLAYER))
	{
		if(m_dwKey & KEY_F1)
			m_bSelect = true;
		if(m_dwKey & KEY_F2)
			m_bSelect = false;
		if(m_dwKey & KEY_F3)
			m_bSelect = false;
		if(m_dwKey & KEY_F4)
			m_bSelect = true;
		return;
	}
	else if (IsSameKey(m_pObjKey, FELLOW_1ST))
	{
		if(m_dwKey & KEY_F1)
			m_bSelect = false;
		if(m_dwKey & KEY_F2)
			m_bSelect = true;
		if(m_dwKey & KEY_F3)
			m_bSelect = false;
		if(m_dwKey & KEY_F4)
			m_bSelect = true;
		return;
	}
	else if (IsSameKey(m_pObjKey, FELLOW_2ND))
	{
		if(m_dwKey & KEY_F1)
			m_bSelect = false;
		if(m_dwKey & KEY_F2)
			m_bSelect = false;
		if(m_dwKey & KEY_F3)
			m_bSelect = true;
		if(m_dwKey & KEY_F4)
			m_bSelect = true;
		return;
	}
}

void CPlayer::Release()
{
	SAFE_DELETE<CPrototype>(&m_pSkillPrototype);
}

CObj* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::StatesChange()
{
	if (m_sPlayInfo.fHealthPoint < 0)
		m_sPlayInfo.fHealthPoint = 0;

	static float i = 0.f;
	i += GET_SINGLE(CTimeMgr)->DeltaTime();
	if ( i*10 > 1)
	{
		if (m_sPlayInfo.fHealthPoint < m_sPlayInfo.fHealthPointMAX)
			m_sPlayInfo.fHealthPoint += 0.5f;
		if (m_sPlayInfo.fMagikaPoint < m_sPlayInfo.fMagikaPointMAX)
			m_sPlayInfo.fMagikaPoint += 0.5f;
		i = 0.f;
	}

	if(m_sPlayInfo.fConstitution < 20)
		m_sPlayInfo.fHealthPointMAX = 
		(m_sPlayInfo.fConstitution * 100);
	else
		m_sPlayInfo.fHealthPointMAX = 
		(m_sPlayInfo.fConstitution * 100) *  
		( 1.f + (m_sPlayInfo.fConstitution-20 * 0.05f));

	// 레벨업에 필요한 경험치 달성 시
	if (m_sPlayInfo.iLevel < 100 && 
		m_sPlayInfo.fExp >= m_sPlayInfo.fMaxExp)
	{
		// 레벨업
		++m_sPlayInfo.iLevel;
		// 경험치
		m_sPlayInfo.fExp = 0;
		// 필요경험치
		m_sPlayInfo.fMaxExp = static_cast<float>((m_sPlayInfo.iLevel * 20) + ((m_sPlayInfo.iLevel * 20) * 0.1)); 
		// 스킬 포인트
		++m_sPlayInfo.iSKillPoint;
		// 스텟 포인트
		m_sPlayInfo.iStatPoint += 5;
		// 체력 초기화
		m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX;
		// 마력 초기화
		m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX;
	}
}

void CPlayer::ScrollChange()
{
	if(!IsSameKey(m_pMotion, ATTACK) && !IsSameKey(m_pMotion, CAST) && IsSameKey(m_pObjName, PLAYER))
	{
		m_Info.vPos;
		CObj::g_tScroll += m_Info.vDir;
		if(CObj::g_tScroll.x < 0)
			CObj::g_tScroll.x -= m_Info.vDir.x;
		if (CObj::g_tScroll.y < 0)
			CObj::g_tScroll.y -= m_Info.vDir.y;
		if(CObj::g_tScroll.x > 1735)
			CObj::g_tScroll.x -= m_Info.vDir.x;
		if (CObj::g_tScroll.y > 760)
			CObj::g_tScroll.y -= m_Info.vDir.y;
	}
}

void CPlayer::SkillActive(const TCHAR* _SkillName)
{
	DEBUG_ENTRY_EXIT;
	DEBUG_LINE;

	if(!IsSameKey(_SkillName, nullptr))
	{
		if( IsSameKey(_SkillName, BONESPEAR) && m_SkillTree.sBoneSpear.iLevel > 0 )
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				const HRESULT hr = GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, BONESPEAR);
				if (IsSmokeMode())
				{
					SmokeCheck(SUCCEEDED(hr), L"skill BoneSpear");
					s_bSmokeSkillCast = true;
				}
				if(FAILED(hr))
				{
					ERR_MSG(g_hWnd, L"BoneSpear 객체 생성 실패");
				}
			}
		}
		else if (IsSameKey(_SkillName, FIREWALL) && m_SkillTree.sFireWall.iLevel > 0 )
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, FIREWALL)))
				{
					ERR_MSG(g_hWnd, L"BoneSpear 객체 생성 실패");
				}
			}
		}
		else if (IsSameKey(_SkillName, BLIZZARD) && m_SkillTree.sBlizzard.iLevel > 0)
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, BLIZZARD)))
				{
					ERR_MSG(g_hWnd, L"BoneSpear 객체 생성 실패");
				}
			}
		}
		else FuncAttack();
	}
	else FuncAttack();
	
}

void CPlayer::SkillHotKey()
{
	if ( m_dwKey & KEY_1 )
		m_SkillActiveName = BONESPEAR;
	else if ( m_dwKey & KEY_2 )
		m_SkillActiveName = FIREWALL;
	else if ( m_dwKey & KEY_3 )
		m_SkillActiveName = BLIZZARD;
}
