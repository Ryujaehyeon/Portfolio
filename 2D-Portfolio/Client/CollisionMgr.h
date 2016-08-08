#pragma once
class CObj;
class CCollisionMgr
{
public:
	static bool CircleCol(
		CObj* pTemp, 
		CObj* pDest);
public:
	CCollisionMgr(void);
	virtual ~CCollisionMgr(void);
};

