#pragma once
#include "obj.h"
class CMenuStatic :
	public CObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CMenuStatic(void);
	CMenuStatic(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CMenuStatic(void);
};

