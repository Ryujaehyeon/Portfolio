#include "StdAfx.h"
#include "ObjMgr.h"
#include "Include.h"
#include "SortMgr.h"

CObjMgr::CObjMgr(void)
{
	m_MonsterCount = 0;
	m_BossCount = 0;
}

CObjMgr::~CObjMgr(void)
{
	Release();
}

HRESULT CObjMgr::AddObject(CPrototype* pProto, wstring pObjKey)
{
	TCHAR* Charter[3] = {L"Player", 
		L"Fellow_First",
		L"Fellow_Second"};

	// 키값을 받고 맵에서 해당 키값과 동일한 리스트를 찾음
	map<wstring, list<CObj*>>::iterator iter = 
		m_MapObject.find(pObjKey);

	// 원형 객체인 프로토로 생성할 객체의 원형 객체를 복사해옴
	CObj* pProtoInst = ((CObjProto*)pProto)->GetProto(pObjKey);

	// 원형 객체가 없으면 오브젝트 생성 실패
	if(pProtoInst == NULL)
		return E_FAIL;

	// 생성에 성공하면 초기화
	if(FAILED(pProtoInst->Initialize()))
		return E_FAIL;

	switch(pProtoInst->GetObjType())
	{
	case OBJ_PLAYER:
		{
			map<wstring, list<CObj*>>::iterator iterPLAYER = 
				m_MapObject.find(PLAYER);
			// 키가 플레이어인 리스트가 없으면
			if (iterPLAYER == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(PLAYER, objlist));
			}
			else
				iterPLAYER->second.push_back(pProtoInst);

			break;
		}
	case OBJ_SKILL:
		{
			
			map<wstring, list<CObj*>>::iterator iterSKILL = 
				m_MapObject.find(SKILL);
			map<wstring, list<CObj*>>::iterator iterPLAYER = 
				m_MapObject.find(PLAYER);
			list<CObj*>::iterator iterPlayerList = iterPLAYER->second.begin();

			for (;iterPlayerList != iterPLAYER->second.end(); ++iterPlayerList)
			{
				// 플레이어 위치에서 스킬이 발사되게끔 함
				// 플레이어 스테이터스에 따라 스킬의 위력이 다르게 변경 가능
				if(pObjKey == BONESPEAR)
				{
					pProtoInst->SetStatas()->iLevel = ((CPlayer*)(*iterPlayerList))->GetSkillTree().sBoneSpear.iLevel;
				}
				else if(pObjKey == FIREWALL)
				{
					pProtoInst->SetStatas()->iLevel = ((CPlayer*)(*iterPlayerList))->GetSkillTree().sFireWall.iLevel;
				}
				else if (pObjKey == BLIZZARD)
				{
					pProtoInst->SetStatas()->iLevel = ((CPlayer*)(*iterPlayerList))->GetSkillTree().sBlizzard.iLevel;
				}

				if((*iterPlayerList)->GetSelect() == true)
					pProtoInst->Setinfo()->vPos = (*iterPlayerList)->GetInfo().vPos;
			}
			// 키가 플레이어인 리스트가 없으면
			if (iterSKILL == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(SKILL, objlist));
			}
			else
				iterSKILL->second.push_back(pProtoInst);

			break;
		}
	case OBJ_MONSTER:
		{

			map<wstring, list<CObj*>>::iterator iterMonster = 
				m_MapObject.find(MONSTER);
			// 키가 플레이어인 리스트가 없으면
			if (iterMonster == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(MONSTER, objlist));
			}
			else
				iterMonster->second.push_back(pProtoInst);

			break;
		}

	//case OBJ_NONPLAYER:
	//	{
	//		map<wstring, list<CObj*>>::iterator iterNONPLAYER = 
	//			m_MapObject.find(L"Npc");
	//		// 키가 플레이어인 리스트가 없으면
	//		if (iterNONPLAYER == m_MapObject.end())
	//		{
	//			// 추가 생성 후
	//			list<CObj*> objlist;
	//			// 리스트에 넣고
	//			objlist.push_back(pProtoInst);
	//			// 맵에 리스트를 넣음
	//			m_MapObject.insert(make_pair(L"Npc", objlist));
	//		}
	//		else
	//			iterNONPLAYER->second.push_back(pProtoInst);

	//		break;
	//	}

	//case OBJ_BROKENBUILDING:
	//	{
	//		map<wstring, list<CObj*>>::iterator iterBROKENOBJ = 
	//			m_MapObject.find(L"BrokenObj");
	//		// 키가 플레이어인 리스트가 없으면
	//		if (iterBROKENOBJ == m_MapObject.end())
	//		{
	//			// 추가 생성 후
	//			list<CObj*> objlist;
	//			// 리스트에 넣고
	//			objlist.push_back(pProtoInst);
	//			// 맵에 리스트를 넣음
	//			m_MapObject.insert(make_pair(L"BrokenObj", objlist));
	//		}
	//		else
	//			iterBROKENOBJ->second.push_back(pProtoInst);

	//		break;
	//	}

	//case OBJ_BUILDING:
	//	{
	//		map<wstring, list<CObj*>>::iterator iterBUILDING = 
	//			m_MapObject.find(L"Building");
	//		// 키가 플레이어인 리스트가 없으면
	//		if (iterBUILDING == m_MapObject.end())
	//		{
	//			// 추가 생성 후
	//			list<CObj*> objlist;
	//			// 리스트에 넣고
	//			objlist.push_back(pProtoInst);
	//			// 맵에 리스트를 넣음
	//			m_MapObject.insert(make_pair(L"Building", objlist));
	//		}
	//		else
	//			iterBUILDING->second.push_back(pProtoInst);

	//		break;
	//	}
	case OBJ_BUTTON:
		{
			map<wstring, list<CObj*>>::iterator iterBUTTON = 
				m_MapObject.find(L"Button");
			// 키가 플레이어인 리스트가 없으면
			if (iterBUTTON == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(L"Button", objlist));
			}
			else
				iterBUTTON->second.push_back(pProtoInst);

			break;
		}

	case OBJ_UI:
		{
			map<wstring, list<CObj*>>::iterator iterUI = 
				m_MapObject.find(L"UI");
			// 키가 플레이어인 리스트가 없으면
			if (iterUI == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(L"UI", objlist));
			}
			else
				iterUI->second.push_back(pProtoInst);
			break;
		}
	default:
		{
			// pObjKey로 받은 값과 같은 객체가 없으면 
			if(iter == m_MapObject.end())
			{
				// 추가 생성 후
				list<CObj*> objlist;
				// 리스트에 넣고
				objlist.push_back(pProtoInst);
				// 맵에 리스트를 넣음
				m_MapObject.insert(make_pair(pObjKey, objlist));
			}
			else // 있으면 리스트 맨 뒤에 추가
				iter->second.push_back(pProtoInst);

			break;
		}
	}
	return S_OK;
}

