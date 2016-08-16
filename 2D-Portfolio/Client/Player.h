#pragma once
#include "stagedynamic.h"
class CPlayer :
	public CStageDynamic
{
protected:
	D3DXVECTOR3 m_vMousePos;
	list<CObj*> m_SkillList;
public:
	void FrameStatas();
	// D3DVector�� ������ POINT��ȯ �� üũ
	bool VecterInRect(D3DXVECTOR3& _Taget);
	// ���� �ൿ�� Ű���� ������ _pObjKey�� m_pObjKey�� �ִ´�.
	void DirectAction( TCHAR* _pObjStatas );
	void CharacterSelect();
	void FuncAttack();
	void StatesChange();
	void ExpAcquired();
	virtual void Setlist(list<CObj*>* _Monster);
	virtual void ScrollChange();

	//void SetSkillList(list<CObj*>* _pSkillList);
	
	list<CObj*>* GetSkillList(){ return &m_SkillList; }
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

