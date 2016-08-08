#pragma once
#include "stagedynamic.h"
class CMonster :
	public CStageDynamic
{
	// 플레이어와 거리충돌체크 후 일정거리 이내에 들어오면 쫓아가 공격 멀어지면 돌아감
protected:
	D3DXVECTOR3 m_vMovePoint;
	int m_iDir;
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();

	void FrameStatas();
	// D3DVector를 가져와 POINT변환 후 체크
	bool VecterInRect(D3DXVECTOR3& _Taget);
	void SetMotionDirect(TCHAR* _MotionDirect);
	// 취할 행동의 키값을 넣으면 _pObjKey를 m_pObjKey에 넣는다.
	void DirectAction( TCHAR* _pObjStatas );
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	void CheckKey();
public:
	CMonster(void);
	CMonster(const OBJINFO& Info);
	CMonster(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CMonster(void);
};

