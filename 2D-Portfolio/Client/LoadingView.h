#pragma once
#include "obj.h"
class LoadingView :
	public CObj
{
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	LoadingView(void);
	LoadingView(const OBJINFO& Info,  TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~LoadingView(void);
};

