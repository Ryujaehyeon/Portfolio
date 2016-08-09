#pragma once

#include "Defines.h"

class CPrototype;
class CSceneObj
{
	static D3DXVECTOR3 g_tScroll;
protected:
	CPrototype* m_pPrototype;
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
public:
	CSceneObj(void);
	virtual ~CSceneObj(void);
};

