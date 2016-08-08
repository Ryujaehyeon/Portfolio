#include "StdAfx.h"
#include "StageDynamic.h"
#include "Include.h"

CStageDynamic::CStageDynamic(void)
{
}


CStageDynamic::CStageDynamic(const OBJINFO& Info, TCHAR* _ObjName, const OBJ_TYPE _ObjType)
	:CObj(Info, _ObjName, _ObjType)
{

}

CStageDynamic::~CStageDynamic(void)
{
}

void CStageDynamic::SetDirect( const D3DXVECTOR3& Dest )
{
	D3DXVECTOR3 Dir = Dest - m_Info.vPos;
	D3DXVec3Normalize(&Dir, &Dir);
	D3DXVec3Normalize( &m_Info.vLook, &m_Info.vLook );

	m_fAngle = acosf(D3DXVec3Dot(&Dir, &m_Info.vLook));

	if (Dest.y < m_Info.vPos.y)
		m_fAngle = 2 * D3DX_PI - m_fAngle;
}

void CStageDynamic::ASterBegin( 
	const D3DXVECTOR3& vDestPos, const D3DXVECTOR3& vSourPos )
{
	int iStartIdx = GetTileIndex(vDestPos);
	if (iStartIdx < 0 )
		return;

	int iEndIdx = GetTileIndex(vSourPos);
	if ( iEndIdx < 0)
		return;

	GET_SINGLE(CAStar)->AStarBegin(iStartIdx, iEndIdx);
}




void CStageDynamic::AStarMove()
{
	list<int>* pBestList = GET_SINGLE(CAStar)->GetBestList();
	if (pBestList->empty())
		return;
	
	const vector<TILE*>* pVecTile = GET_SINGLE(CObjMgr)->GetTileInfo();

	int iDestIdx = pBestList->front();

	m_Info.vDir = (*pVecTile)[iDestIdx]->vPos - m_Info.vPos;

	float fDistance = D3DXVec3Length(&m_Info.vDir);

	D3DXVec3Normalize(&m_Info.vDir, &m_Info.vDir);

	m_Info.vPos += m_Info.vDir * 100.f * GET_SINGLE(CTimeMgr)->DeltaTime();

	if (fDistance < 5.f)
		pBestList->pop_front();

}

