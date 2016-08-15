#pragma once

#include "SceneObj.h"

class CStage : public CSceneObj
{
	
public:
	static D3DXVECTOR3 g_tScroll;
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
public:
	CStage(void);
	virtual ~CStage(void);
};

