#pragma once
#include "objproto.h"
class LoadingObjProto :
	public CObjProto
{
public:
	virtual HRESULT InitProtoInstance();
public:
	LoadingObjProto(void);
	virtual ~LoadingObjProto(void);
};

