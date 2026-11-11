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
	for (int i = 0 ; i < LAYER_END ; ++i)
	{
		for (list<CObj*>::iterator iter = m_SortLayer[i].begin();
			iter != m_SortLayer[i].end(); ++iter)
		{ 
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
