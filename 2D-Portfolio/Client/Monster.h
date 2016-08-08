#pragma once
#include "stagedynamic.h"
class CMonster :
	public CStageDynamic
{
	// �÷��̾�� �Ÿ��浹üũ �� �����Ÿ� �̳��� ������ �Ѿư� ���� �־����� ���ư�
protected:
	D3DXVECTOR3 m_vMovePoint;
	int m_iDir;
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();

	void FrameStatas();
	// D3DVector�� ������ POINT��ȯ �� üũ
	bool VecterInRect(D3DXVECTOR3& _Taget);
	void SetMotionDirect(TCHAR* _MotionDirect);
	// ���� �ൿ�� Ű���� ������ _pObjKey�� m_pObjKey�� �ִ´�.
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

