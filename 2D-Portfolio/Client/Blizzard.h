#pragma once
#include "skill.h"
class CBlizzard :
	public CSkill
{
protected:
	D3DXVECTOR3 m_vMousePos;
	float m_fTime;
public:
	void FrameStatas();
	void DirectAction( TCHAR* _pObjStatas );
	void StateSkill();
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CBlizzard(void);
	CBlizzard(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CBlizzard(void);
};

