#pragma once

#include "Prototype.h"
#include "Obj.h"

class CObjProto : public CPrototype
{
protected:
	map<wstring, CObj*> m_MapProto;
public:
	CObj* GetProto(wstring pObjKey);
public:
	virtual HRESULT InitProtoInstance()PURE;
	virtual void Release();
public:
	CObjProto(void);
	virtual ~CObjProto(void);
};

