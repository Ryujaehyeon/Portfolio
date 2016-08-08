#pragma once
#include "stagedynamic.h"
class CNonPlayer :
	public CStageDynamic
{
public:
	CNonPlayer(void);
	CNonPlayer( const OBJINFO& Info,  TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~CNonPlayer(void);
};

