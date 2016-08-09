#pragma once

#include "Defines.h"
#include "Obj.h"
#include "Prototype.h"

class CObjMgr
{
	SINGLETON(CObjMgr);
private:
	map<wstring, list<CObj*>> m_MapObject;
public:
	const vector<TILE*>* GetTileInfo();
	// ��ü�� ��ȣ�ۿ� ���α׷������� ���ڸ� �޾Ƽ� 
	// �ݺ������� iter�� �Ȱ��� �������� map������ ��
	void ObjInteraction(CObj* pSour);
	// ��ü���� �浹�� �о��
	void CrashAndSlide(CObj* _pDest, CObj* _pSour);
	// �ɷ¿� ���� ��ȯ
	void AbilityTointeract(CObj* _pDest, CObj* _pSour);
	// ��ȣ�ۿ� 
	// ���ݽ� ���� ��ȭ
	void AttackFunc(CObj* _pDest, CObj* _pSour);
	float DegreeMon( CObj* _pDest, CObj* _pSour );
	float DegreeMouse(CObj* _pSour );
	void MonsterRelease();
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();

public:
	HRESULT AddObject(CPrototype* pProto, wstring pObjKey);
	SCENEID Progress();
	void Render();
	void Release();
private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};

