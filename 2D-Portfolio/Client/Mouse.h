#pragma once
class CMouse
{
public:
	static POINT GetPos()
	{
		POINT tPoint;
		GetCursorPos(&tPoint);
		ScreenToClient(g_hWnd, &tPoint);
		return tPoint;
	}
	static POINTFLOAT GetPosf()
	{
		POINT tPoint = GetPos();
		POINTFLOAT fPoint = {(float)tPoint.x, (float)tPoint.y};
		return fPoint;
	}
public:
	CMouse(void);
	~CMouse(void);
};

