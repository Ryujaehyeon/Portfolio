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
