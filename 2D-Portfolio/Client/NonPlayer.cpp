#include "StdAfx.h"
#include "NonPlayer.h"


CNonPlayer::CNonPlayer(void)
{
}

CNonPlayer::CNonPlayer( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType )
	:CStageDynamic(Info, _ObjName, _ObjType)
{

}


CNonPlayer::~CNonPlayer(void)
{
}
