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
}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_vTagetInfo = m_Info.vPos;
	m_Info.fCX = 50.f;
	m_Info.fCY = 75.f;
	
	m_sPlayInfo.iLevel = 1;
	m_sPlayInfo.fExp = 0;
	m_sPlayInfo.fMight = 5;
	m_sPlayInfo.fDexterity = 5;
	m_sPlayInfo.fIntellect = 5;
	m_sPlayInfo.fConstitution = 5;
	m_sPlayInfo.fResolve = 5;
	m_sPlayInfo.fPerception = 5;
	m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel * 50 + m_sPlayInfo.iLevel * 25;
	m_sPlayInfo.fAttack = m_sPlayInfo.iLevel * 10 + m_sPlayInfo.fMight * 1;
	m_sPlayInfo.fDefence = 5;
	m_sPlayInfo.iSKillPoint = 0;
	m_sPlayInfo.fHealthPoint = 250;
	m_sPlayInfo.fHealthPointMAX = (m_sPlayInfo.fConstitution * 100);
	m_sPlayInfo.fMagikaPoint = 250;
	m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.fPerception * 100);
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = 100.0f;

	// �⺻������ ĳ���ʹ� �Ʒ��� �����ְ� ����.
	m_fChaterDirect = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_bRun = true;
	m_pStateKey = L"FieldStand_D";
	m_pMotion = L"FieldStand";
	m_fAngle = D3DXToRadian(280.f);
	m_pTagetObj = NULL;
	return S_OK;
}

SCENEID CPlayer::Progress()
{
	// �ð�
	static float fTime = 0.0f;
	// �ð��� ����
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();
	
	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);
	if (m_sPlayInfo.fHealthPoint <= m_sPlayInfo.fHealthPointMAX)
		++m_sPlayInfo.fHealthPoint;

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

	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5, 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}


void CPlayer::CheckKey()
{
	// ������ ĳ���� ����
	CharacterSelect();
	m_vMousePos = MouseInfoDX();

	//if( m_Crash == false )
	//	m_pTagetObj = this;

	if (GetAsyncKeyState(VK_TAB) & 0x8000 && m_bSelect == true)
	{
		m_bRun = !m_bRun;
	}
	if( GetAsyncKeyState(VK_RBUTTON) & 0x8000 && m_bSelect == true) 
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
			if( GetAsyncKeyState(VK_LBUTTON) & 0x8000 ) 
			{
				// ���콺�� �ٶ󺸴� ����
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´� 
				m_fChaterDirect = m_iDegree;
				// ���� ����̹����� �ٲ�
				m_pMotion = ATTACK;
				if(m_pTagetObj != NULL)
					FuncAttack(m_pTagetObj, this);
				// ������ ������ Ÿ���� �ѹ� ������
				m_pTagetObj = NULL;
			}
		}
		// ��ǥ��ġ�� �������� ��������
		else if ( m_vTagetInfo != m_Info.vPos )
		{
			if( GetAsyncKeyState(VK_LBUTTON) & 0x8000 ) 
			{
				// ���콺�� �ٶ󺸴� ����
				m_Info.vDir = m_vMousePos - m_Info.vPos; 
				// �÷��̾� ĳ���� ������ ���콺�� �ִ� ����(����)�� �ִ´� 
				m_fChaterDirect = m_iDegree;
				// ���� ����̹����� �ٲ�
				m_pMotion = ATTACK;
				// �̵� �� ���ݽ� �̵��� ����
				m_vTagetInfo = m_Info.vPos;
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
void CPlayer::FuncAttack(CObj* _pDest, CObj* _pSour)
{
	if( _pSour->GetObjType() == _pDest->GetObjType())
		return;

	if(_pSour->GetpMotion() == ATTACK)
	{
		if (_pSour->GetFrame().fStart >= PLAYER_ATTACK-0.5f)
		{
			if(_pDest->GetStatas().fDefence >= _pSour->GetStatas().fAttack)
				_pDest->SetStatas()->fHealthPoint -= 1;
			else if(_pDest->GetStatas().fDefence < _pSour->GetStatas().fAttack)
				_pDest->SetStatas()->fHealthPoint -= 
				_pSour->GetStatas().fAttack - _pDest->GetStatas().fDefence;
		}
	}
}
POINT CPlayer::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CPlayer::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
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
			m_pStateKey = L"FieldStand_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"FieldStand_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"FieldStand_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"FieldStand_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"FieldStand_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"FieldStand_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"FieldStand_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"FieldStand_RU";
		}
	}
	// �ȱ�
	if(_pObjStatas == WALK)
	{
		// ������ �Ʒ� RD
		if (m_fChaterDirect > 292.5f && m_fChaterDirect < 337.5f)
		{
			m_pStateKey = L"FieldWalk_RD";
		}
		// �Ʒ� D
		if (m_fChaterDirect > 247.5f && m_fChaterDirect < 292.5f)
		{
			m_pStateKey = L"FieldWalk_D";
		}
		// ���� �Ʒ� LD
		if (m_fChaterDirect > 202.5f && m_fChaterDirect < 247.5f)
		{
			m_pStateKey = L"FieldWalk_LD";
		}
		// ���� L
		if (m_fChaterDirect > 157.5f && m_fChaterDirect < 202.5f)
		{
			m_pStateKey = L"FieldWalk_L";
		}
		// ���� �� LU
		if (m_fChaterDirect > 112.5f && m_fChaterDirect < 157.5f)
		{
			m_pStateKey = L"FieldWalk_LU";
		}
		// �� U
		if (m_fChaterDirect > 67.5f && m_fChaterDirect < 112.5f)
		{
			m_pStateKey = L"FieldWalk_U";
		}
		// ������ R
		if (m_fChaterDirect > 0 &&  m_fChaterDirect < 22.5f
			|| m_fChaterDirect > 337.5f && m_fChaterDirect < 360.f )
		{			
			m_pStateKey = L"FieldWalk_R";
		}
		// ������ �� RU
		if (m_fChaterDirect > 22.5f && m_fChaterDirect < 67.5f)
		{
			m_pStateKey = L"FieldWalk_RU";
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
}

void CPlayer::CharacterSelect()
{
	if (m_pObjKey == PLAYER)
	{
		if(GetAsyncKeyState(VK_F1) & 0x8000)
			m_bSelect = true;
		if(GetAsyncKeyState(VK_F2) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F3) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F4) & 0x8000)
			m_bSelect = true;
		return;
	}
	else if (m_pObjKey == FELLOW_1ST)
	{
		if(GetAsyncKeyState(VK_F1) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F2) & 0x8000)
			m_bSelect = true;
		if(GetAsyncKeyState(VK_F3) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F4) & 0x8000)
			m_bSelect = true;
		return;
	}
	else if (m_pObjKey == FELLOW_2ND)
	{
		if(GetAsyncKeyState(VK_F1) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F2) & 0x8000)
			m_bSelect = false;
		if(GetAsyncKeyState(VK_F3) & 0x8000)
			m_bSelect = true;
		if(GetAsyncKeyState(VK_F4) & 0x8000)
			m_bSelect = true;
		return;
	}
}
void CPlayer::Release()
{

}

CObj* CPlayer::Clone()
{
	return new CPlayer(*this);
}
