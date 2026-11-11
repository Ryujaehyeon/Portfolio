#pragma once

#include "SmokeLog.h"

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
