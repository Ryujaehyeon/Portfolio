#pragma once
#include "stagestatic.h"
class CBuildObj :
	public CStageStatic
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	CBuildObj(void);
	CBuildObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CBuildObj(void);
};

