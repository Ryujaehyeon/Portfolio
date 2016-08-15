#pragma once
#include "stagestatic.h"
class CUIObj :
	public CStageStatic
{
protected:
	list<CObj*>* m_PlayerData;
	list<CObj*>* m_MonsterData;
	int SelectCount;
	CObj* m_PlayerObj;
	bool m_InvenKey;
public:
	void CheckKey();
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	//RECT GetRect()
	//{
	//	RECT rc =
	//	{
	//		float(0), 
	//		float(m_Info.fCY - (m_Info.fCY * VelueToPercentage(m_pObjKey))),
	//		float(m_Info.fCX), 
	//		float(m_Info.fCY)
	//	};
	//	return rc;
	//}
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
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	CUIObj(void);
	CUIObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~CUIObj(void);
};

