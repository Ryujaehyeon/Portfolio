#pragma once
#include "Obj.h"
class CSkillTree :public CObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CSkillTree(void);
	virtual ~CSkillTree(void);
};

