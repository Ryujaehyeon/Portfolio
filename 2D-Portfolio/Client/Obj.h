#pragma once

#include "Defines.h"
class CPrototype;
class CObj
{
protected:
	// 좌표를 모아놓은 구조체
	OBJINFO m_Info;
	// 플레이에 사용되는 스테이터스 구조체
	INFO m_sPlayInfo;
	// 객체 타입
	OBJ_TYPE m_ObjType;
	// 대상이 되는 객체
	CObj* m_pTagetObj;
	// 대상이되는 리스트
	list<CObj*>* m_pTagetList;
	// 오브젝트의 이름
	TCHAR* m_pObjName;
	// 오브젝트 이미지 키이름
	TCHAR*  m_pObjKey;
	// 오브젝트 상태 
	TCHAR*	m_pStateKey;
	// 오브젝트 방향
	TCHAR*  m_pMotion;
	// 오브젝트 회전값
	float m_fAngle;
	// 특정 지점을 볼때 그곳을 보는 오브젝트의 각도
	float m_iDegree;
	// 캐릭터가 가야할 방향(각도)
	float m_fChaterDirect;
	// 이동할 지점 좌표	
	D3DXVECTOR3 m_vTagetInfo;
	// 캐릭터 선택 여부
	bool m_bSelect;
	// 뛰거나 걷거나의 여부
	bool m_bRun;
	// 충돌여부 충돌 true, 충돌 안함 false
	bool m_Crash;
	// 스킬
	CPrototype* m_pSkillPrototype;
protected:
	// 키입력
	DWORD m_dwKey;
	// 레이어
	SORTLAYER m_eLayer;
	// 프레임 구조체 시작프레임과 끝프레임을 관리함
	FRAME m_tFrame;

public:
	// 스크롤
	static D3DXVECTOR3 g_tScroll;
public:
	RECT GetRect()
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
	virtual void SetInfoPos(float _x, float _y){m_Info.vPos.x = _x; m_Info.vPos.y = _y; }
	virtual INFO* SetStatas() {return &m_sPlayInfo;}
	virtual void SetCrash(bool _bCrash){m_Crash = _bCrash;}
	virtual void SetTagetObj(CObj* _pTagetObj);
	virtual void SetMotion(TCHAR* _pMotion) { if(m_pMotion != DEATH) m_pMotion = _pMotion; }
	virtual void SetAngle(float _Angle){m_fAngle = _Angle;}
	virtual void Setlist( list<CObj*>* _TagetList );

	//Get
	virtual SORTLAYER GetLayer() const {return m_eLayer;}
	virtual OBJ_TYPE GetObjType() const {return m_ObjType;}
	virtual const bool GetCrash() const {return m_Crash;}
	virtual const OBJINFO& GetInfo() const {return m_Info;}
	virtual const INFO& GetStatas() const {return m_sPlayInfo;}
	virtual const FRAME& GetFrame() const {return m_tFrame;}
	virtual const TCHAR* GetpMotion() const {return m_pMotion;}
	virtual const bool GetSelect() const { return m_bSelect; }
	virtual const TCHAR* GetName() const { return m_pObjName; }
	virtual const CObj* GetTagetObj() const { if(m_pTagetObj != NULL) return m_pTagetObj; else return nullptr; }
	virtual const CPrototype* GetSkillList() const { return m_pSkillPrototype; }
	//etc
public:
	void FrameMove(const float& fCnt, const float& fMax);
	int GetTileIndex(const D3DXVECTOR3& vPos);
	bool CollisionMouseToTile(const D3DXVECTOR3& vPos, const TILE* pTileInfo);
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
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

