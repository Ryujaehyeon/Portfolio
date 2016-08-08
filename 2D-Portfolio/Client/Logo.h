#pragma once

#include "SceneObj.h"

class CLogo : public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CLogo(void);
	virtual ~CLogo(void);
};

