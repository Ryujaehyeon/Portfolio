#pragma once
#include "menudynamic.h"
class CMenuButton :
	public CMenuDynamic
{
protected:
	TCHAR* m_DrawID;
	bool m_bMouseOver;

	// 불러온 이미지의 얼마만큼의 크기로 그릴지를 결정
	RECT GetRect()
	{
		RECT rc =
		{
			int(0), 
			int(0 + ( m_Info.fCY * m_bMouseOver )),
			int(m_Info.fCX), 
			int(m_Info.fCY + ( m_Info.fCY * m_bMouseOver ))
		};
		return rc;
	}

public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	CMenuButton(void);
	CMenuButton(const OBJINFO& Info, TCHAR* ButtonName, const OBJ_TYPE _ObjType);
	virtual ~CMenuButton(void);
};

