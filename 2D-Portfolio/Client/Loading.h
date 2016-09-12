#pragma once
#include "loadingview.h"
class Loading :
	public LoadingView
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	Loading(void);
	Loading( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~Loading(void);
};

