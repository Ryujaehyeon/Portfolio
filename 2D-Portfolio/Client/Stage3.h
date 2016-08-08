#pragma once
#include "sceneobj.h"
class CStage3 :
	public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CStage3(void);
	virtual ~CStage3(void);
};

