#pragma once
#include "Obj.h"
class CSkillTree :public CObj
{
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
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

