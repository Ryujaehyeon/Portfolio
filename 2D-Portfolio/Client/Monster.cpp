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

	// �⺻������ ĳ���ʹ� �Ʒ��� �����ְ� ����.
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
	
	// �ð�
	static float fTime = 0.0f;
	// �ð��� ����
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();
	
	//m_Info.vPos = m_Info.vPos + CObj::g_tScroll;

	RegenTime();

	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	// �Է¹޴� Ű�� üũ�� ���¸� ����
	CheckKey();

	// �Ÿ��� ���Ѵ�.
	float fDistance = D3DXVec3Length(&m_Info.vDir);

	// ���� ����ȭ
	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	// �̵��� ��ġ�� �ڽ��� ��ġ�� ������ ������ ������ ����
	// ���ϰ� �Ǹ� ���� ���ų� �Ʒ��� ���� ����
	if (m_vTagetInfo != m_Info.vPos)
		// �������� ���Ѵ�
		m_fAngle = acosf(D3DXVec3Dot(&m_Info.vDir, &m_Info.vLook));

	// ������ ���Ѵ�.
	if (m_vTagetInfo.y > m_Info.vPos.y)
	{
		m_fAngle = 2 * D3DX_PI - m_fAngle;
	}
	// ������ ���� ���� ��׸���(0~360)���� ����
	m_iDegree = D3DXToDegree(m_fAngle);

	// �̵� ������ �������� ������ ����
	m_fChaterDirect = m_iDegree;

	// �̵��Ÿ�
	if ( m_pMotion == RUN && m_pMotion != CAST)
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.001f);

	// �ڽ��� ��ġ���� ��ǥ�����Ÿ����� �̵��Ÿ��� ũ�� �ڽ��� ��ġ�� ��ǥ�������� ����
	if (fDistance < 1.0f && m_pMotion != DEATH)
		m_Info.vPos = m_vTagetInfo;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, 0.f);

	// ũ�� ���� �̵� ���� �θ�
	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	DirectAction(m_pMotion);

	//ù��° ���ڰ��� �ʴ� ���������� ����Ұ���, �ι�°�� �ִ� ���
	FrameStatas();

	// �÷��̾�� ������ ����ġ�� ��
	ExpAcquired();

	return SCENEID_NONPASS;
}

