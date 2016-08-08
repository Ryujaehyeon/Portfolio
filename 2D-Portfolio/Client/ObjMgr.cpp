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

	// Ű���� �ް� �ʿ��� �ش� Ű���� ������ ����Ʈ�� ã��
	map<wstring, list<CObj*>>::iterator iter = 
		m_MapObject.find(pObjKey);

	// ���� ��ü�� ������� ������ ��ü�� ���� ��ü�� �����ؿ�
	CObj* pProtoInst = ((CObjProto*)pProto)->GetProto(pObjKey);

	// ���� ��ü�� ������ ������Ʈ ���� ����
	if(pProtoInst == NULL)
		return E_FAIL;

	// ������ �����ϸ� �ʱ�ȭ
	if(FAILED(pProtoInst->Initialize()))
		return E_FAIL;
	
	// pObjKey�� ���� ���� ���� ��ü�� ������ 
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
			// �߰� ���� ��
			list<CObj*> objlist;
			// ����Ʈ�� �ְ�
			objlist.push_back(pProtoInst);
			// �ʿ� ����Ʈ�� ����
			m_MapObject.insert(make_pair(pObjKey, objlist));
		}
	}
	// ������ ����Ʈ �� �ڿ� �߰�
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

				// ���� ������ 
				MonsterRelease();

				// ������ �������� ����Ʈ ����;
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

			// �÷��̾� Ȥ�� ���Ͱ� �ƴϸ� �ݺ��� ó������ 
			if((*iter2)->GetObjType() != OBJ_PLAYER &&
				(*iter2)->GetObjType() != OBJ_MONSTER)
				continue;

			// 
			switch(pSour->GetObjType()) 
			{
				// �÷��̾��
			case OBJ_PLAYER:
				CrashAndSlide((*iter2), pSour);
				AbilityTointeract((*iter2), pSour);

				break;
				// ���͸�
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

	// ���� ����� �ִ� ������ ���ϰ�
	InfoDir = _pDest->GetInfoPos().vPos - _pSour->GetInfoPos().vPos;
	// �Ÿ��� ���Ѵ�.
	float fDistance = ((_pSour->GetInfoPos().fCX *0.5f) +
		(_pDest->GetInfoPos().fCX)*0.5f);
	// ���� �Ÿ�, ��ü���� �߽������� �Ÿ�
	float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

	// ���Ϸ��� ����� Ÿ��
	switch(_pSour->GetObjType()) 
	{
	// �÷��̾��
	case OBJ_PLAYER:
		{
			// ����� �����϶� 
			if(_pDest->GetObjType() == OBJ_MONSTER)
			{
				// �Ÿ��� 25 �����϶�
				if (fDistance > fRealDistance)
				{
					if (PtInRect(&_pDest->RealRect(), MouseInfo()))
						_pDest->SetCrash(true);
					else
						_pDest->SetCrash(false);

					// ���Ϳ� ���콺�� �浹�ϰ�, Ŭ�� ���϶�
					if(GetAsyncKeyState(VK_LBUTTON)&0x8000 
						&& PtInRect(&_pDest->RealRect(), MouseInfo()))
						AttackFunc(_pDest, _pSour);
				}
			}
			break;
		}
	case OBJ_MONSTER:
		{
			// ����� �÷��̾� �϶�
			if(_pDest->GetObjType() == OBJ_PLAYER)
			{
				// �Ÿ��� 20 �����϶�
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
	
	// 0808 09:00 ����
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
	// 0808 09:30 ���Ͱ� ���͸� �ж� �и��� ���ʹ� �����ؾ���
	//			  ���Ͱ� �÷��̾ �ж� �������� ����
	//			  �÷��̾ ���͸� �ж� ���ʹ� �����ϰ� ����
	//			  �÷��̾ �÷��̾ �ж� �����ϰ� �и�
	
	if(_pDest->GetObjType() == OBJ_PLAYER)
		return;

	// ���� ��ü�� �ڽ��� �ƴϸ�
	if(_pDest->GetObjType() == OBJ_MONSTER && _pSour->GetpMotion() == RUN)
	{
		// ���� ����� �ִ� ������ ���ϰ�
		_pSour->Setinfo()->vDir = _pDest->GetInfoPos().vPos - _pSour->GetInfoPos().vPos;
		
		// ���ü������ ǥ����ǥ ���ϸ� �浹�������� �Ÿ�.
		float fDistance = ((_pSour->GetInfoPos().fCX * 0.5f) +
			(_pDest->GetInfoPos().fCX * 0.5f));

		// ���� �Ÿ�, ��ü���� �߽������� �Ÿ�
		float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

		// fDistance�� ���ΰ��� �߽������� ǥ������� �Ÿ��� ���Ѱ��ε�
		// ǥ�鿡�� "[�浹������!]�� �߽�����(fDistance)"�� �Ÿ��� ǥ���� �հ��̴�.
		// �̶� �߽��������� �Ÿ�(fRealDistance)�� fDistance ���� ũ�� �浹���� ����
		// fDistance ���� ������ �浹
		if(fDistance > fRealDistance)
		{
			// 0806 �浹�� �о�ų� ����
			// ������ ����
			// �ð�������� �з�����
			D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
						  &_pSour->GetInfoPos().vDir, 
						  &D3DXVECTOR3(0.f, 0.f, -1.f));
			// �ݽð�������� �з�����
			//D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
			//	&D3DXVECTOR3(0.f, 0.f, -1.f), 
			//	&_pSour->GetInfoPos().vDir);

			D3DXVECTOR3 fPushRange = 
				D3DXVECTOR3(fDistance- fRealDistance, fDistance - fRealDistance, 0);

			D3DXVec3Normalize(&fPushRange, 
							  &fPushRange);
			// ����ȭ
			D3DXVec3Normalize(&_pDest->Setinfo()->vDir, 
							  &_pDest->Setinfo()->vDir);

			// 0808 ������ �ٲ㼭 ��ģ�Ÿ��� �����ؼ� �а��� ����
			// += �� D3DXVec3Cross �� ���� ������, -= �� �ݴ����
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
	// ���� first, ó������ ������ ���� �ݺ���
	for(map<wstring, list<CObj*>>::iterator iter
		= m_MapObject.begin(); iter != m_MapObject.end(); ++iter)
	{
		// ���� second, second�� ����Ʈ�� ������ ����Ʈ�� ������ ���� �ݺ���
		for(list<CObj*>::iterator iter2 = iter->second.begin();
			iter2 != iter->second.end();)
		{
			// ���͸�
			if((*iter2)->GetObjType() == OBJ_MONSTER)
			{
				// ü��üũ ��
				if((*iter2)->GetpMotion() == DEATH && 
					(*iter2)->GetFrame().fStart > (*iter2)->GetFrame().fLast-1)
				{
					// �Ҵ�� ���� ��ü ����
					SAFE_DELETE<CObj>(&(*iter2));
					// ���� ����
					++iter2;
				}
				else
					// ���� ����
					++iter2;
				// iter2�� ����� ���̸� ��� 
				if (iter2 == iter->second.end())
					break;
			}
			else
				// ���Ͱ� �ƴ� ���� ���� ����
				++iter2;
			// ��尡 ���̸� ���
			if (iter2 == iter->second.end())
				break;
		}
	}
}