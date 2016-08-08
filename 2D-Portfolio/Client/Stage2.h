#pragma once
#include "sceneobj.h"
class CStage2 :
	public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CStage2(void);
	virtual ~CStage2(void);
};

