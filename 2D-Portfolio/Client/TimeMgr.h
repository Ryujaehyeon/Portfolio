#pragma once

#include "Defines.h"

class CTimeMgr
{
	SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTime;
	float         m_fTime;
public:
	void InitTimeMgr();
	void SetTime();
public:
	float DeltaTime() const ;
private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};

