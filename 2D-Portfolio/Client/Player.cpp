#include "StdAfx.h"
#include "Player.h"
#include "Include.h"

CPlayer::CPlayer(void)
{
}

CPlayer::CPlayer( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageDynamic(Info, _ObjName, _ObjType)
{
	m_pObjKey = _ObjName;
	m_pObjName = _ObjName;
	m_pTagetList = NULL;
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
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
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
	m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel*20;
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 10 + m_sPlayInfo.fMight * 1;
	m_sPlayInfo.fDefence = 5;
	m_sPlayInfo.iSKillPoint = 100;
	m_sPlayInfo.iStatPoint = 100;
	m_sPlayInfo.fHealthPoint = 250;
	m_sPlayInfo.fHealthPointMAX = (m_sPlayInfo.fConstitution * 100);
	m_sPlayInfo.fMagikaPoint = 250;
	m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.fPerception * 100);
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = 150.0f;
	//----------------------------------------------------------------------------//
	// �⺻������ ĳ���ʹ� �Ʒ��� �����ְ� ����.
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
	DebugLog(L"���콺 : %8.3f, %8.3f \n��ǥ : %5.1f, %5.1f \n��ũ�� : %5.1f, %5.1f", 
		m_vMousePos.x, m_vMousePos.y,
		m_Info.vPos.x, m_Info.vPos.y,
		CUIObj::g_tScroll.x,CUIObj::g_tScroll.y);

	// �ð�
	static float fTime = 0.0f;
	// �ð��� ����
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	StatesChange();
	// �Է¹޴� Ű�� üũ�� ���¸� ����
	CheckKey();

	// �Ÿ��� ���Ѵ�.
	float fDistance = D3DXVec3Length(&m_Info.vDir);

	// ���� ����ȭ
	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);
	
	// �ڽ��� ��ġ�� ���콺�� ��ġ�� ���� ������, 
	// ������ ���� ���� ���� dir���� 0�̵� 90���� ���� ���⶧��
	if(m_Info.vPos != m_vMousePos)
		// �������� ���Ѵ�
		m_fAngle = acosf(D3DXVec3Dot(&m_Info.vDir, &m_Info.vLook));

	// ������ ���Ѵ�.
	if (m_vMousePos.y > m_Info.vPos.y)
	{
		m_fAngle = 2 * D3DX_PI - m_fAngle;
	}
	// ������ ���� ���� ��׸���(0~360)���� ����
	m_iDegree = D3DXToDegree(m_fAngle);


	ScrollChange();

	// �̵��Ÿ�
	// Run �Ǵ� Walk �����϶��� 
	if (m_pMotion == RUN)
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.008f);
	else if (m_pMotion == WALK)
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.005f);

	// �ڽ��� ��ġ���� ��ǥ�����Ÿ����� �̵��Ÿ��� ũ�� �ڽ��� ��ġ�� ��ǥ�������� ����
	if (fDistance < 2.0f)
		m_Info.vPos = m_vTagetInfo;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, m_Info.vPos.z );

	// ũ�� ���� �̵� ���� �θ�
	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	// ĳ���Ͱ� �׷��� ������ ������
	DirectAction(m_pMotion);

	//ù��° ���ڰ��� �ʴ� ���������� ����Ұ���, �ι�°�� �ִ� ���
	FrameStatas();

	return SCENEID_NONPASS;
}

