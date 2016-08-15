#pragma once
#include "Defines.h"
//#include "SingletonTemplate.h"
class CKeyMgr //: public SingletonTemplate<CKeyMgr>
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

