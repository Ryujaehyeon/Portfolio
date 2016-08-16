#include "StdAfx.h"
#include "SkillTree.h"


CSkillTree::CSkillTree(void)
{
}


CSkillTree::~CSkillTree(void)
{
}

HRESULT CSkillTree::Initialize()
{
	return S_OK;
}

SCENEID CSkillTree::Progress()
{
	return SCENEID_NONPASS;
}

void CSkillTree::Render()
{

}

void CSkillTree::Release()
{

}

CObj* CSkillTree::Clone()
{
	return new CSkillTree(*this);
}
