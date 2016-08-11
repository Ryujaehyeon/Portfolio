#ifndef __MATHDEF_HPP__
#define __MATHDEF_HPP__

#ifndef _INC_MATH
#include <math.h>
#endif

#define MATH_PI 3.1415926535f
#define DegreeToRadian(degree) ((float)degree * MATH_PI / 180.0f)
#define RadianToDegree(radian) ((float)radian * 180.0f / MATH_PI)

#ifdef __cplusplus
extern "C" {
#endif

__inline float __stdcall __getradian(float x1, float y1, float z1, float x2, float y2, float z2)
{
	double dis1 = sqrtf(x1 * x1 + y1 * y1 + z1 * z1);
	double dis2 = sqrtf(x2 * x2 + y2 * y2 + z2 * z2);
	x1 = float(x1 / dis1);
	x2 = float(x2 / dis2);
	y1 = float(y1 / dis1);
	y2 = float(y2 / dis2);
	z1 = float(z1 / dis1);
	z2 = float(z2 / dis2);
	double inner = x1 * x2 + y1 * y2 + z1 * z2;
	return acosf(float(inner));
}

__inline float __stdcall __getdegree(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return RadianToDegree(__getradian(x1, y1, z1, x2, y2, z2));
}


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C++" {
#endif

#ifdef _WINDOWS_

__inline BOOL __stdcall IntersectCircle(const RECT& Collider1, const RECT& Collider2)
{
	LONG X1 = (Collider1.right  - Collider1.left) >> 1;
	LONG Y1 = (Collider1.bottom - Collider1.top)  >> 1;
	LONG X2 = (Collider2.right  - Collider2.left) >> 1;
	LONG Y2 = (Collider2.bottom - Collider2.top)  >> 1;
	float Distance1 = sqrtf(X1 * X1 + Y1 * Y1);
	float Distance2 = sqrtf(X2 * X2 + Y2 * Y2);
	X1 = ((Collider1.right - X1) - (Collider2.right - X2));
	Y1 = ((Collider1.bottom - Y1) - (Collider2.bottom - Y2));
	float Distance3 = sqrtf(X1 * X1 + Y1 * Y1);
	if(Distance1 + Distance2 <= Distance3)
		return TRUE;
	return FALSE;
}

__inline BOOL __stdcall IntersectCircle(const RECT& Collider1, const POINT& Position1, const RECT& Collider2, const POINT& Position2)
{
	RECT rc1 = {
		Collider1.left + Position1.x, 
		Collider1.top + Position1.y, 
		Collider1.right + Position1.x, 
		Collider1.bottom + Position1.y};
	RECT rc2 = {
		Collider2.left + Position2.x, 
		Collider2.top + Position2.y, 
		Collider2.right + Position2.x, 
		Collider2.bottom + Position2.y};
	return IntersectCircle(rc1, rc2);
}

__inline BOOL __stdcall IntersectEllipse(const RECT& Collider, const POINT& IntersectPosition)
{
	POINT uv = {(Collider.right - Collider.left) >> 1, (Collider.bottom - Collider.top) >> 1};
	POINT pos = {Collider.right - (uv.x), Collider.bottom - (uv.y)};
	if(PtInRect(&Collider, IntersectPosition))
	{
		return float((IntersectPosition.x - pos.x) * (IntersectPosition.x - pos.x)) / float(uv.x * uv.x) + 
			   float((IntersectPosition.y - pos.y) * (IntersectPosition.x - pos.y)) / float(uv.y * uv.y) <= 1.0f;
	}
	else
		return FALSE;
}

__inline BOOL __stdcall IntersectEllipse(const RECT& Collider, const POINT& Position, const POINT& IntersectPosition)
{
	RECT rc1 = {Collider.left + Position.x, Collider.top + Position.y, Collider.right + Position.x, Collider.bottom + Position.y};
	return IntersectEllipse(rc1, IntersectPosition);
}

__inline BOOL _stdcall IntersectRect(const RECT& Collider1, const RECT& Collider2, RECT* lpRect = 0)
{
	if(Collider1.left < Collider2.right && Collider1.top < Collider2.bottom && Collider1.right > Collider2.left && Collider1.bottom > Collider2.top)
	{
		if(lpRect != 0)
		{
			(*lpRect).left   = (Collider1.left   < Collider2.left)  ?Collider2.left  :Collider1.left;
			(*lpRect).top    = (Collider1.top    < Collider2.top)   ?Collider2.top   :Collider1.top;
			(*lpRect).right  = (Collider1.right  < Collider2.right) ?Collider1.right :Collider2.right;
			(*lpRect).bottom = (Collider1.bottom < Collider2.bottom)?Collider1.bottom:Collider2.bottom;
		}
		return TRUE;
	}
	else
	{
		if(lpRect != 0)
		{
			(*lpRect).left   = 0;
			(*lpRect).top    = 0;
			(*lpRect).right  = 0;
			(*lpRect).bottom = 0;
		}
		return FALSE;
	}
}

__inline BOOL __stdcall IntersectRect(const RECT& Collider1, const POINT& Position1
	, const RECT& Collider2, const POINT& Position2, RECT* lpRect = 0)
{
	RECT rc1 = {
		Collider1.left + Position1.x, 
		Collider1.top + Position1.y, 
		Collider1.right + Position1.x, 
		Collider1.bottom + Position1.y};
	RECT rc2 = {
		Collider2.left + Position2.x, 
		Collider2.top + Position2.y, 
		Collider2.right + Position2.x, 
		Collider2.bottom + Position2.y};
	return IntersectRect(rc1, rc2, lpRect);
}
#endif

_inline float __stdcall GetRadian(float x1, float y1, float z1, float x2, float y2, float z2)
{ return __getradian(x1, y1, z1, x2, y2, z2); }

_inline float __stdcall GetRadian(float x1, float y1, float x2, float y2)
{ return __getradian(x1, y1, 0, x2, y2, 0); }

_inline float __stdcall GetDegree(float x1, float y1, float z1, float x2, float y2, float z2)
{ return __getdegree(x1, y1, z1, x2, y2, z2); }

_inline float __stdcall GetDegree(float x1, float y1, float x2, float y2)
{ return __getdegree(x1, y1, 0, x2, y2, 0); }

#ifdef __cplusplus
}
#endif

#endif