SCENEID CObjMgr::Progress()
{


	SCENEID iScene;
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			map<wstring, list<CObj*>>::iterator iterPlayer = m_MapObject.find(PLAYER);
			map<wstring, list<CObj*>>::iterator iterMonster = m_MapObject.find(MONSTER);
			map<wstring, list<CObj*>>::iterator iterSkill = m_MapObject.find(SKILL);
						
			//// 플레이어 정보를 보기 위함
			//if((*iter2)->GetName() == PLAYER)
			//DebugLog(L"레벨 : %d \n체력 : %8.3f,%8.3f \n경험치 : %8.3f/%8.3f \n공격력 : %8.2f \n방어력 : %8.2f \n힘 : %8.2f \n체질 : %8.2f \n민첩 : %8.2f \n통찰 : %8.2f \n지능 : %8.2f \n결의 %8.2f \n능력포인트 : %d \n기술포인트 : %d",
			//(*iter2)->GetStatas().iLevel, 
			//(*iter2)->GetStatas().fHealthPoint, (*iter2)->GetStatas().fHealthPointMAX, 
			//(*iter2)->GetStatas().fExp,(*iter2)->GetStatas().fMaxExp,
			//(*iter2)->GetStatas().fAttack,(*iter2)->GetStatas().fDefence, 
			//(*iter2)->GetStatas().fMight, 
			//(*iter2)->GetStatas().fConstitution, 
			//(*iter2)->GetStatas().fDexterity, 
			//(*iter2)->GetStatas().fPerception, 
			//(*iter2)->GetStatas().fIntellect, 
			//(*iter2)->GetStatas().fResolve,
			//(*iter2)->GetStatas().iStatPoint,(*iter2)->GetStatas().iSKillPoint);


			iScene = (*iter2)->Progress();
			switch((*iter2)->GetObjType())
			{
			case OBJ_PLAYER:
			case OBJ_SKILL:
			case OBJ_MONSTER:
				if ((*iter2)->GetObjType() == OBJ_PLAYER)
				{					
					if(iterMonster != m_MapObject.end())
						(*iter2)->Setlist(
						&m_MapObject.find(MONSTER)->second);
				}
				else if ((*iter2)->GetObjType() == OBJ_MONSTER)
				{
					// 몬스터 갯수를 측정후 몬스터 카운트에 넣어줌
					m_MonsterCount = iterMonster->second.size();

					m_BossCount = 0;
					for (list<CObj*>::iterator iterCount = iterMonster->second.begin();
						 iterCount != iterMonster->second.end() ; ++iterCount	)
					{
						if ((*iterCount)->GetName() == SUMMONER ||
							(*iterCount)->GetName() == GRISWOLD ||
							(*iterCount)->GetName() == ANDARIEL ||
							(*iterCount)->GetName() == IZUAL ||
							(*iterCount)->GetName() == DIABLO)
						{
							if(m_BossCount < 1)
								++m_BossCount; 
						}
					}


					if(iterPlayer != m_MapObject.end())
						(*iter2)->Setlist(
						&m_MapObject.find(PLAYER)->second);
				}
				else if((*iter2)->GetObjType() == OBJ_SKILL)
				{
					if(iterPlayer != m_MapObject.end())
						(*iter2)->Setlist(
						&m_MapObject.find(PLAYER)->second);

					if(iterMonster != m_MapObject.end() && 
						iterSkill != m_MapObject.end())
						ColCircle(&iterSkill->second, &iterMonster->second);
				}
				ObjInteraction((*iter2));
				// iter 삭제시 
				IterRelease();
				// 삭제된 데이터의 리스트 삭제;
				if(*iter2 == nullptr)
				{
					iter2 = iter->second.erase(iter2);
					if (iter2 == iter->second.end())
						break;
					continue;
				}
				break;
			case OBJ_UI:
				{
					// 플레이어 정보
					if(iterPlayer != m_MapObject.end())
					{
						static_cast<CPlayer*>(*iter2)->Setlist(
							&m_MapObject.find(PLAYER)->second);
					}

					// 몬스터 정보
					if(iterMonster != m_MapObject.end())
					{
						static_cast<CMonster*>(*iter2)->Setlist(
							&m_MapObject.find(MONSTER)->second);
					}
					break;
				}
			}
			if(iter2 == iter->second.end())
				break;

			if(iScene > SCENEID_NONPASS)
				return iScene;

			if ((SCENEID)iScene == SCENEID_END)
			{
				return SCENEID_END;
			}
			// 0806
			GET_SINGLE(CSortMgr)->SetLayer(*iter2);
		}
	}

	return iScene;
}


