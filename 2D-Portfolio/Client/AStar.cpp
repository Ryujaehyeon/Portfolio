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
	// ��ŸƮ == ����ġ , �� == ��ǥ����
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
		// ��
		iIndex = pParent->iIndex - TILECNTX * 2;

		// ���ǵ�
		// �����ϴ� Ÿ���ΰ�?
		// �� �� �ִ� Ÿ���ΰ�?
		// �̹� ������ Ÿ���ΰ�?
		// ���¸���Ʈ�� �̹� �ִ°�?

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex > -1)
		{
			// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
			if ( ((*pVecTile)[iIndex]->byOption ^ 
				TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
			{
				// 3. �̹� ������ Ÿ���ΰ�?
				if (CheckList(iIndex))
				{
					pNode = MakeNode(iIndex, pParent, pVecTile);
					m_OpenList.push_back(pNode);
				}
			}
		}
		// ������ ��
		iIndex = pParent->iIndex - 
			( TILECNTX - (pParent->iIndex / TILECNTX) % 2 );

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex > -1)
		{
			// ���� ó��
			if (pParent->iIndex % (TILECNTX * 2) 
				!= TILECNTX * 2 - 1)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// ������
		iIndex = pParent->iIndex + 1 ;

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % TILECNTX != TILECNTX - 1)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}

		// ������ �Ʒ�
		iIndex =  pParent->iIndex + 
			( TILECNTX + (pParent->iIndex / TILECNTX) % 2 ); 

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % TILECNTX != TILECNTX - 1)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex, pParent, pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// �Ʒ�
		iIndex = pParent->iIndex + TILECNTX * 2;

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex < TILECNTX * TILECNTY)
		{
			// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
			if ( ((*pVecTile)[iIndex]->byOption ^ 
				TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
			{
				// 3. �̹� ������ Ÿ���ΰ�?
				if (CheckList(iIndex))
				{
					pNode = MakeNode(iIndex,pParent,pVecTile);
					m_OpenList.push_back(pNode);
				}
			}
		}

		// �� �Ʒ�
		iIndex = pParent->iIndex + 
			(TILECNTX - 1 + (pParent->iIndex / TILECNTX) % 2);

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex < TILECNTX * TILECNTY)
		{
			if (pParent->iIndex % (TILECNTX * 2) != 0)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex,pParent,pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// ��
		iIndex = pParent->iIndex - 1 ;

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex > -1)
		{
			if (pParent->iIndex % TILECNTX != 0)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
					if (CheckList(iIndex))
					{
						pNode = MakeNode(iIndex, pParent, pVecTile);
						m_OpenList.push_back(pNode);
					}
				}
			}
		}
		// �� ��
		iIndex = pParent->iIndex - 
			( TILECNTX + 1 - (pParent->iIndex / TILECNTX) % 2 );

		// 1. �����ϴ� Ÿ���ΰ�?
		if (iIndex > -1)
		{
			// ���� ó��
			if (pParent->iIndex % (TILECNTX * 2) != 0)
			{
				// 2. ��(�̵�) �� �ִ� Ÿ���ΰ�?
				if ( ((*pVecTile)[iIndex]->byOption ^ 
					TILE_OPTION_DONT_MOVE) & TILE_OPTION_DONT_MOVE )
				{
					// 3. �̹� ������ Ÿ���ΰ�?
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
	// ���ڷ� ���� iIndex�� Open, Close�� �����ϴ��� ���θ� üũ��
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

	// �θ�������� �Ÿ�
	D3DXVECTOR3 vDir = (*pVecTile)[iIndex]->vPos - 
		(*pVecTile)[pParent->iIndex]->vPos;

	float fPCost = D3DXVec3Length(&vDir);
	
	// ������������ �Ÿ�
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
