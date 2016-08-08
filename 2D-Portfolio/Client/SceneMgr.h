#pragma once

#include "Defines.h"

class CSceneObj;
class CSceneMgr
{
	SINGLETON(CSceneMgr);
private:
	CSceneObj* m_pSceneObj;
public:
	HRESULT InitScene(SCENEID ID);
	SCENEID Progress();
	void Render();
	void Release();
private:
	CSceneMgr(void);
public:
	~CSceneMgr(void);
};

