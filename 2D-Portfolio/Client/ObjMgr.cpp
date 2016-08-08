#include "StdAfx.h"
#include "ObjMgr.h"
#include "Include.h"
#include "SortMgr.h"

CObjMgr::CObjMgr(void)
{


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
	
	// pObjKey로 받은 값과 같은 객체가 없으면 
	if(iter == m_MapObject.end())
	{
		if (pObjKey == L"Fellow_First" || pObjKey == L"Fellow_Second")
		{
			map<wstring, list<CObj*>>::iterator iterPlayer = 
				m_MapObject.find(L"Player");

			iterPlayer->second.push_back(pProtoInst);
		}
		else
		{
			// 추가 생성 후
			list<CObj*> objlist;
			// 리스트에 넣고
			objlist.push_back(pProtoInst);
			// 맵에 리스트를 넣음
			m_MapObject.insert(make_pair(pObjKey, objlist));
		}
	}
	// 있으면 리스트 맨 뒤에 추가
	else
		iter->second.push_back(pProtoInst);
	return S_OK;
}


SCENEID CObjMgr::Progress()
{
	//if(m_monsterList == NULL)
	//{
	//	map<wstring, list<CObj*>>::iterator iter = 
	//		m_MapObject.find(L"Monster");

	//	m_monsterList = &iter->second;
	//}
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end(); ++iter2)
		{
			
			int iScene = (*iter2)->Progress();
			switch(iScene)
			{
			case SCENEID_NONPASS:
			case SCENEID_STAGE:
			case SCENEID_STAGE2:
			case SCENEID_STAGE3:
				ObjInteraction((*iter2));

				// 몬스터 삭제시 
				MonsterRelease();

				// 삭제된 데이터의 리스트 삭제;
				if(*iter2 == NULL)
				{
					iter2 = iter->second.erase(iter2);
					if (iter2 == iter->second.end())
						break;
					continue;
				}
			}
			if(iter2 == iter->second.end())
				break;

			if(iScene > -1)
				return (SCENEID)iScene;

			if ((SCENEID)iScene == SCENEID_END)
			{
				return SCENEID_END;
			}
			// 0806
			GET_SINGLE(CSortMgr)->SetLayer(*iter2);
		}
	}
	
	return SCENEID_NONPASS;
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

			// 플레이어 혹은 몬스터가 아니면 반복문 처음으로 
			if((*iter2)->GetObjType() != OBJ_PLAYER &&
				(*iter2)->GetObjType() != OBJ_MONSTER)
				continue;

			// 
			switch(pSour->GetObjType()) 
			{
				// 플레이어면
			case OBJ_PLAYER:
				CrashAndSlide((*iter2), pSour);
				AbilityTointeract((*iter2), pSour);

				break;
				// 몬스터면
			case OBJ_MONSTER:
				CrashAndSlide((*iter2), pSour);
				AbilityTointeract((*iter2), pSour);
				break;
			};
		}
	}
}

void CObjMgr::AbilityTointeract(CObj* _pDest, CObj* _pSour)
{
	D3DXVECTOR3 InfoDir;

	// 비교할 대상이 있는 방향을 정하고
	InfoDir = _pDest->GetInfoPos().vPos - _pSour->GetInfoPos().vPos;
	// 거리를 구한다.
	float fDistance = ((_pSour->GetInfoPos().fCX *0.5f) +
		(_pDest->GetInfoPos().fCX)*0.5f);
	// 실제 거리, 객체간의 중심점간의 거리
	float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

	// 비교하려는 대상의 타입
	switch(_pSour->GetObjType()) 
	{
	// 플레이어면
	case OBJ_PLAYER:
		{
			// 대상이 몬스터일때 
			if(_pDest->GetObjType() == OBJ_MONSTER)
			{
				// 거리가 25 이하일때
				if (fDistance > fRealDistance)
				{
					if (PtInRect(&_pDest->RealRect(), MouseInfo()))
						_pDest->SetCrash(true);
					else
						_pDest->SetCrash(false);

					// 몬스터와 마우스가 충돌하고, 클릭 중일때
					if(GetAsyncKeyState(VK_LBUTTON)&0x8000 
						&& PtInRect(&_pDest->RealRect(), MouseInfo()))
						AttackFunc(_pDest, _pSour);
				}
			}
			break;
		}
	case OBJ_MONSTER:
		{
			// 대상이 플레이어 일때
			if(_pDest->GetObjType() == OBJ_PLAYER)
			{
				// 거리가 20 이하일때
				if (fDistance > fRealDistance)
				{
					AttackFunc(_pDest, _pSour);
				}
			}
		}
		break;
	};
}

