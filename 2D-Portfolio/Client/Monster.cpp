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
}


CMonster::~CMonster(void)
{
}

HRESULT CMonster::Initialize()
{
	
	m_Info.vPos = D3DXVECTOR3((rand()%800), (rand()%600), 0.f);
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_vTagetInfo = m_Info.vPos;
	m_Info.fCX = 50.f;
	m_Info.fCY = 75.f;

	m_sPlayInfo.iLevel = 1;
	m_sPlayInfo.fExp = m_sPlayInfo.fMaxExp = 0;
	m_sPlayInfo.fAttack = 10;
	m_sPlayInfo.fDefence = 5;
	m_sPlayInfo.fMight = 5;
	m_sPlayInfo.fDexterity = 5;
	m_sPlayInfo.fIntellect = 5;
	m_sPlayInfo.fConstitution = 5;
	m_sPlayInfo.fResolve = 5;
	m_sPlayInfo.fPerception = 5;
	m_sPlayInfo.fHealthPoint = 1;
	m_sPlayInfo.fHealthPointMAX = (m_sPlayInfo.fConstitution * 100);
	m_sPlayInfo.fMagikaPoint = m_sPlayInfo.fMagikaPointMAX = (m_sPlayInfo.fPerception * 100);
	m_sPlayInfo.iGold = 0;
	m_sPlayInfo.fSpeed = (100.0f) + (m_sPlayInfo.fDexterity * 3.0f);

	// 기본적으로 캐릭터는 아래를 보고있게 설정.
	m_iDegree = 280.f;
	m_bSelect = true;
	m_eLayer = LAYER_OBJECT;
	m_vMovePoint = m_Info.vPos;
	m_pStateKey = L"FieldStand_D";
	m_pMotion = L"FieldStand";
	
	m_iDir = rand()%8;

	return S_OK;
}

SCENEID CMonster::Progress()
{
	
	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

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
	if ( m_pMotion != DEATH)
		m_Info.vPos += m_Info.vDir * ((m_sPlayInfo.fSpeed + (m_sPlayInfo.fDexterity * 3.0f))*0.001f);

	// 자신의 위치에서 목표지점거리보다 이동거리가 크면 자신의 위치를 목표지점으로 지정
	if (fDistance < 1.0f)
		m_Info.vPos = m_vTagetInfo;

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x, m_Info.vPos.y, m_Info.vPos.z );

	// 크기 자전 이동 공전 부모
	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;

	DirectAction(m_pMotion);

	//첫번째 인자값은 초당 몇프레임을 출력할건지, 두번째는 최대 장수
	FrameStatas();

	return SCENEID_NONPASS;
}

