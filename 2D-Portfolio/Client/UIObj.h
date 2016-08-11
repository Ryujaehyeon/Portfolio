#pragma once
#include "stagestatic.h"
class CUIObj :
	public CStageStatic
{
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	CUIObj(void);
	CUIObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~CUIObj(void);
};

