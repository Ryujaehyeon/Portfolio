#pragma once

#include "Defines.h"

class CAStar
{
	SINGLETON(CAStar);
private:
	list<NODE*> m_OpenList;		 //������ ���
	list<NODE*> m_CloseList;	 //������ ���
	list<int> m_BestList;		 //������ ���
public:
	int m_iStartIndex;
	int m_iGoalIndex;
public:
	list<int>* GetBestList()
	{
		return &m_BestList;
	}
public:
	static bool Compare(NODE* pDestNode, NODE* pSourNode)
	{
		return pDestNode->fCost < pSourNode->fCost;
	}
public:
	void AStarBegin(const int& iStart, const int& iGoal);
	void MakeRoute();
	bool CheckList(const int& iIndex);
	NODE* MakeNode(int iIndex, NODE* pParent, const vector<TILE*>* pVecTile);
	void Release();
private:
	CAStar(void);
public:
	virtual ~CAStar(void);
};

