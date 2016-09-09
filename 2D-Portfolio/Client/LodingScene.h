#pragma once

#include "SceneObj.h"

class CLodingScene : public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CLodingScene(void);
	virtual ~CLodingScene(void);
};

