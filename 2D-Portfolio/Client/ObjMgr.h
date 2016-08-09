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
	// 객체간 상호작용 프로그래스에서 인자를 받아서 
	// 반복문으로 iter로 똑같은 시점에서 map끝까지 비교
	void ObjInteraction(CObj* pSour);
	// 객체끼리 충돌시 밀어내기
	void CrashAndSlide(CObj* _pDest, CObj* _pSour);
	// 능력에 따른 교환
	void AbilityTointeract(CObj* _pDest, CObj* _pSour);
	// 상호작용 
	// 공격시 상태 변화
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

