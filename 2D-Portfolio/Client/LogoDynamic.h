#pragma once

#include "Obj.h"

class CLogoDynamic : public CObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CLogoDynamic(void);
	CLogoDynamic(const OBJINFO& Info, const OBJ_TYPE _OjbType);
	virtual ~CLogoDynamic(void);
};

