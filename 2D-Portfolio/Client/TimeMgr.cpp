#include "StdAfx.h"
#include "TimeMgr.h"


CTimeMgr::CTimeMgr(void)
	: m_FrameTime()
	, m_FixTime()
	, m_LastTime()
	, m_CpuTime()
	, m_fTime(0.0f)
{}

CTimeMgr::~CTimeMgr(void)
{}

void CTimeMgr::InitTimeMgr()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTime);
}

void CTimeMgr::SetTime()
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart
			> m_CpuTime.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTime);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart)
				/ m_CpuTime.QuadPart;
	m_FixTime = m_FrameTime;
}

float CTimeMgr::DeltaTime() const
{
	// 결정적 모드에서는 실제 경과 시간 대신 고정 길이(intent/render-capture)
	if (IsFixedStepMode())
	{
		return FIXED_STEP_DELTA;
	}
	return m_fTime;
}
