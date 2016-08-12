#pragma once
#include "stagestatic.h"
class CUIObj :
	public CStageStatic
{
protected:
	list<CObj*>* m_PlayerData;
	int SelectCount;
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	RECT GetRect()
	{
		RECT rc =
		{
			int(0), 
			int( m_Info.fCY - m_Info.fCY * VelueToPercentage(m_pObjKey)),
			int( m_Info.fCX ), 
			int( m_Info.fCY )
		};
		return rc;
	}

	virtual void Setlist(list<CObj*>* _Player);

	virtual float VelueToPercentage(TCHAR* VelueName);
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	CUIObj(void);
	CUIObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CUIObj( const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType );
	virtual ~CUIObj(void);
};

