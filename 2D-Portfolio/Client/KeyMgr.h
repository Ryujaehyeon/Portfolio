#pragma once
#include "Defines.h"
class CKeyMgr
{
	SINGLETON(CKeyMgr);
private:
	DWORD m_dwKey;
public:
	const DWORD GetKey() const;
public:
	void CheckKey();
	DWORD PushKey(DWORD dwKey);
private:
		CKeyMgr(void);
public:
	~CKeyMgr(void);
};

