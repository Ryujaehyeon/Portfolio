#pragma once
#include "objproto.h"
class CSkillProto :
	public CObjProto
{
public:
	virtual HRESULT InitProtoInstance();
public:
	CSkillProto(void);
	virtual ~CSkillProto(void);
};