void CMonster::Render()
{
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, m_pStateKey, int(m_tFrame.fStart));
	// 프레임 값이 저장한 이미지 벡터크기를 벗어난 값이 들어가면 에러

	if(pTexInfo == NULL)
		return;

	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.6, 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CMonster::CheckKey()
{
	// 제어할 캐릭터 선택
	static float fTime = 0.0f;
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();
	

	// 이동할 거리
	float moveLength = rand()%30+1;
	// 몇초에 한번 이동할지
	int icount = (int)fTime%10;
	// 방향
	
	// 3초에 한번 방향과 이동을 정함
	if( icount == 5)
	{
		m_iDir = rand()%16;
		switch(m_iDir/2)
		{
			//  방향을 정함
		case 0:
			// RD
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y + moveLength, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;												   
		case 1:	
			// RU
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y - moveLength, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;												   
		case 2:	
			// LD
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y + moveLength, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;													 		
		case 3:	
			// LU
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y - moveLength, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;
		case 4:	
			// L
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x - moveLength, m_Info.vPos.y, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;
		case 5:	
			// R
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x + moveLength, m_Info.vPos.y, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;
		case 6:	
			// U
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y - moveLength, 0);
			/*if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;*/
			break;
		case 7:	
			// D
			m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y + moveLength, 0);
			if(m_tFrame.fStart >= m_tFrame.fLast)
				m_iDir = rand()%8;
			break;
		}

	}
	if (m_vMovePoint.x <= 0 || m_vMovePoint.x >= WINSIZEX ||
		m_vMovePoint.y <= 0 || m_vMovePoint.y >= WINSIZEY)
		m_vMovePoint = m_Info.vPos;

	if( m_pMotion == STAND) 
	{	
		// 이동할 지점을 가져옴
		m_vTagetInfo = m_vMovePoint;
		// 이동할 지점을 바라보는 방향
		m_Info.vDir = m_vTagetInfo - m_Info.vPos;
	}
	else if( m_pMotion == RUN )
	{
		//m_vTagetInfo = m_vMovePoint;
		m_Info.vDir = m_vTagetInfo - m_Info.vPos;
	}

	// 움직일 목표위치에 도달했는지
	if ( m_vTagetInfo == m_Info.vPos )
	{
		// 도달했을 시 서있는 상태
		m_pMotion = STAND;
		if( m_pMotion == ATTACK ) 
		{
			// 마우스를 바라보는 방향
			m_Info.vDir = m_vMovePoint - m_Info.vPos; 
			// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다 
			m_fChaterDirect = m_iDegree;
			// 취할 모션이미지를 바꿈
			m_pMotion = ATTACK;
		}
	}
	// 목표위치에 도달하지 못했을때
	else if ( m_vTagetInfo != m_Info.vPos )
	{
		// 공격시
		if( m_pMotion == ATTACK ) 
		{
			// 마우스를 바라보는 방향
			m_Info.vDir = m_vMovePoint - m_Info.vPos; 
			// 플레이어 캐릭터 방향을 마우스가 있는 방향(각도)을 넣는다 
			m_fChaterDirect = m_iDegree;
			// 취할 모션이미지를 바꿈
			m_pMotion = ATTACK;
			// 이동 중 공격시 이동을 멈춤
			m_vTagetInfo = m_Info.vPos;
		}
		else
			// 공격 중이 아닐때 
			m_pMotion = RUN;
	}
	if(int(m_sPlayInfo.fHealthPoint) <= 0)
	{
		m_pMotion = DEATH;
		m_vTagetInfo = m_Info.vPos; 
	}
}
void CMonster::Release()
{

}

CObj* CMonster::Clone()
{
	return new CMonster(*this);
}

POINT CMonster::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CMonster::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}


void CMonster::FrameStatas()
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
			m_pStateKey = L"FieldStand_RD";
		}
		// 아래 D
		if (m_fChaterDirect >= 247.5f && m_fChaterDirect <= 292.5f)
		{
			m_pStateKey = L"FieldStand_D";
		}
		// 왼쪽 아래 LD
		if (m_fChaterDirect >= 202.5f && m_fChaterDirect <= 247.5f)
		{
			m_pStateKey = L"FieldStand_LD";
		}
		// 왼쪽 L
		if (m_fChaterDirect >= 157.5f && m_fChaterDirect <= 202.5f)
		{
			m_pStateKey = L"FieldStand_L";
		}
		// 왼쪽 위 LU
		if (m_fChaterDirect >= 112.5f && m_fChaterDirect <= 157.5f)
		{
			m_pStateKey = L"FieldStand_LU";
		}
		// 위 U
		if (m_fChaterDirect >= 67.5f && m_fChaterDirect <= 112.5f)
		{
			m_pStateKey = L"FieldStand_U";
		}
		// 오른쪽 R
		if (m_fChaterDirect >= 0 &&  m_fChaterDirect <= 22.5f
			|| m_fChaterDirect >= 337.5f && m_fChaterDirect <= 360.f )
		{			
			m_pStateKey = L"FieldStand_R";
		}
		// 오른쪽 위 RU
		if (m_fChaterDirect >= 22.5f && m_fChaterDirect <= 67.5f)
		{
			m_pStateKey = L"FieldStand_RU";
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
		if (m_fChaterDirect >= 292.5f && m_fChaterDirect <= 337.5f)
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
}