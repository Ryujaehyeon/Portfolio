#pragma once
#include "obj.h"
class CStageStatic :
	public CObj
{
public:
	void InitTileFromFile();
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CStageStatic(void);
	CStageStatic(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CStageStatic(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CStageStatic(void);
};

