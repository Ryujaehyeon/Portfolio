#pragma once
#include "objproto.h"
class CMenuObjProto :
	public CObjProto
{
public:
	virtual HRESULT InitProtoInstance();
public:
	CMenuObjProto(void);
	virtual ~CMenuObjProto(void);
};

