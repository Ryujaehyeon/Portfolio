#pragma once

#include "LogoStatic.h"

class CLogoBackGround : public CLogoStatic
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
	CLogoBackGround(void);
	CLogoBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CLogoBackGround(void);
};

