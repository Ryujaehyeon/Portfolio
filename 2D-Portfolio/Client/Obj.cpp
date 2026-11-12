#include "StdAfx.h"
#include "Obj.h"
#include "Include.h"

CObj::CObj(void)
	:m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_tFrame()
	, m_fAngle(0.0f)
	, m_dwKey(0)
{
}

CObj::CObj(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	: m_Info(Info)
	, m_sPlayInfo()
	, m_ObjType(_ObjType)
	, m_pTagetObj(NULL)
	, m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_tFrame(0,0)
	, m_pMotion(STAND)
	, m_fAngle(0)
	, m_iDegree(0)
	, m_fChaterDirect(0)
	, m_bSelect(false)
	, m_bRun(true)
	, m_Crash(false)
	, m_dwKey(0)
{
}


CObj::CObj(const OBJINFO& Info, const TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	: m_Info(Info)
	, m_sPlayInfo()
	, m_ObjType(_ObjType)
	, m_pTagetObj(NULL)
	, m_pObjKey(NULL)
	, m_pStateKey(NULL)
	, m_tFrame(0,0)
	, m_pMotion(STAND)
	, m_fAngle(0)
	, m_iDegree(0)
	, m_fChaterDirect(0)
	, m_bSelect(false)
	, m_bRun(true)
	, m_Crash(false)
	, m_dwKey(0)
{
	wcscpy_s(m_sPlayInfo.szName, _ObjName);
}

CObj::~CObj(void)
{
}

D3DXVECTOR3 CObj::g_tScroll = D3DXVECTOR3(0.f, 0.f, 0.f);

void CObj::FrameMove( const float& fCnt, const float& fMax )
{
	m_tFrame.fStart += fCnt * GET_SINGLE(CTimeMgr)->DeltaTime();
	m_tFrame.fLast = fMax;

	// 한 바퀴가 끝나면 0 으로 되돌린다. 「fStart <= 0」 을 한 바퀴의 시작으로 보는 판정(스킬 생성·공격·경험치·시체 삭제)이
	// 리셋 직후 한 프레임 동안 참이 된다 — 구성과 무관하게 같아야 한다(intent/ub-fixes).
	// fMax 와 같을 때도 되돌린다 — 프레임 번호(int(fStart))가 텍스처 장 수(fMax)와 같아지면 범위를 넘는다(intent/frame-index-bounds).
	if (m_tFrame.fStart >= fMax)
	{
		m_tFrame.fStart = 0.f;
	}

}

int CObj::GetTileIndex( const D3DXVECTOR3& vPos )
{
	const vector<TILE*>* pVecTile = 
		GET_SINGLE(CObjMgr)->GetTileInfo();
	
	int iSize = static_cast<int>(pVecTile->size());
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

void CObj::SetTagetObj( CObj* _pTagetObj )
{
	m_pTagetObj = _pTagetObj;
}


POINT CObj::MouseInfo()
{
	POINT pt;
	GameCursorPos(g_hWnd, &pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x = static_cast<LONG>(pt.x + CObj::g_tScroll.x);
	pt.y = static_cast<LONG>(pt.y + CObj::g_tScroll.y);
	return pt;
}

D3DXVECTOR3 CObj::MouseInfoDX()
{
	POINT pt;
	GameCursorPos(g_hWnd, &pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3((pt.x + CObj::g_tScroll.x) , ( pt.y + CObj::g_tScroll.y) , 0.f);
}

void CObj::Setlist( list<CObj*>* _TagetList )
{
	m_pTagetList = _TagetList;
}