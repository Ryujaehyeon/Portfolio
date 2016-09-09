#pragma once
#include "lodingview.h"
class CLoding :
	public CLodingView
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CLoding(void);
	CLoding(const OBJINFO& Info, const OBJ_TYPE _OjbType);
	virtual ~CLoding(void);
};

