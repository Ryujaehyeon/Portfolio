#include "StdAfx.h"
#include "ObjProto.h"


CObjProto::CObjProto(void)
{
}

CObjProto::~CObjProto(void)
{
	Release();
}

CObj* CObjProto::GetProto(wstring pObjKey)
{
	map<wstring, CObj*>::iterator iter 
		= m_MapProto.find(pObjKey);
	if(iter == m_MapProto.end())
	{
		ERR_MSG(g_hWnd, L"복사하려는 원형 객체가 없습니다.");
		return NULL;
	}
	return iter->second->Clone();
}

void CObjProto::Release()
{
	for(map<wstring, CObj*>::iterator iter = m_MapProto.begin();
		iter != m_MapProto.end(); ++iter)
	{
		SAFE_DELETE<CObj>(&iter->second);
	}
	m_MapProto.clear();
}
