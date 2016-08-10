#pragma once
#include <Windows.h>
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

#ifdef DEBUG_EXPORT
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#ifdef _UNICODE
#define DebugMsg DebugMsgw
#else
#define DebugMsg DebugMsga
#endif

EXTERN EXPORT void DebugMsgw(wchar_t* str);
EXTERN EXPORT void DebugMsga(char* str);
EXTERN EXPORT HRESULT CreateConsole();
EXTERN EXPORT void DestroyConsole();