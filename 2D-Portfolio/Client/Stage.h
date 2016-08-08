#pragma once

#include "SceneObj.h"

class CStage : public CSceneObj
{
private:
public:
	
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CStage(void);
	virtual ~CStage(void);
};

