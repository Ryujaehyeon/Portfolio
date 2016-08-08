#include "StdAfx.h"
#include "StageStatic.h"

CStageStatic::CStageStatic(void)
{
}


CStageStatic::CStageStatic(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:CObj(Info, _ObjType)
{

}


CStageStatic::CStageStatic(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CObj(Info, _ObjName, _ObjType)
{

}


CStageStatic::~CStageStatic(void)
{
}
