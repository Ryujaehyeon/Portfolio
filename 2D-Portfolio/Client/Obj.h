#pragma once

#include "Defines.h"

class CObj
{
protected:
	OBJINFO m_Info;
	INFO m_sPlayInfo;
	OBJ_TYPE m_ObjType;
protected:
	// ������Ʈ�� �̸�
	TCHAR* m_pObjName;
	// ������Ʈ �̹��� Ű�̸�
	TCHAR*  m_pObjKey;
	// ������Ʈ ���� 
	TCHAR*	m_pStateKey;
	// ������Ʈ ����
	TCHAR*  m_pMotion;
	// ������Ʈ ȸ����
	float m_fAngle;
	// Ư�� ������ ���� �װ��� ���� ������Ʈ�� ����
	float m_iDegree;
	// ĳ���Ͱ� ������ ����(����)
	float m_fChaterDirect;
	// �̵��� ���� ��ǥ	
	D3DXVECTOR3 m_vTagetInfo;
	// ĳ���� ���� ����
	bool m_bSelect;
	// �ٰų� �Ȱų��� ����
	bool m_bRun;
	// �浹���� �浹 true, �浹 ���� false
	bool m_Crash;
protected:
	SORTLAYER m_eLayer;

	// ������
	float m_fFrame;

	// ������ ����ü ���������Ӱ� ���������� ������
	FRAME m_tFrame;

public:
	SORTLAYER GetLayer() const {return m_eLayer;}
	OBJ_TYPE GetObjType() const {return m_ObjType;}
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
	D3DXVECTOR3 SetTagetInfo(){return m_vTagetInfo;}
public:
	//Set
	virtual OBJINFO* Setinfo(){return &m_Info;};
	virtual INFO* SetStatasInfo() {return &m_sPlayInfo;}
	virtual bool SetCrash(){return m_Crash;}

	//Get
	virtual const OBJINFO& GetInfoPos() const {return m_Info;}
	virtual const INFO& GetStatasInfo() const {return m_sPlayInfo;}
	virtual const FRAME& GetFrame() const {return m_tFrame;}
	virtual const TCHAR* GetpMotion() const {return m_pMotion;}
public:
	void FrameMove(const float& fCnt, const float& fMax);
	int GetTileIndex(const D3DXVECTOR3& vPos);
	bool CollisionMouseToTile(const D3DXVECTOR3& vPos, const TILE* pTileInfo);
	virtual POINT MouseInfo()PURE;
	virtual D3DXVECTOR3 MouseInfoDX()PURE;
public:
	virtual HRESULT Initialize()PURE;
	virtual SCENEID Progress()PURE;
	virtual void Render()PURE;
	virtual void Release()PURE;
	virtual CObj* Clone()PURE;
public:
	CObj(void);
	CObj(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	CObj(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CObj(void);
};

