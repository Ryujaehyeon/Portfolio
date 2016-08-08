#pragma once

#include "Defines.h"

class CMainGame
{
public:
	HRESULT Initialize();
	SCENEID Progress();
	void Render();
	void Release();
public:
	CMainGame(void);
	~CMainGame(void);
};

