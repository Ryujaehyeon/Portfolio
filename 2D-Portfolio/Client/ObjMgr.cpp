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

	switch(pProtoInst->GetObjType())
	{
	case OBJ_PLAYER:
		{
			map<wstring, list<CObj*>>::iterator iterPLAYER = 
				m_MapObject.find(L"Player");
			// Ű�� �÷��̾��� ����Ʈ�� ������
			if (iterPLAYER == m_MapObject.end())
			{
				// �߰� ���� ��
				list<CObj*> objlist;
				// ����Ʈ�� �ְ�
				objlist.push_back(pProtoInst);
				// �ʿ� ����Ʈ�� ����
				m_MapObject.insert(make_pair(L"Player", objlist));
			}
			else
				iterPLAYER->second.push_back(pProtoInst);

			break;
		}
	case OBJ_MONSTER:
		{
			map<wstring, list<CObj*>>::iterator iterMonster = 
				m_MapObject.find(L"Monster");
			// Ű�� �÷��̾��� ����Ʈ�� ������
			if (iterMonster == m_MapObject.end())
			{
				// �߰� ���� ��
				list<CObj*> objlist;
				// ����Ʈ�� �ְ�
				objlist.push_back(pProtoInst);
				// �ʿ� ����Ʈ�� ����
				m_MapObject.insert(make_pair(L"Monster", objlist));
			}
			else
				iterMonster->second.push_back(pProtoInst);

			break;
		}

	//case OBJ_NONPLAYER:
	//	{
	//		map<wstring, list<CObj*>>::iterator iterNONPLAYER = 
	//			m_MapObject.find(L"Npc");
	//		// Ű�� �÷��̾��� ����Ʈ�� ������
	//		if (iterNONPLAYER == m_MapObject.end())
	//		{
	//			// �߰� ���� ��
	//			list<CObj*> objlist;
	//			// ����Ʈ�� �ְ�
	//			objlist.push_back(pProtoInst);
	//			// �ʿ� ����Ʈ�� ����
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
	//		// Ű�� �÷��̾��� ����Ʈ�� ������
	//		if (iterBROKENOBJ == m_MapObject.end())
	//		{
	//			// �߰� ���� ��
	//			list<CObj*> objlist;
	//			// ����Ʈ�� �ְ�
	//			objlist.push_back(pProtoInst);
	//			// �ʿ� ����Ʈ�� ����
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
	//		// Ű�� �÷��̾��� ����Ʈ�� ������
	//		if (iterBUILDING == m_MapObject.end())
	//		{
	//			// �߰� ���� ��
	//			list<CObj*> objlist;
	//			// ����Ʈ�� �ְ�
	//			objlist.push_back(pProtoInst);
	//			// �ʿ� ����Ʈ�� ����
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
			// Ű�� �÷��̾��� ����Ʈ�� ������
			if (iterBUTTON == m_MapObject.end())
			{
				// �߰� ���� ��
				list<CObj*> objlist;
				// ����Ʈ�� �ְ�
				objlist.push_back(pProtoInst);
				// �ʿ� ����Ʈ�� ����
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
			// Ű�� �÷��̾��� ����Ʈ�� ������
			if (iterUI == m_MapObject.end())
			{
				// �߰� ���� ��
				list<CObj*> objlist;
				// ����Ʈ�� �ְ�
				objlist.push_back(pProtoInst);
				// �ʿ� ����Ʈ�� ����
				m_MapObject.insert(make_pair(L"UI", objlist));
			}
			else
				iterUI->second.push_back(pProtoInst);
			break;
		}
	default:
		{
			// pObjKey�� ���� ���� ���� ��ü�� ������ 
			if(iter == m_MapObject.end())
			{
				// �߰� ���� ��
				list<CObj*> objlist;
				// ����Ʈ�� �ְ�
				objlist.push_back(pProtoInst);
				// �ʿ� ����Ʈ�� ����
				m_MapObject.insert(make_pair(pObjKey, objlist));
			}
			else // ������ ����Ʈ �� �ڿ� �߰�
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
			iScene = (*iter2)->Progress();

			switch((*iter2)->GetObjType())
			{
			case OBJ_PLAYER:
			case OBJ_MONSTER:
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
				break;
			case OBJ_UI:
				{
					//map<wstring, list<CObj*>> finditer = m_MapObject.find(PLAYER);
					//list<CObj*> iterPLAYER = finditer->second;
					//if(iterPLAYER != iterPLAYER)
					// Ȥ�� �� ����ó���� �ʿ��ҵ� �� 
						static_cast<CUIObj*>(*iter2)->Setlist(&m_MapObject.find(PLAYER)->second);
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



void CObjMgr::CrashAndSlide( CObj* _pDest, CObj* _pSour )
{
	if(_pDest->GetObjType() != OBJ_PLAYER 
		&& _pDest->GetObjType() != OBJ_MONSTER) 
		return;
	// ���� ����� �ִ� ������ ���ϰ�
	_pSour->Setinfo()->vDir = _pDest->GetInfo().vPos - _pSour->GetInfo().vPos;

	// Ÿ���� ���ϱ� ���� �Ÿ�
	{
		// ���� ũ�⿡ ���� �浹
		float fDistance = ((_pSour->GetInfo().fCX * 0.7f) +
			(_pDest->GetInfo().fCX * 0.7f));

		// ���� �Ÿ�, ��ü���� �߽������� �Ÿ�
		float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);
		if(fDistance > fRealDistance)
		{
			if(_pSour->GetObjType() != _pDest->GetObjType())
			{
				// �浹 ����
				_pSour->SetCrash(true);
				_pDest->SetCrash(true);
				// Ÿ�� ����
				_pSour->SetTagetObj(_pDest);
				_pDest->SetTagetObj(_pSour);
			}
		}
	}

	// �浹�� �о��ֱ� ���� �Ÿ�
	// �浹 �Ÿ�
	float fDistance = ((_pSour->GetInfo().fCX * 0.4f) +
		(_pDest->GetInfo().fCX * 0.4f));

	// ���� �Ÿ�, ��ü���� �߽������� �Ÿ�
	float fRealDistance = D3DXVec3Length(&_pSour->Setinfo()->vDir);

	// fDistance�� ���ΰ��� �߽������� ǥ������� �Ÿ��� ���Ѱ��ε�
	// ǥ�鿡�� "[�浹������!]�� �߽�����(fDistance)"�� �Ÿ��� ǥ���� �հ��̴�.
	// �̶� �߽��������� �Ÿ�(fRealDistance)�� fDistance ���� ũ�� �浹���� ����
	// fDistance ���� ������ �浹

	if(fDistance > fRealDistance)
	{
		// ������ ���Ѵ�
		// �а��ִ� ����� ������ ������ �и��� ����� ������ �����
		D3DXVec3Cross(&_pDest->Setinfo()->vDir, 
			&_pSour->GetInfo().vDir, 
			&D3DXVECTOR3(0.f, 0.f, -1.f));

		// �浹�ؼ� ��ģ��ŭ �о��ֱ� ���� ��ģ �Ÿ��� ����
		float fPushRange = fDistance - fRealDistance;

		// ����ȭ , 
		D3DXVec3Normalize(&_pSour->Setinfo()->vDir, 
			&_pSour->Setinfo()->vDir);

		// �̴� ������ �и��Բ�
		_pDest->Setinfo()->vPos += (_pSour->Setinfo()->vDir * fPushRange);
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
				// �׾����� ����üũ ��
				if((*iter2)->GetpMotion() == DEATH && 
					(*iter2)->GetFrame().fStart >= (*iter2)->GetFrame().fLast-1)
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
