#include "StdAfx.h"
#include "SortMgr.h"
#include "Include.h"

CSortMgr::CSortMgr(void)
{
}


CSortMgr::~CSortMgr(void)
{
	Release();
}

void CSortMgr::SetLayer( CObj* pObj )
{
	if(pObj != NULL)
		m_SortLayer[pObj->GetLayer()].push_back(pObj);
}

void CSortMgr::Render()
{
	// y 축에 따라 먼저 그려지고 늦게 그려지게 정렬
	//for (list<CObj*>::iterator iter = m_SortLayer[i].begin();
	//	iter != m_SortLayer[i].end(); ++iter)
	//{
	//	(*iter)->GetInfoPos().vPos.y;
	//}
	for (int i = 0 ; i < LAYER_END ; ++i)
	{
		for (list<CObj*>::iterator iter = m_SortLayer[i].begin();
			iter != m_SortLayer[i].end(); ++iter)
		{
			if((*iter) != NULL)
				(*iter)->Render();
		}
		m_SortLayer[i].clear();
	}
}

void CSortMgr::Release()
{
	for ( int i = 0 ; i < LAYER_END; ++i )
	{
		m_SortLayer[i].clear();
	}
}
