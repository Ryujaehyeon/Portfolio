#pragma once
#include "stagestatic.h"
class CStageBackGround :
	public CStageStatic
{
private:
	vector<TILE*> m_vecTile;
public:
	void InitTileFromFile();
	const vector<TILE*>* GetVecTile() const
	{	return &m_vecTile;	}
public:
	virtual HRESULT Initialize();
	virtual SCENEID Progress();
	virtual void Render();
	virtual void Release();
	virtual CObj* Clone();
public:
	virtual POINT MouseInfo();
	virtual D3DXVECTOR3 MouseInfoDX();
public:
	CStageBackGround(void);
	CStageBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType);
	virtual ~CStageBackGround(void);
};

