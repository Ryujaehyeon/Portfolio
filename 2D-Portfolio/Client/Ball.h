#pragma once

#include "LogoDynamic.h"

class CBall : public CLogoDynamic
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CBall(void);
	CBall(const OBJINFO& Info, const OBJ_TYPE _OjbType);
	virtual ~CBall(void);
};

