#pragma once

#include "SceneObj.h"

class CLoadingScene : public CSceneObj
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CLoadingScene(void);
	virtual ~CLoadingScene(void);
};

