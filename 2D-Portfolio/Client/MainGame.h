#pragma once

#include "Defines.h"

class CMainGame
{
public:
	HDC hdc;
	static HANDLE hEvent;
public:
	HRESULT Initialize();
	SCENEID Progress();
	void Render();
	void Release();
public:
	CMainGame(void);
	~CMainGame(void);
};