void CObjMgr::ObjInteraction( CObj* pSour )
{
	D3DXVECTOR3 InfoDir;
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			if(pSour == (*iter2))
				continue;

			CrashAndSlide((*iter2), pSour);
			AbilityTointeract((*iter2), pSour);
		}
	}
}

void CObjMgr::AbilityTointeract(CObj* _pDest, CObj* _pSour)
{

}



bool CObjMgr::CrashAndSlide( CObj* _pDest, CObj* _pSour )
{
	// UI나 건물형 객체를 밀지 않게
	if(_pDest->GetObjType() == OBJ_UI 
		&& _pDest->GetObjType() != OBJ_BUILDING) 
		return false;

	// 자신의 스킬이 자신을 밀지 않게
	if (_pSour->GetObjType() == OBJ_SKILL
		&& _pDest->GetObjType() == OBJ_PLAYER)
		return false;

	// 스킬이 스킬을 밀지 않게
	else if (_pSour->GetObjType() == OBJ_SKILL
		&& _pDest->GetObjType() == OBJ_SKILL)
		return false;

	// 플레이어가 스킬을 밀지 않게
	else if ( _pSour->GetObjType() == OBJ_PLAYER
		&& _pDest->GetObjType() == OBJ_SKILL)
		return false;

	

	// 비교할 대상이 있는 방향을 정하고
	_pSour->Setinfo()->vDir = (_pDest->GetInfo().vPos + CObj::g_tScroll) - (_pSour->GetInfo().vPos + CObj::g_tScroll);

	// 타겟을 정하기 위한 거리
	{
		// 실제 크기에 따른 충돌
		float fDistance = ((_pSour->GetInfo().fCX * 0.7f) +
			(_pDest->GetInfo().fCX * 0.7f));

		// 실제 거리, 객체간의 중심점간의 거리
		float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

		// fDistance * 3.f는 자신의 크기보다 실제로 충돌하는 크기를 늘려서
		// 다른대상을 인지하는 거리로 삼아 이 거리 안으로 들어오면 충돌했다 판단하고
		// 객체에 충돌했다고 넘겨줌으로 객체내에서 충돌로 인한 AI작동을 하게 함
		if(fDistance * 5.f > fRealDistance)
		{
			if(_pSour->GetObjType() != _pDest->GetObjType())
			{
				// 충돌 여부
				_pSour->SetCrash(true);
				_pDest->SetCrash(true);

				// 타겟 설정
				//_pSour->SetTagetObj(_pDest);
				//_pDest->SetTagetObj(_pSour);
			}
		}
	}

	// 충돌시 밀어주기 위한 거리
	// 충돌 거리
	float fDistance = ((_pSour->GetInfo().fCX * 0.4f) +
		(_pDest->GetInfo().fCX * 0.4f));

	// 실제 거리, 객체간의 중심점간의 거리
	float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

	// fDistance가 서로간의 중심점에서 표면까지의 거리를 더한것인데
	// 표면에서 "[충돌했을때!]의 중심점간(fDistance)"의 거리는 표면의 합계이다.
	// 이때 중심점끼리의 거리(fRealDistance)는 fDistance 보다 크면 충돌하지 않음
	// fDistance 보다 작으면 충돌
	if(fDistance > fRealDistance)
	{
		// 스킬, 몬스터가 서로를 밀지 않게 충돌하면 충돌했다고 리턴해줌
		if (_pSour->GetObjType() == OBJ_SKILL
		&& _pDest->GetObjType() == OBJ_MONSTER
		|| _pSour->GetObjType() == OBJ_MONSTER
		&& _pDest->GetObjType() == OBJ_SKILL)
		return true;

		// 외적을 구한다
		// 밀고있는 대상의 방향의 옆으로 밀리는 대상의 방향을 잡아줌
		D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
			&_pSour->GetInfo().vDir, 
			&D3DXVECTOR3(0.f, 0.f, -1.f));

		// 충돌해서 겹친만큼 밀어주기 위해 겹친 거리를 구함
		float fPushRange = fDistance - fRealDistance;

		// 정규화 , 
		D3DXVec3Normalize(&_pSour->Setinfo()->vDir, 
			&_pSour->Setinfo()->vDir);

		// 미는 방향대로 밀리게끔
		_pDest->Setinfo()->vPos += (_pSour->Setinfo()->vDir * fPushRange);
		return true;
	}
	return false;
}

