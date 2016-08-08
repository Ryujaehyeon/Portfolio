#pragma once
#include "obj.h"
class CMenuDynamic :
	public CObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CMenuDynamic(void);
	CMenuDynamic(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CMenuDynamic(void);
};

