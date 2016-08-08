#pragma once
#include "obj.h"
class CStageDynamic :
	public CObj
{
public:

public:
	void ASterBegin(const D3DXVECTOR3& vDestPos, const D3DXVECTOR3& vSourPos);
	void AStarMove();
	void SetDirect(const D3DXVECTOR3& Dest);
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CStageDynamic(void);
	CStageDynamic(const OBJINFO& Info,  TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CStageDynamic(void);
};

