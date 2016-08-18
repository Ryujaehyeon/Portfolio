#include "StdAfx.h"
#include "UIObj.h"
#include "Include.h"

CUIObj::CUIObj(void)
{
}

CUIObj::CUIObj( const OBJINFO& Info, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType),
	SelectCount(0)
{

}

CUIObj::CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageStatic(Info, _ObjType),
	SelectCount(0)
{
	m_pObjKey = _ObjName;

}

bool CUIObj::m_InvenKey = false;
bool CUIObj::m_SkillTree = false;
bool CUIObj::m_Bank = false;
bool CUIObj::m_Character = false;

CUIObj::~CUIObj(void)
{
	Release();
}


HRESULT CUIObj::Initialize()
{
	TCHAR* UI[]=
	{
		L"StatusBar",
		L"HP",
		L"HPBarBack",
		L"HPBar",
		L"MP",
		L"Stamina",
		L"Run",
		L"SkillPointTrue",
		L"SkillPointFalse",
		L"StatPointTrue",
		L"StatPointFalse",
		L"ExpBar",
		L"Inven",
		L"SkillTree",
		L"Bank",
		L"Character",
		L"BoneSpearButton",
		L"FireWallButton",
		L"BlizzardButton"
	};

	if(m_pObjKey == UI[0])
	{
		m_Info.vPos  = D3DXVECTOR3(512.f, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[1])
	{
		m_Info.vPos  = D3DXVECTOR3(93.f, 635.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 128.f;
	}
	if(m_pObjKey == UI[2])
	{
		m_Info.vPos  = D3DXVECTOR3(400.f, 20.f, 0.f);
		m_Info.fCX = 100.f;
		m_Info.fCY = 10.f;
	}
	if(m_pObjKey == UI[3])
	{
		m_Info.vPos  = D3DXVECTOR3(400.f, 20.f, 0.f);
		m_Info.fCX = 100.f;
		m_Info.fCY = 10.f;
	}
	if(m_pObjKey == UI[4])
	{
		m_Info.vPos  = D3DXVECTOR3(755.f, 635.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 128.f;
	}
	if(m_pObjKey == UI[5])
	{	
		m_Info.vPos  = D3DXVECTOR3(336.f, 589.f, 0.f);
		m_Info.fCX = 128.f;
		m_Info.fCY = 32.f;
	}
	if(m_pObjKey == UI[6])
	{	
		m_Info.vPos  = D3DXVECTOR3(263.f, 581.f, 0.f);
		m_Info.fCX = 32.f;
		m_Info.fCY = 32.f;
	}
	if(m_pObjKey == UI[7])
	{	
		m_Info.vPos  = D3DXVECTOR3(578.f, 578.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[8])	
	{	
		m_Info.vPos  = D3DXVECTOR3(578.f, 578.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[9])				 
	{	
		m_Info.vPos  = D3DXVECTOR3(221.f, 577.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[10])				 
	{	
		m_Info.vPos  = D3DXVECTOR3(221.f, 577.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[11])				 
	{
		m_Info.vPos  = D3DXVECTOR3(316, 564.f, 0.f);
		m_Info.fCX = 120.f;
		m_Info.fCY = 3.f;
	}
	if(m_pObjKey == UI[12])				 
	{
		m_Info.vPos  = D3DXVECTOR3(512, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[13])				 
	{
		m_Info.vPos  = D3DXVECTOR3(512, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[14])				 
	{
		m_Info.vPos  = D3DXVECTOR3(512, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[15])				 
	{
		m_Info.vPos  = D3DXVECTOR3(512, 512.f, 0.f);
		m_Info.fCX = 1024.f;
		m_Info.fCY = 1024.f;
	}
	if(m_pObjKey == UI[16])				 
	{
		m_Info.vPos  = D3DXVECTOR3(530.f, 48.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[17])				 
	{
		m_Info.vPos  = D3DXVECTOR3(530.f, 135.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	if(m_pObjKey == UI[18])				 
	{
		m_Info.vPos  = D3DXVECTOR3(530.f, 310.f, 0.f);
		m_Info.fCX = 64.f;
		m_Info.fCY = 64.f;
	}
	m_Info.vDir  = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_Info.vLook = D3DXVECTOR3(1.0f, 0.f, 0.f);
	m_pStateKey = NULL;
	m_eLayer = LAYER_UI;
	return S_OK;
}

SCENEID CUIObj::Progress()
{
	// 시간
	static float fTime = 0.0f;
	// 시간값 누적
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();
	
	CheckKey();
	
	if (m_SkillTree == true)
	{
		if (m_pObjKey == L"BoneSpearButton" && PtInRect(&RealRect(), MouseInfo()) )
		{
			if (m_dwKey & KEY_LBUTTON && fTime > 1.f)
			{
				fTime = 0;
				for (list<CObj*>::iterator iter = m_PlayerData->begin();
					iter != m_PlayerData->end(); ++iter)
				{
					if((*iter)->GetSelect() == true)
					{
						if(
							((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel <
							((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLimitLevel
							&& ((CPlayer*)(*iter))->SetStatas()->iSKillPoint > 0)
						{
							++((CPlayer*)(*iter))->SetSkillTree()->sBoneSpear.iLevel;
							--((CPlayer*)(*iter))->SetStatas()->iSKillPoint;
						}
					}
				}
			}
		}
		else if(m_pObjKey == L"FireWallButton" && PtInRect(&RealRect(), MouseInfo()) )
		{
			if (m_dwKey & KEY_LBUTTON && fTime > 1.f)
			{
				fTime = 0;
				for (list<CObj*>::iterator iter = m_PlayerData->begin();
					iter != m_PlayerData->end(); ++iter)
				{
					if(((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel > 
						((CPlayer*)(*iter))->GetSkillTree().sFireWall.iMinLevel)
					{
						if(
							((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel <
							((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLimitLevel
							&& ((CPlayer*)(*iter))->SetStatas()->iSKillPoint > 0)
						{
							++((CPlayer*)(*iter))->SetSkillTree()->sFireWall.iLevel;
							--((CPlayer*)(*iter))->SetStatas()->iSKillPoint;
						}
					}
				}
			}
		}
		else if(m_pObjKey == L"BlizzardButton" && PtInRect(&RealRect(), MouseInfo()) )
		{
			if (m_dwKey & KEY_LBUTTON && fTime > 1.f)
			{
				fTime = 0;
				for (list<CObj*>::iterator iter = m_PlayerData->begin();
					iter != m_PlayerData->end(); ++iter)
				{
					if(((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel > 
						((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iMinLevel)
					{
						if(
							((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iLevel <
							((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iLimitLevel
							&& ((CPlayer*)(*iter))->SetStatas()->iSKillPoint > 0)
						{
							++((CPlayer*)(*iter))->SetSkillTree()->sBlizzard.iLevel;
							--((CPlayer*)(*iter))->SetStatas()->iSKillPoint;
						}
					}
				}
			}
		}
	}


/*

			else if (m_pObjKey == L"BlizzardButton" && PtInRect(&RealRect(), MouseInfo()) )
			{
				for (list<CObj*>::iterator iter = m_PlayerData->begin();
					iter != m_PlayerData->end(); ++iter)
				{
					if((*iter)->GetSelect() == true)
					{
						if(((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel > 
							((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iMinLevel)
							++((CPlayer*)(*iter))->SetSkillTree()->sBlizzard.iLevel;
						--((CPlayer*)(*iter))->SetStatas()->iSKillPoint;
					}
				}
			}

		}
	}*/
	
	D3DXMatrixScaling(&m_Info.matScale, 1.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	D3DXMatrixTranslation(&m_Info.matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);

	m_Info.matWorld = m_Info.matScale * m_Info.matTrans;
	return SCENEID_NONPASS;
}

void CUIObj::Render()
{
	// 텍스쳐매니저를 통해 텍스쳐를 가져온다.
	// 이미 Initialize에서 정해진 오브젝트의 이름으로 구해온다.
	D3DXVECTOR3 Mouse = MouseInfoDX();

	for (list<CObj*>::iterator iter = m_PlayerData->begin();
		iter != m_PlayerData->end(); ++iter)
	{
		if((*iter)->GetSelect())
		{
			if(SelectCount < 3)
				++SelectCount;
			else
				SelectCount = 3;
			m_PlayerObj = (*iter);
		}
		else
			--SelectCount;
	}
	if (SelectCount == 3)
	{
		m_PlayerObj = (*m_PlayerData->begin());
	}
		
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	if (m_pObjKey == L"SkillPoint")
	{
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey, NULL, 1);
	}

	// 없으면 실패하여 리턴
	if(pTexInfo == NULL)
		return;

	// 출력창에서 해당 함수를 거쳤는지 확인 가능
	// DEBUG_ENTRY_EXIT;
	// 해당 라인을 지났는지
	// DEBUG_LINE;

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);

	//// &GetRect() 그리는 이미지의 좌표

	if(m_pObjKey == L"HP" ||m_pObjKey == L"MP" )
	{
		RECT rc = {
					int(0), 
					int(m_Info.fCY - (m_Info.fCY * VelueToPercentage(m_pObjKey))),
					int(m_Info.fCX), 
					int(m_Info.fCY)
					};

		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * VelueToPercentage(m_pObjKey), 0);

		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if ( m_pObjKey == L"ExpBar" )
	{
		RECT rc = {
			float(0), 
			float(0),
			float(m_Info.fCX * VelueToPercentage(m_pObjKey)), 
			float(m_Info.fCY)
		};
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.5f, 0);

		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if ( m_pObjKey == L"HPBar" )
	{
		RECT rc = {
			float(0), 
			float(0),
			float(m_Info.fCX * VelueToPercentage(m_pObjKey)), 
			float(m_Info.fCY)
		};
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.5f, 0);

		if( VelueToPercentage(m_pObjKey) >= 0.f && m_pObjKey == L"HPBar")
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
	else if (m_pObjKey == L"HPBarBack")
	{
		RECT rc = {
			float(0), 
			float(0),
			float(m_Info.fCX), 
			float(m_Info.fCY)
		};
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.5f, 0);

		if (VelueToPercentage(L"HPBar")>= 0.f)
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			&rc, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (m_pObjKey == L"Inven")
	{
		if(m_InvenKey == true)
		{
			// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
			m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
				pTexInfo->ImgInfo.Height * 0.5f, 0);

			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	else if (m_pObjKey == L"SkillTree")
	{
		if(m_SkillTree == true)
		{
			// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
			m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
				pTexInfo->ImgInfo.Height * 0.5f, 0);

			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


		}
	}
	else if (m_pObjKey == L"Bank")
	{
		if(m_Bank == true)
		{
			// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
			m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
				pTexInfo->ImgInfo.Height * 0.5f, 0);

			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	else if (m_pObjKey == L"Character")
	{
		if(m_Character == true)
		{
			// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
			m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
				pTexInfo->ImgInfo.Height * 0.5f, 0);

			GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
				NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	else if (m_pObjKey == L"BoneSpearButton")
	{
		if(m_SkillTree == true)
		{
			{
				// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
				m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
					pTexInfo->ImgInfo.Height * 0.5f, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			FontOutput(m_pObjKey);
		}
	}
	else if (m_pObjKey == L"FireWallButton")
	{
		if(m_SkillTree == true)
		{
			{
				// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
				m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
					pTexInfo->ImgInfo.Height * 0.5f, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			FontOutput(m_pObjKey);
		}
	}
	else if (m_pObjKey == L"BlizzardButton")
	{
		if(m_SkillTree == true)
		{
			{
				// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
				m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
					pTexInfo->ImgInfo.Height * 0.5f, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			FontOutput(m_pObjKey);
		}
	}
	else
	{
		// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
		m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
			pTexInfo->ImgInfo.Height * 0.5f, 0);

		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUIObj::Release()
{

}

CObj* CUIObj::Clone()
{
	return new CUIObj(*this);
}

void CUIObj::Setlist( list<CObj*>* _list )
{
	list<CObj*>::iterator iter = _list->begin();
	if(iter != _list->end() && (*iter)->GetObjType() == OBJ_PLAYER)
		m_PlayerData = _list;
	else if (iter != _list->end() && (*iter)->GetObjType() == OBJ_MONSTER)
		m_MonsterData = _list;
}

float CUIObj::VelueToPercentage(TCHAR* VelueName)
{
	if(L"HP" == VelueName)
		return m_PlayerObj->GetStatas().fHealthPoint/m_PlayerObj->GetStatas().fHealthPointMAX;

	if (L"MP" == VelueName)
		return m_PlayerObj->GetStatas().fMagikaPoint/m_PlayerObj->GetStatas().fMagikaPointMAX;

	if (L"ExpBar" == VelueName)
		return m_PlayerObj->GetStatas().fExp/m_PlayerObj->GetStatas().fMaxExp;

	if (L"HPBar" == VelueName )
	{
		list<CObj*>::iterator iter = m_MonsterData->begin();
		for (; iter != m_MonsterData->end(); ++iter)
		{
			RECT rc = 
			{   float(m_Info.vPos.x + CObj::g_tScroll.x), 
				float(m_Info.vPos.y + CObj::g_tScroll.y),
				float(m_Info.vPos.x + CObj::g_tScroll.x), 
				float(m_Info.vPos.y + CObj::g_tScroll.y)
			};
			if(PtInRect(&(*iter)->RealRect(), MouseInfo()))
			{
				return (*iter)->GetStatas().fHealthPoint/
						(*iter)->GetStatas().fHealthPointMAX;
			}
		}
	}
}

void CUIObj::CheckKey()
{
	// 시간
	static float fTimeButton = 0.0f;

	// 시간값 누적
	fTimeButton += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 키매니저
	m_dwKey = GET_SINGLE(CKeyMgr)->GetKey();

	if ( m_dwKey & KEY_I && fTimeButton > 1.f)
	{
		m_InvenKey = !m_InvenKey;
		fTimeButton = 0.f;
	}
	if ( m_dwKey & KEY_K && fTimeButton > 1.f)
	{
		m_SkillTree = !m_SkillTree;
		fTimeButton = 0.f;
	}
	if ( m_dwKey & KEY_B && fTimeButton > 1.f)
	{
		m_Bank = !m_Bank;
		fTimeButton = 0.f;
	}
	if ( m_dwKey & KEY_C && fTimeButton > 1.f)
	{
		m_Character = !m_Character;
		fTimeButton = 0.f;
	}
}


POINT CUIObj::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

void CUIObj::FontOutput( TCHAR* _pObjKey )
{

	TCHAR* NumberFont[] = {FONT0, FONT1, FONT2, FONT3, FONT4, FONT5, FONT6, FONT7, FONT8, FONT9 };

	//DebugLog(L"%d, %d, %d", 
	//	((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel, 
	//	((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel/10, 
	//	temp);

	if (m_pObjKey == L"BoneSpearButton")
	{
		for (list<CObj*>::iterator iter = m_PlayerData->begin();
			iter != m_PlayerData->end(); ++iter)
		{
			if((*iter)->GetSelect() == true)
			{
				int temp = 0;
				int temp2 = 0;
				temp = ((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel % 10;
				temp2 = ((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel/10;

				const TEXINFO* pTexInfo 
					= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp]);

				m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x - 62,
					m_Info.vCenter.y - 60, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


				if(temp2 > 0)
				{
					const TEXINFO* pTexInfo 
						= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp2]);

					m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x + 10,
						m_Info.vCenter.y , 0);

					GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
						NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				DebugLog(L"%d, %d, %d", 
					((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel, 
					((CPlayer*)(*iter))->GetSkillTree().sBoneSpear.iLevel/10,
					temp);
			}
		}
	}
	else if (m_pObjKey == L"FireWallButton")
	{
		for (list<CObj*>::iterator iter = m_PlayerData->begin();
			iter != m_PlayerData->end(); ++iter)
		{
			if((*iter)->GetSelect() == true)
			{
				int temp = 0;
				int temp2 = 0;
				temp = ((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel % 10;
				temp2 = ((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel / 10;

				const TEXINFO* pTexInfo 
					= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp]);

				m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x - 62,
					m_Info.vCenter.y - 60, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


				if(temp2 > 0)
				{
					const TEXINFO* pTexInfo 
						= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp2]);

					m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x + 10,
						m_Info.vCenter.y , 0);

					GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
						NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				DebugLog(L"%d, %d, %d", 
					((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel, 
					((CPlayer*)(*iter))->GetSkillTree().sFireWall.iLevel/10,
					temp);
			}
		}
	}
	else if (m_pObjKey == L"BlizzardButton")
	{
		for (list<CObj*>::iterator iter = m_PlayerData->begin();
			iter != m_PlayerData->end(); ++iter)
		{
			if((*iter)->GetSelect() == true)
			{
				int temp = 0;
				int temp2 = 0;
				temp = ((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iLevel % 10;
				temp2 = ((CPlayer*)(*iter))->GetSkillTree().sBlizzard.iLevel/10;

				const TEXINFO* pTexInfo 
					= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp]);

				m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x - 62,
					m_Info.vCenter.y - 60, 0);

				GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
					NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


				if(temp2 > 0)
				{
					const TEXINFO* pTexInfo 
						= GET_SINGLE(CTextureMgr)->GetTexture(NumberFont[temp2]);

					m_Info.vCenter = D3DXVECTOR3(m_Info.vCenter.x + 10,
						m_Info.vCenter.y , 0);

					GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
						NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}
}
