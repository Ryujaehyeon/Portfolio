#pragma once
#include "obj.h"
class CLodingView :
	public CObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CLodingView(void);
	CLodingView(const OBJINFO& Info, const OBJ_TYPE _OjbType);
	virtual ~CLodingView(void);
};

