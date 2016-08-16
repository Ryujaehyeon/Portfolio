#pragma once
#include "skill.h"
class CBoneSpear :
	public CSkill
{
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* CBoneSpear::Clone();
public:
	CBoneSpear(void);
	virtual ~CBoneSpear(void);
};

