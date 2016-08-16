#pragma once
#include "menustatic.h"
class CMenuBackGround :
	public CMenuStatic
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CMenuBackGround(void);
	CMenuBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CMenuBackGround(void);
};