void CMonster::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// ������ ���� ������ �̹��� ����ũ�⸦ ��� ���� ���� ����

	if(pTexInfo == NULL)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// �׸���
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

	// ��ü 
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
	// ������ ĳ���� ����
	static float fTime = 0.0f;
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// ������ ĳ���� ����
	static float fTimeSkill = 0.0f;
	fTimeSkill += GET_SINGLE(CTimeMgr)->DeltaTime();

	// �������� ������ �Ȱ�����
	
	m_fWaitTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// �̵��� �Ÿ�
	float moveLength = rand()%100+1;
	// ���ʿ� �ѹ� �̵�����
	
	//
	if(m_sPlayInfo.fHealthPoint <= 0)
	{

		// ������ ���� Ÿ������ ���
		Tageting();

		// �ٶ󺸴� ����
		if(m_pTagetObj != NULL)
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

		m_pMotion = DEATH;
		
		return;
	}
	// 3�ʿ� �ѹ� ����� �̵��� ����, �浹���� �ʾ�����
	if(m_Crash == false)
	{
		// ����
		if( m_pMotion == STAND) 
		{	
			// �̵��� ������ ������
			m_vTagetInfo = m_vMovePoint;

			// �̵��� ������ �ٶ󺸴� ����
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
					//  ������ ����
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
		// �ȱ�
		if( m_pMotion == RUN )
		{
			m_vTagetInfo = m_vMovePoint;
			m_Info.vDir = m_vTagetInfo - m_Info.vPos;
			if ( m_vTagetInfo == m_Info.vPos )
			{
				// �������� �� ���ִ� ����
				m_pMotion = STAND;
			}
		}
		// ������
		if (m_pMotion == ATTACK)
		{
			m_pMotion = STAND;
		}
	}
	else if( m_Crash == true )
	{
		// �浹������
		if(int(m_sPlayInfo.fHealthPoint) <= 0)
			m_pMotion = DEATH;
		
		// �ٶ󺸴� ����
		//m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;
		Tageting();
		float fDistance = (m_pTagetObj->Setinfo()->fCX * 0.5f) +
						  (m_Info.fCX * 0.5f);

		float fRealDistance = D3DXVec3Length(&m_Info.vDir);

		if (m_pObjName == SUMMONER)
		{
			// �Ÿ��� �⺻���� ��Ÿ����� �ְ� 3���� �Ÿ����� ������ ��ų���
			if(fDistance * 1.2f < fRealDistance && fDistance * 5.f > fRealDistance && m_pMotion != DEATH)
			{
					fTimeSkill = 0.f;
					m_pMotion = CAST;
					MonSKill();
			}
			// �Ÿ��� �⺻���� ��Ÿ����� ������
			else if (fDistance * 1.2f > fRealDistance)
			{
				m_pMotion = ATTACK;
				FuncAttack();
			}
			else if(fDistance * 5.0f < fRealDistance)
			{
				// �⺻���� ��Ÿ��� �浹�Ÿ����� �ָ� �Ѿư���.
				m_pMotion = RUN;
			}
		}
		// �Ÿ��� ���� �ٸ� ��ü�� �浹���� �� ����(�ڽ��� ũ�⺸�� 1.2��� ���� ��Ÿ�)
		else if(fDistance * 1.2f > fRealDistance && m_pMotion != DEATH)
		{
			m_pMotion = ATTACK;
			FuncAttack();
		}
		else if(fDistance < fRealDistance)
		{
			// �⺻���� ��Ÿ��� �浹�Ÿ����� �ָ� �Ѿư���.
			m_pMotion = RUN;
		}
		

		if(fDistance * 5.f < fRealDistance)
		{
			// �浹�Ÿ��� 5���� �Ÿ����� ũ�� �浹���¸� Ǭ��
			if(int(m_tFrame.fStart) < 0.f )
				m_pMotion = STAND;
			m_Crash = false;
			m_pTagetObj = NULL;
		}
		if(m_pMotion == RUN && m_fWaitTime > 5.f)
		{
			// �Ѿư��� ���¿��� 5�� �̻� ������ �浹���¸� Ǭ��
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
		// ����Ʈ ���� �浹�� �༮�� ������
		if ((*iter)->GetCrash() == true)
		{
			// �ش� ��ü�� Ÿ����
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// Ÿ���� ���� �����ϰ� ������ ������
			m_vTagetInfo = m_pTagetObj->GetInfo().vPos;

			// ���� ��� �������� ������ �������� ����
			if (m_tFrame.fStart <= 0.f)
			{
				// ���� ������ �÷��̾� ���ݷº��� ũ�� ������ 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// �÷��̾� ���ݷ��� �� ũ�� ���� ������ �� ������ ��ŭ�� �������� ���� 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack*1.5f - (*iter)->GetStatas().fDefence;

				// ����� ������ Ÿ���� ����
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
		// ����Ʈ ���� �浹�� �༮�� ������
		if ((*iter)->GetCrash() == true)
		{
			// �ش� ������ ��ü�� Ÿ����
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// Ÿ���� ���� �����ϰ� ������ ������
			m_vTagetInfo = m_pTagetObj->GetInfo().vPos;

			// ���� ��� �������� ������ �������� ����
			if (m_tFrame.fStart <= 0.f)
			{
				// ���� ������ �÷��̾� ���ݷº��� ũ�� ������ 1
				if((*iter)->GetStatas().fDefence >= m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 1;
				// �÷��̾� ���ݷ��� �� ũ�� ���� ������ �� ������ ��ŭ�� �������� ���� 
				else if((*iter)->GetStatas().fDefence < m_sPlayInfo.fAttack)
					(*iter)->SetStatas()->fHealthPoint -= 
					m_sPlayInfo.fAttack - (*iter)->GetStatas().fDefence;

				// ����� ������ Ÿ���� ����
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
		// ������ �Ʒ� RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Stand_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Stand_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Stand_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Stand_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Stand_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Stand_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Stand_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Stand_RU";
		}
	}
	else if (_pObjStatas == RUN)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Run_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Run_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Run_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Run_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Run_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Run_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Run_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Run_RU";
		}
	}	
	else if (_pObjStatas == ATTACK)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == ATTACKSKILL)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
		{
			m_pStateKey = L"Attack_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Attack_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Attack_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Attack_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Attack_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Attack_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Attack_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Attack_RU";
		}
	}
	else if (_pObjStatas == DEATH)
	{
		// ������ �Ʒ� RD
		{
			m_pStateKey = L"Death_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Death_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Death_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Death_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Death_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Death_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Death_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Death_RU";
		}
	}
	else if (_pObjStatas == CAST)
	{
		// ������ �Ʒ� RD
		{
			m_pStateKey = L"Cast_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"Cast_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"Cast_LD";
		}
		// ���� L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"Cast_L";
		}
		// ���� �� LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"Cast_LU";
		}
		// �� U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"Cast_U";
		}
		// ������ R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"Cast_R";
		}
		// ������ �� RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"Cast_RU";
		}
	}
}


void CMonster::Tageting()
{
	// ������ ���� Ÿ������ ���
	list<CObj*>::iterator iter = m_pTagetList->begin();
	for (;iter != m_pTagetList->end(); ++iter)
	{
		// ����Ʈ ���� �浹�ϰ� ���콺�� ���� ���� ������
		if ((*iter)->GetCrash() == true)
		{
			// �ش� ������ ��ü�� Ÿ����
			m_pTagetObj = (*iter);
			m_Info.vDir = m_pTagetObj->GetInfo().vPos - m_Info.vPos;

			// Ÿ���� ���� �����ϰ� ������ ������
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
