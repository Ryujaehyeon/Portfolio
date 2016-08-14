#include "StdAfx.h"
#include "FullModeMgr.h"

#ifdef _WINDOWS_

CFullModeMgr* CFullModeMgr::_Instance = 0;

CFullModeMgr::CFullModeMgr(void)
{}

CFullModeMgr::~CFullModeMgr(void)
{ Release(); }

void CFullModeMgr::SetMode(HWND hWnd, const int& iWidth, const int& iHeight, const int& iPel)
{
	_hwnd = hWnd;
	_pos.x = 10;
	_pos.y = 10;
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &_saved);
	memset(&_user, 0, sizeof(_user));
	_user.dmSize = sizeof(_user);
	_user.dmPelsWidth = iWidth;
	_user.dmPelsHeight = iHeight;
	_user.dmBitsPerPel = iPel;
	_user.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	ModeChange(WINDOW_WINMODE);
}

void CFullModeMgr::_modechange(const int& _x, const int& _y, const int& _width, const int& _height, const DWORD& _style)
{
	SetWindowLong(_hwnd, GWL_STYLE, _style);
	MoveWindow(_hwnd, _x, _y, _width, _height, true);
	SetFocus(_hwnd);
	ShowWindow(_hwnd, 1);
}

void CFullModeMgr::_posset(void)
{
	RECT rc;
	GetWindowRect(_hwnd, &rc);
	_pos.x = rc.left;
	_pos.y = rc.top;
}

bool CFullModeMgr::ModeChange(const int& iWinMode)
{
	if(WINDOW_FULLMODE == iWinMode)
	{
		if(ChangeDisplaySettings(&_user, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			_posset();
			_modechange(0, 0, _user.dmPelsWidth, _user.dmPelsHeight, WS_POPUP);
			_mode = iWinMode;
			return true;
		}
	}
	else if(WINDOW_WINMODE == iWinMode)
	{
		if(ChangeDisplaySettings(&_saved, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
		{
			RECT rc = {0, 0, _user.dmPelsWidth, _user.dmPelsHeight};
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
			_modechange(_pos.x, _pos.y, rc.right - rc.left, rc.bottom - rc.top, WS_OVERLAPPEDWINDOW);
			_mode = iWinMode;
			return true;
		}
	}
	return false;
}

void CFullModeMgr::Release()
{ ChangeDisplaySettings(&_saved, CDS_RESET); }

#endif _WINDOWS_