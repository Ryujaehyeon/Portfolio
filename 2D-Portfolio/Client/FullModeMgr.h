#pragma once

#ifdef _WINDOWS_

#define WINDOW_FULLMODE 0x01
#define WINDOW_WINMODE  0x02

class CFullModeMgr
{
private:
	int _mode;
	HWND _hwnd;
	DEVMODE _saved;
	DEVMODE _user;
	POINT _pos;
public:
	const int& GetMode(void) const {return _mode;}
public:
	void SetMode(HWND hWnd, const int& iWidth, const int& iHeight, const int& iPel = 32);
	bool ModeChange(const int& iWinMode);
public:
	void Release();
private:
	void _modechange(const int& _x, const int& _y, const int& _width, const int& _height, const DWORD& _style);
	void _posset(void);
#pragma region SINGLETON
public:
	static CFullModeMgr* _getInstace()
	{
		if(_Instance == 0)
			_Instance = new CFullModeMgr;
		return _Instance;
	}
	void Destroy()
	{
		if(_Instance)
		{
			delete _Instance;
			_Instance = 0;
		}
	}
private:
	CFullModeMgr(void);
public:
	~CFullModeMgr(void);
private:
	static CFullModeMgr* _Instance;
#pragma endregion SINGLETON
};

class ModeSetter
{ public: CFullModeMgr* operator->(){return CFullModeMgr::_getInstace();} };

#endif

