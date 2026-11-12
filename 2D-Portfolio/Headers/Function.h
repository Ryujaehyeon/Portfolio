#pragma once

#include "SmokeLog.h"

// 이름·키·모션 문자열을 내용으로 비교한다. 주소(==)로 비교하면 컴파일러의 문자열 병합(/GF)에 기대게 된다(intent/string-key-compare).
// 하나라도 nullptr 이면 두 포인터가 같을 때만 참.
inline bool IsSameKey(const TCHAR* const _pLeft, const TCHAR* const _pRight)
{
	if (_pLeft == nullptr || _pRight == nullptr)
	{
		return _pLeft == _pRight;
	}
	return _tcscmp(_pLeft, _pRight) == 0;
}

template <typename T>
void SAFE_DELETE(T** pObj)
{
	if(*pObj != nullptr)
	{
		delete *pObj;
		*pObj = nullptr;
	}
}

inline void ErrMsgBox(const HWND _hWnd, const TCHAR* const _pFormat, ...)
{
	TCHAR szMsg[256];
	va_list ap;
	va_start(ap, _pFormat);
	_vsnwprintf_s(szMsg, _TRUNCATE, _pFormat, ap);
	va_end(ap);
	// 스모크 모드에서는 무인 실행이 멈추지 않게 메시지박스 대신 로그에 실패로 남긴다
	if (IsSmokeMode())
	{
		SmokeCheck(false, L"err %s", szMsg);
		return;
	}
	MessageBox(_hWnd, szMsg, L"system err", MB_OK);
}

// acosf 의 입력을 [-1, 1] 로 자른다 — 정규화한 두 벡터의 내적이 부동소수점 오차로 1 을 살짝 넘으면 acosf 가 NaN 을 돌려준다(intent/frame-index-bounds).
inline float ClampedAcosf(const float _fCos)
{
	if (_fCos > 1.f)
	{
		return acosf(1.f);
	}
	if (_fCos < -1.f)
	{
		return acosf(-1.f);
	}
	return acosf(_fCos);
}

// 실제 입력 읽기 — 스모크 모드에서는 사용자의 마우스·키보드를 무시한다.
// 전역 입력(GetAsyncKeyState·GetCursorPos)이라 다른 창에서의 조작도 들어오기 때문이다(intent/smoke-input-isolation).
// 게임 창이 비활성일 때(이 스레드에 활성 창이 없을 때)도 키·버튼을 안 눌린 것으로 본다(intent/input-unify).
inline SHORT GameAsyncKeyState(const int _iKey)
{
	if (IsSmokeMode())
	{
		return 0;
	}
	if (GetActiveWindow() == nullptr)
	{
		return 0;
	}
	return GetAsyncKeyState(_iKey);
}

// 커서의 화면 좌표. 스모크 모드에서는 _hWnd 클라이언트 영역 가운데로 고정한다.
inline void GameCursorPos(const HWND _hWnd, POINT* const _pPoint)
{
	if (IsSmokeMode())
	{
		RECT rcClient = {};
		GetClientRect(_hWnd, &rcClient);
		_pPoint->x = (rcClient.left + rcClient.right) / 2;
		_pPoint->y = (rcClient.top + rcClient.bottom) / 2;
		ClientToScreen(_hWnd, _pPoint);
		return;
	}
	GetCursorPos(_pPoint);
}

// 폴더가 있는가
inline bool IsDirectory(const TCHAR* const _pPath)
{
	const DWORD dwAttr = GetFileAttributes(_pPath);
	return dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

// 리소스 상대 경로(../Resource/…, Map.dat, *.mp3)는 「2D-Portfolio/<앱 폴더>」가 작업 디렉터리라고 가정한다.
// 작업 디렉터리에 ../Resource 가 없으면(탐색기에서 exe 를 더블클릭한 경우 등) exe 폴더부터 위로 올라가며
// Resource 와 <앱 폴더> 를 함께 가진 폴더를 찾아 그 <앱 폴더> 로 옮긴다. 못 찾으면 그대로 둔다(intent/resource-path).
inline void SetResourceWorkDir(const TCHAR* const _pAppFolder)
{
	if (IsDirectory(L"../Resource"))
	{
		return;
	}
	TCHAR szDir[MAX_PATH] = {};
	const DWORD dwLen = GetModuleFileName(nullptr, szDir, MAX_PATH);
	if (dwLen == 0 || dwLen >= MAX_PATH)
	{
		return;
	}
	TCHAR szCandidate[MAX_PATH] = {};
	TCHAR* pSlash = _tcsrchr(szDir, L'\\');
	while (pSlash != nullptr)
	{
		*pSlash = L'\0';	// 한 단계 위 폴더(처음에는 exe 파일 이름을 떼어 exe 폴더)
		if (_stprintf_s(szCandidate, L"%s\\Resource", szDir) > 0 && IsDirectory(szCandidate)
			&& _stprintf_s(szCandidate, L"%s\\%s", szDir, _pAppFolder) > 0 && IsDirectory(szCandidate))
		{
			SetCurrentDirectory(szCandidate);
			return;
		}
		pSlash = _tcsrchr(szDir, L'\\');
	}
}
