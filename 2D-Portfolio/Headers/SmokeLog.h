#pragma once

// 자동 검증용 스모크 로그 (intent/smoke-log). -smoke 로 실행했을 때만 켜진다.
// 꺼져 있으면 모든 함수가 즉시 반환한다 — 일반 실행과 Release 동작은 그대로다.
// 상태는 SmokeState() 의 static 하나에 둔다(헤더 전용: Client·MFC_000 공용).

#include <string>
#include <vector>

struct SMOKESTATE
{
	HANDLE hFile;
	ULONGLONG ullStart;
	int iFailCount;
	std::vector<std::wstring> vecPassed;
};

inline SMOKESTATE& SmokeState()
{
	static SMOKESTATE tState = { INVALID_HANDLE_VALUE, 0, 0, std::vector<std::wstring>() };
	return tState;
}

inline bool IsSmokeMode()
{
	return SmokeState().hFile != INVALID_HANDLE_VALUE;
}

// "[  경과 ms] TAG 메시지" 한 줄을 UTF-8 로 바로 쓴다(버퍼 없음 — 크래시해도 마지막 줄까지 남는다).
inline void SmokeWriteLine(const TCHAR* const _pTag, const TCHAR* const _pMsg)
{
	SMOKESTATE& rState = SmokeState();
	TCHAR szLine[512];
	_snwprintf_s(szLine, _TRUNCATE, L"[%6llu ms] %s %s\n",
		GetTickCount64() - rState.ullStart, _pTag, _pMsg);

	char szUtf8[1536];
	const int iLen = WideCharToMultiByte(CP_UTF8, 0, szLine, -1, szUtf8, sizeof(szUtf8), nullptr, nullptr);
	if (iLen > 1)
	{
		DWORD dwWritten = 0;
		WriteFile(rState.hFile, szUtf8, static_cast<DWORD>(iLen - 1), &dwWritten, nullptr);
	}
}

// 스모크 모드를 켜고 로그 파일을 새로 만든다. 실패하면 false(스모크 꺼짐).
inline bool SmokeBegin(const TCHAR* const _pLogPath)
{
	SMOKESTATE& rState = SmokeState();
	rState.hFile = CreateFile(_pLogPath, GENERIC_WRITE, FILE_SHARE_READ, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (rState.hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	rState.ullStart = GetTickCount64();
	rState.iFailCount = 0;
	rState.vecPassed.clear();
	SmokeWriteLine(L"INFO", L"smoke begin");
	return true;
}

inline void SmokeLog(const TCHAR* const _pFormat, ...)
{
	if (!IsSmokeMode())
	{
		return;
	}
	TCHAR szMsg[384];
	va_list ap;
	va_start(ap, _pFormat);
	_vsnwprintf_s(szMsg, _TRUNCATE, _pFormat, ap);
	va_end(ap);
	SmokeWriteLine(L"INFO", szMsg);
}

// 확인 결과 한 줄. PASS 는 메시지를 기억하고(SmokeEnd 의 필수 항목 대조), FAIL 은 수를 센다.
inline void SmokeCheck(const bool _bPass, const TCHAR* const _pFormat, ...)
{
	if (!IsSmokeMode())
	{
		return;
	}
	TCHAR szMsg[384];
	va_list ap;
	va_start(ap, _pFormat);
	_vsnwprintf_s(szMsg, _TRUNCATE, _pFormat, ap);
	va_end(ap);

	SMOKESTATE& rState = SmokeState();
	if (_bPass)
	{
		rState.vecPassed.push_back(szMsg);
	}
	else
	{
		++rState.iFailCount;
	}
	SmokeWriteLine(_bPass ? L"PASS" : L"FAIL", szMsg);
}

// 필수 항목(PASS 메시지의 접두어)이 하나라도 없으면 FAIL 로 남기고, RESULT 줄을 쓴 뒤 파일을 닫는다.
// 반환: 종료 코드 — 0 = 전부 통과. 스모크가 아니면 0.
inline int SmokeEnd(const TCHAR* const* const _ppRequired, const int _iCount)
{
	if (!IsSmokeMode())
	{
		return 0;
	}
	SMOKESTATE& rState = SmokeState();
	for (int i = 0; i < _iCount; ++i)
	{
		const size_t iPrefixLen = wcslen(_ppRequired[i]);
		bool bFound = false;
		for (const std::wstring& rPassed : rState.vecPassed)
		{
			if (rPassed.compare(0, iPrefixLen, _ppRequired[i]) == 0)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			SmokeCheck(false, L"missing %s", _ppRequired[i]);
		}
	}

	TCHAR szResult[64];
	_snwprintf_s(szResult, _TRUNCATE, L"%s (fail=%d)",
		rState.iFailCount == 0 ? L"PASS" : L"FAIL", rState.iFailCount);
	SmokeWriteLine(L"RESULT", szResult);

	const int iExitCode = (rState.iFailCount == 0) ? 0 : 1;
	CloseHandle(rState.hFile);
	rState.hFile = INVALID_HANDLE_VALUE;
	return iExitCode;
}
