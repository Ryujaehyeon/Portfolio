#include "StdAfx.h"
#include "CollisionMgr.h"
#include "Include.h"

CCollisionMgr::CCollisionMgr(void)
{
}


CCollisionMgr::~CCollisionMgr(void)
{
}
bool CCollisionMgr::CircleCol( CObj* pTemp, CObj* pDest)
{
	//대상과 대상이 충돌하면 중심점이 아닌 서로의 표면에서 충돌해야하기 때문에
	//중심점끼리의 실제 거리에서 서로의 중심부터 표면까지의 길이를 합해 실제거리 값에서 빼주면 충돌거리가 된다.

	//대상체 중심점부터 표면까지의 길이와 대상체 중심점부터 표면까지의 길이의 합
	float fDistance = float(
		(pDest->GetInfo().fCX * 0.5f /*사거리 - 30 */)+
		(pTemp->GetInfo().fCX * 0.5f /*사거리 - 30 */));

	//대상체와 대상체의 실제 가로 거리
	float fWidth = float(pDest->GetInfo().fX - pTemp->GetInfo().fX);
		//+ CGameScene::g_tScroll.x; // 캐릭터가 이동한 만큼 변경된 스크롤값을 더함

	//대상체와 대상체의 실제 세로 거리									   
	float fHeight = float(pDest->GetInfo().fY - pTemp->GetInfo().fY);
		//+ CGameScene::g_tScroll.y; // 캐릭터가 이동한 만큼 변경된 스크롤값을 더함

	float fRealDistance = 
		sqrt(fWidth * fWidth + fHeight * fHeight);

	if (fDistance > fRealDistance)
		return true;

	return false;
}
