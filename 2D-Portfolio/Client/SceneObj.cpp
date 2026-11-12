#include "StdAfx.h"
#include "SceneObj.h"


CSceneObj::CSceneObj(void)
	// 장면 대부분이 m_pSkillPrototype 을 만들지 않는데 CStage 의 Release 가 지우므로 nullptr 로 시작한다
	: m_pPrototype(nullptr)
	, m_pSkillPrototype(nullptr)
{
	
}


CSceneObj::~CSceneObj(void)
{
}
