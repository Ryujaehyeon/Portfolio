#pragma once
#include "menudynamic.h"
class CMenuButton :
	public CMenuDynamic
{
protected:
	TCHAR* m_DrawID;
	bool m_bMouseOver;

	// �ҷ��� �̹����� �󸶸�ŭ�� ũ��� �׸����� ����
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

	// ���� �浹�ϴ� ��ǥ�� üũ
	//RECT RealRect()
	//{
	//	RECT rc =
	//	{
	//		int(m_Info.vPos.x - (m_Info.fCX * 0.5f)), 
	//		int(m_Info.vPos.y - (m_Info.fCY * 0.5f)),
	//		int(m_Info.vPos.x + (m_Info.fCX * 0.5f)), 
	//		int(m_Info.vPos.y + (m_Info.fCY * 0.5f))
	//	};
	//	return rc;
	//}
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
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