void CPlayer::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// ������ ���� ������ �̹��� ����ũ�⸦ ��� ���� ���� ����

	if(pTexInfo == NULL)
		return;

	m_Info.vCenter = D3DXVECTOR3((pTexInfo->ImgInfo.Width * 0.5f)+CObj::g_tScroll.x,
		(pTexInfo->ImgInfo.Height * 0.5)+CObj::g_tScroll.y, 0);

	//////////////////////////////////////////////////////////////////////////
	// �׸���
	if (m_pMotion != RUN)
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

	if (m_pMotion == CAST)
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


	// �ð�
	static float fTime = 0.0f;
	// �ð��� ����
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// ������ ĳ���� ����
	CharacterSelect();

	// ��ų ����Ű
	SkillHotKey();

	// ���콺 ��ǥ �޾ƿ�
	m_vMousePos = MouseInfoDX();

	// Ű�Ŵ����� Ű�� �޾ƿ�
	m_dwKey = GET_SINGLE(CKeyMgr)->GetKey();


	if (m_dwKey & KEY_TAB && m_bSelect == true && fTime > 3.0f)
	{
		fTime = 0;
		m_bRun = !m_bRun;
	}
	if( m_dwKey & KEY_LBUTTON && m_bSelect == true) 
	{	
		// ��Ŭ���� Ŭ���� ������ ������
		m_vTagetInfo = m_vMousePos;
		// ���콺�� �ٶ󺸴� ����
		m_Info.vDir = m_vTagetInfo - m_Info.vPos; 
		// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´� 
		m_fChaterDirect = m_iDegree;
	}
	else 
	{
		m_Info.vDir = m_vTagetInfo - m_Info.vPos; 
	}
	if ( m_bSelect == true )
	{
		// ĳ���Ͱ� ������ ��ǥ��ġ�� �����ߴ���
		if ( m_vTagetInfo == m_Info.vPos )
		{
			// �������� �� ���ִ� ����
			m_pMotion = STAND;
			// Ŭ�� ������ ������
			if(m_dwKey & KEY_SPACE) 
			{
				FuncAttack();
			}
			if(  m_dwKey & KEY_RBUTTON )
			{
				// ���콺�� �ٶ󺸴� ����
				m_Info.vDir = m_vMousePos - m_Info.vPos;
				// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´�.
				m_fChaterDirect = m_iDegree;
				m_vTagetInfo = m_Info.vPos;
				SkillActive(m_SkillActiveName);
			}
		}
		// ��ǥ��ġ�� �������� ��������
		else if ( m_vTagetInfo != m_Info.vPos )
		{
			if( m_dwKey & KEY_LBUTTON ) 
			{
				// ���콺�� �ٶ󺸴� ����
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´�.
				m_fChaterDirect = m_iDegree;
				// ���� ����̹����� �ٲ�
				m_pMotion = RUN;
				// �̵� �� ���ݽ� �̵��� ����
				m_vTagetInfo = m_Info.vPos;

			}
			else if(m_dwKey & KEY_RBUTTON )
			{
				// ���콺�� �ٶ󺸴� ����
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´�.
				m_fChaterDirect = m_iDegree;
				m_vTagetInfo = m_Info.vPos;
				SkillActive(m_SkillActiveName);
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
			// �������� �� ���ִ� ����
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
	// ���콺�� �ٶ󺸴� ����
	m_Info.vDir = m_vMousePos - m_Info.vPos; 
	// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´� 
	m_fChaterDirect = m_iDegree;
	// ���� ����̹����� �ٲ�
	m_pMotion = ATTACK;

	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// ����Ʈ ���� �浹�ϰ� ���콺�� ���� ���� ������
		if (PtInRect(&(*iter)->RealRect(), MouseInfo())
			&& (*iter)->GetCrash() == true)
		{
			// �ش� ������ ��ü�� Ÿ����
			this->m_pTagetObj = (*iter);
			// ���� ��� �������� ������ �������� ����
			if (m_tFrame.fStart >= PLAYER_ATTACK-0.5f)
			{
				// ���� ������ �÷��̾� ���ݷº��� ũ�� ������ 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// �÷��̾� ���ݷ��� �� ũ�� ���� ������ �� ������ ��ŭ�� �������� ���� 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack - (*iter)->GetStatas().fDefence;

				// ���Ͱ� ������ Ÿ���� ����
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
	if (m_pMotion == ATTACK)
		FrameMove(PLAYER_ATTACK, PLAYER_ATTACK);
	else if(m_pMotion == STAND)
		FrameMove(PLAYER_STAND, PLAYER_STAND);
	else if (m_pMotion == WALK)
		FrameMove(PLAYER_WALK, PLAYER_WALK);
	else if (m_pMotion == RUN)
		FrameMove(PLAYER_RUN, PLAYER_RUN);
	else if (m_pMotion == CAST)
		FrameMove(PLAYER_CAST, PLAYER_CAST);
}

bool CPlayer::VecterInRect( D3DXVECTOR3& _Taget )
{
	POINT pt;
	pt.x = _Taget.x;
	pt.y = _Taget.y;
	
	return PtInRect(&RealRect(), pt);
}

void CPlayer::DirectAction( TCHAR* _pObjStatas )
{
	// 0808 09:00 m_pMotion�� �ٲٰ� _pObjStatas�ٲ��� �ʾƼ� ����
	m_pMotion = _pObjStatas;
	// ���ֱ�
	if(_pObjStatas == STAND)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Stand_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Stand_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Stand_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Stand_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Stand_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Stand_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Stand_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Stand_RU";
		}
	}
	// �ȱ�
	if(_pObjStatas == WALK)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Walk_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Walk_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Walk_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Walk_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Walk_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Walk_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Walk_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Walk_RU";
		}
	}
	else if (_pObjStatas == RUN)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Run_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Run_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Run_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Run_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Run_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Run_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Run_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Run_RU";
		}
	}	
	else if (_pObjStatas == ATTACK)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == ATTACKSKILL)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == CAST)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"Cast_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"Cast_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"Cast_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"Cast_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"Cast_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"Cast_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"Cast_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"Cast_RU";
		}
	}
}

