#pragma once
#include "objproto.h"
class CStageObjProto :
	public CObjProto
{
public:
	virtual HRESULT InitProtoInstance();
public:
	CStageObjProto(void);
	virtual ~CStageObjProto(void);
};

