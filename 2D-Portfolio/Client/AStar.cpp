#include "StdAfx.h"
#include "AStar.h"
#include "Include.h"

CAStar::CAStar(void)
{
}


CAStar::~CAStar(void)
{
	Release();
}

void CAStar::AStarBegin( const int& iStart, const int& iGoal )
{
	// 스타트 == 내위치 , 골 == 목표지점
	if (iStart == iGoal)
		return;
	const vector<TILE*>* pVecTile = GET_SINGLE(CObjMgr)->GetTileInfo();
	if (pVecTile == NULL)
		return;

	if ((*pVecTile)[iGoal]->byOption & TILE_OPTION_DONT_MOVE)
		return;

	Release();

	m_iStartIndex = iStart;
	m_iGoalIndex = iGoal;
	MakeRoute();
}

void CAStar::MakeRoute()
{
	NODE* pParent = new NODE;

	pParent->iIndex = m_iStartIndex;
	pParent->pParent = NULL;
	pParent->fCost = 0.f;
	m_CloseList.push_back(pParent);

	const vector<TILE*>* pVecTile = GET_SINGLE(CObjMgr)->GetTileInfo();
	
	if (pVecTile == NULL)
		return;

	NODE* pNode = NULL;
	int iIndex = 0;

	while(true)
	{
		// 위
		iIndex = pParent->iIndex - TILECNTX * 2;

		// 조건들
		// 존재하는 타일인가?
		// 갈 수 있는 타일인가?
		// 이미 조사한 타일인가?
		// 오픈리스트에 이미 있는가?

		// 1. 존재하는 타일인가?
		if (iIndex > -1)
		{
			// 2. 갈(이동) 수 있는 타일인가?
			if ( ((*pVecTile)[iIndex]->byOption ^ 
				TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
			{
				// 3. 이미 조사한 타일인가?
				if (CheckList(iIndex))
				{
					pNode = MakeNode(iIndex, pParent, pVecTile);
					m_OpenList.push_back(pNode);
				}
			}
		}
		// 오른쪽 위
		iIndex = pParent->iIndex - 
			( TILECNTX - (pParent->iIndex / TILECNTX) % 2 );

		// 1. 존재하는 타일인가?
		if (iIndex > -1)
		{
			// 예외 처리
			if (pParent->iIndex % (TILECNTX * 2) 
				!= TILECNTX * 2 - 1)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// 오른쪽
		iIndex = pParent->iIndex + 1 ;

		// 1. 존재하는 타일인가?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % TILECNTX != TILECNTX - 1)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}

		// 오른쪽 아래
		iIndex =  pParent->iIndex + 
			( TILECNTX + (pParent->iIndex / TILECNTX) % 2 ); 

		// 1. 존재하는 타일인가?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % TILECNTX != TILECNTX - 1)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex, pParent, pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// 아래
		iIndex = pParent->iIndex + TILECNTX * 2;

		// 1. 존재하는 타일인가?
		if (iIndex < TILECNTX * TILECNTY)
		{
			// 2. 갈(이동) 수 있는 타일인가?
			if ( ((*pVecTile)[iIndex]->byOption ^ 
				TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
			{
				// 3. 이미 조사한 타일인가?
				if (CheckList(iIndex))
				{
					pNode = MakeNode(iIndex,pParent,pVecTile);
					m_OpenList.push_back(pNode);
				}
			}
		}

		// 왼 아래
		iIndex = pParent->iIndex + 
			(TILECNTX - 1 + (pParent->iIndex / TILECNTX) % 2);

		// 1. 존재하는 타일인가?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % (TILECNTX * 2) != 0)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// 왼
		iIndex = pParent->iIndex - 1 ;

		// 1. 존재하는 타일인가?
		if (iIndex > -1)
		{
			if (pParent->iIndex % TILECNTX != 0)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex, pParent, pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// 왼 위
		iIndex = pParent->iIndex - 
			( TILECNTX + 1 - (pParent->iIndex / TILECNTX) % 2 );

		// 1. 존재하는 타일인가?
		if (iIndex > -1)
		{
			// 예외 처리
			if (pParent->iIndex % (TILECNTX * 2) != 0)
			{
				// 2. 갈(이동) 수 있는 타일인가?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. 이미 조사한 타일인가?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex, pParent, pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		m_OpenList.sort(Compare);

		list<NODE*>::iterator iter = m_OpenList.begin();

		pParent = *iter;
		m_CloseList.push_back(*iter);
		iter = m_OpenList.erase(iter);

		if(pParent->iIndex == m_iGoalIndex)
		{
			while(true)
			{
				m_BestList.push_front(pParent->iIndex);
				pParent =  pParent->pParent;
				if (pParent->iIndex == m_iStartIndex)
					break;
			}
			break;
		}
	}
}

bool CAStar::CheckList( const int& iIndex )
{
	// 인자로 받은 iIndex가 Open, Close에 존재하는지 여부를 체크함
	for (list<NODE*>::iterator iter = m_OpenList.begin();
		iter != m_OpenList.end(); ++iter)
	{
		if ((*iter)->iIndex == iIndex)
			return false;

	}

	for (list<NODE*>::iterator iter = m_CloseList.begin();
		iter != m_CloseList.end(); ++iter)
	{
		if ((*iter)->iIndex == iIndex)
			return false;
	}
	return true;
}

NODE* CAStar::MakeNode( int iIndex, NODE* pParent, const vector<TILE*>* pVecTile )
{
	NODE* pNode = new NODE;
	pNode->iIndex = iIndex;
	pNode->pParent = pParent;

	// 부모노드까지의 거리
	D3DXVECTOR3 vDir = (*pVecTile)[iIndex]->vPos - 
		(*pVecTile)[pParent->iIndex]->vPos;

	float fPCost = D3DXVec3Length(&vDir);
	
	// 도착노드까지의 거리
	vDir = (*pVecTile)[iIndex]->vPos - 
		(*pVecTile)[m_iGoalIndex]->vPos;

	float fGCost = D3DXVec3Length(&vDir);
	pNode->fCost = fPCost + fGCost;
	
	return pNode;
}

void CAStar::Release()
{
	for (list<NODE*>::iterator iter = m_OpenList.begin();
		iter != m_OpenList.end(); ++iter)
	{
		SAFE_DELETE<NODE>(&(*iter));

	}
	for (list<NODE*>::iterator iter = m_CloseList.begin();
		iter != m_CloseList.end(); ++iter)
	{
		SAFE_DELETE<NODE>(&(*iter));
	}

	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();
}
