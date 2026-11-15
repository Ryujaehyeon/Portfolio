#pragma once

// 결정적 실행 모드(intent/render-capture). Client 를 -shot <폴더> 로 실행했을 때만 켜진다.
// 켜지면 난수 시드와 프레임 시간을 고정하고, 스모크 일정도 프레임 수로 센다 — 같은 명령이면 매번 같은 프레임에 같은 장면이 된다.
// 꺼져 있으면 GameTickMs() 가 GetTickCount64() 와 같다 — 일반 실행과 기존 스모크 동작은 그대로다.
// 상태는 FixedStepState() 의 static 하나에 둔다(헤더 전용, SmokeLog.h 와 같은 방식).

#include <string>

// 결정적 모드의 한 프레임 길이(초)
constexpr float FIXED_STEP_DELTA = 1.f / 60.f;

struct FIXEDSTEPSTATE
{
	bool bOn;
	std::wstring strShotDir;
	ULONGLONG ullFrame;
	int iScene;
	int iSceneFrame;

	FIXEDSTEPSTATE()
		: bOn(false)
		, ullFrame(0)
		, iScene(-1)
		, iSceneFrame(0)
	{
	}
};

inline FIXEDSTEPSTATE& FixedStepState()
{
	static FIXEDSTEPSTATE tState;
	return tState;
}

inline bool IsFixedStepMode()
{
	return FixedStepState().bOn;
}

// 폴더를 만든다 — 중간 폴더까지(이미 있으면 그대로)
inline bool CreateDirectoryTree(const std::wstring& _strDir)
{
	for (size_t i = 0; i <= _strDir.size(); ++i)
	{
		if (i == _strDir.size() || _strDir[i] == L'/' || _strDir[i] == L'\\')
		{
			const std::wstring strPart = _strDir.substr(0, i);
			if (strPart.empty() || strPart == L"." || strPart == L"..")
			{
				continue;
			}
			if (!CreateDirectory(strPart.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
			{
				return false;
			}
		}
	}
	return true;
}

// 결정적 모드를 켜고 캡처 폴더를 만든다. 폴더를 못 만들면 false(모드는 켜진다 — 캡처만 실패한다).
inline bool FixedStepBegin(const TCHAR* const _pShotDir)
{
	FIXEDSTEPSTATE& rState = FixedStepState();
	rState.bOn = true;
	rState.strShotDir = _pShotDir;
	return CreateDirectoryTree(rState.strShotDir);
}

// 프레임마다 장면 갱신 직후 1번 — 장면이 바뀌면 장면 프레임을 0 부터 다시 센다
inline void FixedStepAdvance(const int _iScene)
{
	FIXEDSTEPSTATE& rState = FixedStepState();
	if (!rState.bOn)
	{
		return;
	}
	++rState.ullFrame;
	if (_iScene != rState.iScene)
	{
		rState.iScene = _iScene;
		rState.iSceneFrame = 0;
	}
	else
	{
		++rState.iSceneFrame;
	}
}

// 게임 시계(ms). 평소에는 실제 시간, 결정적 모드에서는 「지나간 프레임 수 × 프레임 길이」.
// 스모크 일정처럼 장면 진행에 영향을 주는 시간 판정은 이것을 쓴다.
inline ULONGLONG GameTickMs()
{
	const FIXEDSTEPSTATE& rState = FixedStepState();
	if (!rState.bOn)
	{
		return GetTickCount64();
	}
	return rState.ullFrame * 1000ULL / 60ULL;
}