void CPlayer::CharacterSelect()
{
	if (m_pObjKey == PLAYER)
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
	else if (m_pObjKey == FELLOW_1ST)
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
	else if (m_pObjKey == FELLOW_2ND)
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

	// �������� �ʿ��� ����ġ �޼� ��
	if (m_sPlayInfo.iLevel < 100 && 
		m_sPlayInfo.fExp >= m_sPlayInfo.fMaxExp)
	{
		// ������
		++m_sPlayInfo.iLevel;
		// ����ġ
		m_sPlayInfo.fExp = 0;
		// �ʿ����ġ
		m_sPlayInfo.fMaxExp = (m_sPlayInfo.iLevel * 20) + ((m_sPlayInfo.iLevel * 20) * 0.1); 
		// ��ų ����Ʈ
		++m_sPlayInfo.iSKillPoint;
		// ���� ����Ʈ
		m_sPlayInfo.iStatPoint += 5;
		// ü�� �ʱ�ȭ
		m_sPlayInfo.fHealthPoint = m_sPlayInfo.fHealthPointMAX;
		// ���� �ʱ�ȭ
		m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX;
	}
}

void CPlayer::ScrollChange()
{
	if(m_pMotion != ATTACK && m_pMotion != CAST && m_pObjName == PLAYER)
	{
		m_Info.vPos;
		CObj::g_tScroll += m_Info.vDir;
		if(CObj::g_tScroll.x < 0)
			CObj::g_tScroll.x -= m_Info.vDir.x;
		if (CObj::g_tScroll.y < 0)
			CObj::g_tScroll.y -= m_Info.vDir.y;
		if(CObj::g_tScroll.x > 1730)
			CObj::g_tScroll.x -= m_Info.vDir.x;
		if (CObj::g_tScroll.y > 732)
			CObj::g_tScroll.y -= m_Info.vDir.y;
	}
}

void CPlayer::SkillActive(TCHAR* _SkillName)
{
	DEBUG_ENTRY_EXIT;
	DEBUG_LINE;

	if(_SkillName != nullptr)
	{
		if( _SkillName == BONESPEAR && m_SkillTree.sBoneSpear.iLevel > 0 )
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, BONESPEAR)))
				{
					ERR_MSG(g_hWnd, L"BoneSpear ��ü ���� ����");
				}
			}
		}
		else if (_SkillName == FIREWALL && m_SkillTree.sFireWall.iLevel > 0 )
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, FIREWALL)))
				{
					ERR_MSG(g_hWnd, L"BoneSpear ��ü ���� ����");
				}
			}
		}
		else if (_SkillName == BLIZZARD && m_SkillTree.sBlizzard.iLevel > 0)
		{
			m_pMotion = CAST;
			if (m_tFrame.fStart <= 0)
			{
				if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
					, BLIZZARD)))
				{
					ERR_MSG(g_hWnd, L"BoneSpear ��ü ���� ����");
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
