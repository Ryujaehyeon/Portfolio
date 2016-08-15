#pragma once
#include "stagedynamic.h"
class CPlayer :
	public CStageDynamic
{
protected:
	D3DXVECTOR3 m_vMousePos;
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();

	void FrameStatas();
	// D3DVector를 가져와 POINT변환 후 체크
	bool VecterInRect(D3DXVECTOR3& _Taget);
	// 취할 행동의 키값을 넣으면 _pObjKey를 m_pObjKey에 넣는다.
	void DirectAction( TCHAR* _pObjStatas );
	void CharacterSelect();
	void FuncAttack();
	void StatesChange();
	void ExpAcquired();
	virtual void Setlist(list<CObj*>* _Monster);
	virtual void ScrollChange();
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	void CheckKey();
public:
	CPlayer(void);
	CPlayer(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType);
	virtual ~CPlayer(void);
};

