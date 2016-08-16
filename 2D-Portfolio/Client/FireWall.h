#pragma once
#include "skill.h"
class CFireWall :
	public CSkill
{
protected:
	D3DXVECTOR3 m_vMousePos;
public:
	void FrameStatas();
	void DirectAction( TCHAR* _pObjStatas );
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CFireWall(void);
	CFireWall(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CFireWall(void);
};

