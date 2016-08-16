#pragma once
class CObj;
class CCollisionMgr
{
	static bool CircleCol(
		CObj* pTemp, 
		CObj* pDest);

public:
	static void ColCircle(
		list<CObj*> *pBulletList, 
		list<CObj*> *pMonsterList);
public:
	CCollisionMgr(void);
	virtual ~CCollisionMgr(void);
};

