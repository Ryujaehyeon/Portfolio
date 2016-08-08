#include "StdAfx.h"
#include "Obj.h"
#include "Include.h"

CObj::CObj(void)
	:m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_fFrame(0)
	, m_tFrame()
	, m_fAngle(0.0f)
{
}

CObj::CObj(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:m_Info(Info)
	, m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_fFrame(0)
	, m_ObjType(_ObjType)
	, m_tFrame(0,0)
	, m_bRun(true)
{
}


CObj::CObj(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:m_Info(Info)
	, m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_fFrame(0)
	, m_ObjType(_ObjType)
	, m_tFrame(0,0)
	, m_bRun(true)
	, m_pObjName(_ObjName)
{
	wcscpy_s(m_sPlayInfo.szName, _ObjName);
}

CObj::~CObj(void)
{
}

void CObj::FrameMove( const float& fCnt, const float& fMax )
{
	m_fFrame += fCnt * GET_SINGLE(CTimeMgr)->DeltaTime();
	m_tFrame.fStart += fCnt * GET_SINGLE(CTimeMgr)->DeltaTime();
	m_tFrame.fLast = fMax;

#if _DEBUG
	// 디버그 모드 시 브레이크로 정지 후 다시 진행할때 
	// 들어오는 타임값이 max값을 초과해 프레임값으로 인해 에러가 나지 않게
	// 초기화
	if (m_fFrame > fMax)
		m_fFrame = 0;
	if (m_tFrame.fStart > fMax)
		m_tFrame.fStart = 0;
#else
	// 오차값을 줄이기 위해
	if (m_fFrame > fMax)
		m_fFrame -= fMax;
	if (m_tFrame.fStart > fMax)
		m_tFrame.fStart -= fMax;
#endif

}

int CObj::GetTileIndex( const D3DXVECTOR3& vPos )
{
	const vector<TILE*>* pVecTile = 
		GET_SINGLE(CObjMgr)->GetTileInfo();
	
	int iSize = pVecTile->size();
	for (int i = 0; i < iSize; ++i)
	{
		if (CollisionMouseToTile(vPos, (*pVecTile)[i]))
		{
			return i;
		}
	}	
	return -1;
}

bool CObj::CollisionMouseToTile( const D3DXVECTOR3& vPos, const TILE* pTileInfo )
{
	// y = ax + b;
	// a? == 기울기는?
	float fGrad = (TILESIZEY * 0.5f) / (TILESIZEX * 0.5f);


	// b? == 직선이 어느점을 관통하는지?
	// b = y - ax;

	D3DXVECTOR2 vPoint[4];

	vPoint[0] = D3DXVECTOR2(pTileInfo->vPos.x
		, pTileInfo->vPos.y + TILESIZEY * 0.5f);
	vPoint[1] = D3DXVECTOR2(pTileInfo->vPos.x + TILESIZEX * 0.5f
		, pTileInfo->vPos.y);
	vPoint[2] = D3DXVECTOR2(pTileInfo->vPos.x
		, pTileInfo->vPos.y - TILESIZEY * 0.5f);
	vPoint[3] = D3DXVECTOR2(pTileInfo->vPos.x - TILESIZEX * 0.5f
		, pTileInfo->vPos.y);

	float fB[4] = 
	{
		vPoint[0].y - fGrad * vPoint[0].x,
		vPoint[1].y + fGrad * vPoint[1].x,
		vPoint[2].y - fGrad * vPoint[2].x,
		vPoint[3].y + fGrad * vPoint[3].x
	};

	// y = ax + b;
	// y - ax - b = 0	 //직선상에 존재한다.
	// y - ax - b < 0	 //직선의 아래에 있다
	// y - ax - b > 0	 //직선의 위에 있다.

	if(vPos.y -  fGrad * vPos.x - fB[0] < 0 &&
	   vPos.y - -fGrad * vPos.x - fB[1] < 0 &&
	   vPos.y -  fGrad * vPos.x - fB[2] >= 0 &&
	   vPos.y - -fGrad * vPos.x - fB[3] >= 0)
	{
		return true;
	}

	return false;
}