void CObjMgr::ColCircle(list<CObj*> *pSkill, 
	list<CObj*> *pMonsterList)
{
	for (list<CObj*>::iterator iter = pSkill->begin();
		iter != pSkill->end(); ++iter)
	{
		for (list<CObj*>::iterator iter2 = pMonsterList->begin();
			iter2 != pMonsterList->end();)
		{
			if ((*iter) == nullptr || (*iter2) == nullptr)
				break;

			if (CrashAndSlide(*iter, *iter2))
			{
				// 몬스터 방어력이 플레이어 공격력보다 크면 데미지 
				if((*iter2)->GetStatas().fDefence >= (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 1;
				// 플레이어 공격력이 더 크면 몬스터 방어력을 뺀 나머지 만큼의 데미지만 적용 
				else if((*iter2)->GetStatas().fDefence < (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 
					(*iter)->GetStatas().fAttack - (*iter2)->GetStatas().fDefence;

				if((*iter)->GetName() == BONESPEAR)
					(*iter)->SetStatas()->fHealthPoint = -1.f;


				//충돌시 데미지
				//SAFE_DELETE(&(*iter));
				//SAFE_DELETE(&(*iter2));
				
				//list의 노드를 삭제
				//iter = pSkill->erase(iter);
				//iter2 = pMonsterList->erase(iter2);
				++iter;
				++iter2;
			}
			else //충돌 실패시
			{
				//좌표를 받아 화면 밖을 벗어날 시
				if( (*iter)->GetInfo().vPos.y < 0		 + CObj::g_tScroll.y  ||
					(*iter)->GetInfo().vPos.y > WINSIZEY + CObj::g_tScroll.y  ||
					(*iter)->GetInfo().vPos.x < 0		 + CObj::g_tScroll.x  ||
					(*iter)->GetInfo().vPos.x > WINSIZEX + CObj::g_tScroll.x	)
				{

					//해제 및 노드 삭제
					//SAFE_DELETE(&(*iter));
					//iter = pSkill->erase(iter);
					if((*iter)->GetName() == BONESPEAR)
						(*iter)->SetStatas()->fHealthPoint = -1.f;
					++iter;
				}
				//다음 노드를 가르킨다.
				++iter2;
			}
			//iter가 끝을 가르키면 반복문을 끝냄
			if (iter == pSkill->end())
				break;
		}
		//iter가 끝을 가르키면 반복문을 끝냄
		if (iter == pSkill->end())
			break;
	}
}

POINT CObjMgr::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CObjMgr::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}


void CObjMgr::Render()
{
	GET_SINGLE(CSortMgr)->Render();
}

void CObjMgr::Release()
{
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			SAFE_DELETE<CObj>(&(*iter2));
		}
		iter->second.clear();
	}
	m_MapObject.clear();

	// 
	DESTROY_SINGLE(CSortMgr);
}

const vector<TILE*>* CObjMgr::GetTileInfo()
{
	map<wstring, list<CObj*>>::iterator iter = 
		m_MapObject.find(L"StageBack");
	if (iter == m_MapObject.end())
		return NULL;
	return static_cast<CStageBackGround*>(iter->second.front())->GetVecTile();
}

void CObjMgr::IterRelease()
{
	// 맵의 first, 처음부터 끝까지 도는 반복문
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		// 맵의 second, second엔 리스트가 있으며 리스트의 끝까지 도는 반복문
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end();)
		{
			// 몬스터면
			if((*iter2)->GetObjType() == OBJ_MONSTER)
			{
				// 죽었는지 상태체크 후
				// 0817 오전 몬스터가 제대로 삭제되지 않음
				if((*iter2)->GetpMotion() == DEATH && 
					(*iter2)->GetFrame().fStart <= 0.f/*>= (*iter2)->GetFrame().fLast-2*/)
				{
					// 할당된 몬스터 객체 삭제
					SAFE_DELETE<CObj>(&(*iter2));
					// 다음 노드로
					++iter2;
				}
				else
					// 다음 노드로
					++iter2;
				// iter2가 노드의 끝이면 벗어남 
				if (iter2 == iter->second.end())
					break;
			}
			else if ((*iter2)->GetObjType() == OBJ_SKILL)
			{
				if((*iter2)->GetpMotion() == DEATH)
				{
					// 할당된 스킬 객체 삭제
					SAFE_DELETE<CObj>(&(*iter2));
					// 다음 노드로
					++iter2;
				}
				else
					// 다음 노드로
					++iter2;
				// iter2가 노드의 끝이면 벗어남 
				if (iter2 == iter->second.end())
					break;
			}
			else
				// 몬스터가 아닌 노드면 다음 노드로
				++iter2;
			// 노드가 끝이면 벗어남
			if (iter2 == iter->second.end())
				break;
		}
	}
}
