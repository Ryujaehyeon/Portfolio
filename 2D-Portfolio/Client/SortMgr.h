#pragma once
#include "Defines.h"
class CObj;
class CSortMgr
{
	SINGLETON(CSortMgr);
private:
	list<CObj*> m_SortLayer[LAYER_END];
public:
	void SetLayer(CObj* pObj);
	void Render();
	void Release();
private:
	CSortMgr(void);
public:
	virtual ~CSortMgr(void);
};

