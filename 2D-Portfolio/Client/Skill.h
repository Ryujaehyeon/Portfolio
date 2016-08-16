#pragma once

//#include "Defines.h"
#include "Obj.h"

class CSkill : public CObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CSkill(void);
	CSkill(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CSkill(void);
};

