#pragma once
#include "sceneobj.h"
class CMenuD2 :
	public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CMenuD2(void);
	virtual ~CMenuD2(void);
};