void CObjMgr::AttackFunc( CObj* _pDest, CObj* _pSour )
{
	if( _pSour->GetObjType() == _pDest->GetObjType())
		return;
	
	// 0808 09:00 에러
	//else if (_pSour->GetObjType() == OBJ_MONSTER)
	//	_pSour->SetMotion(ATTACK);

	if (_pSour->GetFrame().fStart >= _pSour->GetFrame().fLast-3)
	{
		if(_pDest->GetStatasInfo().fDefence >= _pSour->GetStatasInfo().fAttack)
			_pDest->SetStatasInfo()->fHealthPoint -= 1;
		else if(_pDest->GetStatasInfo().fDefence < _pSour->GetStatasInfo().fAttack)
			_pDest->SetStatasInfo()->fHealthPoint -= 
				_pSour->GetStatasInfo().fAttack - _pDest->GetStatasInfo().fDefence;
	}
}

void CObjMgr::CrashAndSlide( CObj* _pDest, CObj* _pSour )
{
	// 0808 09:30 몬스터가 몬스터를 밀때 밀리는 몬스터는 정지해야함
	//			  몬스터가 플레이어를 밀땐 밀지말고 공격
	//			  플레이어가 몬스터를 밀땐 몬스터는 정지하고 공격
	//			  플레이어가 플레이어를 밀땐 정지하고 밀림
	
	if(_pDest->GetObjType() == OBJ_PLAYER)
		return;

	// 비교할 객체가 자신이 아니면
	if(_pDest->GetObjType() == OBJ_MONSTER && _pSour->GetpMotion() == RUN)
	{
		// 비교할 대상이 있는 방향을 정하고
		_pSour->Setinfo()->vDir = _pDest->GetInfoPos().vPos - _pSour->GetInfoPos().vPos;
		
		// 대상체끼리의 표면좌표 더하면 충돌했을때의 거리.
		float fDistance = ((_pSour->GetInfoPos().fCX * 0.5f) +
			(_pDest->GetInfoPos().fCX * 0.5f));

		// 실제 거리, 객체간의 중심점간의 거리
		float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

		// fDistance가 서로간의 중심점에서 표면까지의 거리를 더한것인데
		// 표면에서 "[충돌했을때!]의 중심점간(fDistance)"의 거리는 표면의 합계이다.
		// 이때 중심점끼리의 거리(fRealDistance)는 fDistance 보다 크면 충돌하지 않음
		// fDistance 보다 작으면 충돌
		if(fDistance > fRealDistance)
		{
			// 0806 충돌시 밀어내거나 멈춤
			// 외적을 구함
			// 시계방향으로 밀려나게
			D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
						  &_pSour->GetInfoPos().vDir, 
						  &D3DXVECTOR3(0.f, 0.f, -1.f));
			// 반시계방향으로 밀려나게
			//D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
			//	&D3DXVECTOR3(0.f, 0.f, -1.f), 
			//	&_pSour->GetInfoPos().vDir);

			D3DXVECTOR3 fPushRange = 
				D3DXVECTOR3(fDistance- fRealDistance, fDistance - fRealDistance, 0);

			D3DXVec3Normalize(&fPushRange, 
							  &fPushRange);
			// 정규화
			D3DXVec3Normalize(&_pDest->Setinfo()->vDir, 
							  &_pDest->Setinfo()->vDir);

			// 0808 조건을 바꿔서 겹친거리만 측정해서 밀게할 예정
			// += 는 D3DXVec3Cross 로 구한 정방향, -= 는 반대방향
			// 
			if((_pSour->GetInfoPos().vPos.y) > (_pDest->GetInfoPos().vPos.y))
				_pDest->SetTagetInfo() = _pDest->Setinfo()->vPos += (_pDest->GetInfoPos().vDir - fPushRange);
			else if((_pSour->GetInfoPos().vPos.y) < (_pDest->GetInfoPos().vPos.y))
				_pDest->SetTagetInfo() = _pDest->Setinfo()->vPos += (_pDest->GetInfoPos().vDir + fPushRange);/*
			else if((_pSour->GetInfoPos().vPos.x) < (_pDest->GetInfoPos().vPos.x))
				_pDest->SetTagetInfo() = _pDest->Setinfo()->vPos -= (_pDest->GetInfoPos().vDir - fPushRange);*/

		}
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

void CObjMgr::MonsterRelease()
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
				// 체력체크 후
				if((*iter2)->GetpMotion() == DEATH && 
					(*iter2)->GetFrame().fStart > (*iter2)->GetFrame().fLast-1)
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
			else
				// 몬스터가 아닌 노드면 다음 노드로
				++iter2;
			// 노드가 끝이면 벗어남
			if (iter2 == iter->second.end())
				break;
		}
	}
}