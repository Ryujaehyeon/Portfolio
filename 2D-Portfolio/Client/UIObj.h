#pragma once
#include "stagestatic.h"
class CUIObj :
	public CStageStatic
{
	static bool m_InvenKey;
	static bool m_SkillTree;
	static bool m_Bank;
	static bool m_Character;
protected:
	list<CObj*>* m_PlayerData;
	list<CObj*>* m_MonsterData;
	int SelectCount;
	CObj* m_PlayerObj;
public:
	void CheckKey();
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	virtual POINT MouseInfo();
	RECT RealRect()
	{
		RECT rc =
		{
			int(m_Info.vPos.x - (m_Info.fCX * 0.5f)), 
			int(m_Info.vPos.y - (m_Info.fCY * 0.5f)),
			int(m_Info.vPos.x + (m_Info.fCX * 0.5f)), 
			int(m_Info.vPos.y + (m_Info.fCY * 0.5f))
		};
		return rc;
	}
	virtual void Setlist(list<CObj*>* _list);

	virtual float VelueToPercentage(TCHAR* VelueName);
public:
	CUIObj(void);
	CUIObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~CUIObj(void);
};

