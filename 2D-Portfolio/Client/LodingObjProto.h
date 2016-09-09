#pragma once
#include "objproto.h"
class CLodingObjProto :
	public CObjProto
{
public:
	virtual HRESULT InitProtoInstance();
public:
	CLodingObjProto(void);
	virtual ~CLodingObjProto(void);
};

