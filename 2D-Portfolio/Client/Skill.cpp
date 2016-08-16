#include "StdAfx.h"
#include "Skill.h"
#include "Include.h"

CSkill::CSkill( void )
{

}

CSkill::CSkill(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CObj(Info, _ObjName, _ObjType)
{
}

CSkill::~CSkill(void)
{
}

HRESULT CSkill::Initialize()
{
	
	return S_OK;
}

SCENEID CSkill::Progress()
{
	return SCENEID_NONPASS;
}

void CSkill::Render()
{

}

void CSkill::Release()
{

}

CObj* CSkill::Clone()
{
	return new CSkill(*this);
}

