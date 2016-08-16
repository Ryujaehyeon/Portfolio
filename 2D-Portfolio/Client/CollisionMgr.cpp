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
	float fWidth = float((pDest->GetInfo().vPos.x+ CObj::g_tScroll.x) - (pTemp->GetInfo().vPos.x + CObj::g_tScroll.x));
		//+ CGameScene::g_tScroll.x; // 캐릭터가 이동한 만큼 변경된 스크롤값을 더함

	//대상체와 대상체의 실제 세로 거리									   
	float fHeight = float((pDest->GetInfo().vPos.y + CObj::g_tScroll.y) - (pTemp->GetInfo().vPos.y + CObj::g_tScroll.y));
		//+ CGameScene::g_tScroll.y; // 캐릭터가 이동한 만큼 변경된 스크롤값을 더함

	float fRealDistance = 
		sqrt(fWidth * fWidth + fHeight * fHeight);

	if (fDistance > fRealDistance)
		return true;

	return false;
}

void CCollisionMgr::ColCircle(list<CObj*> *pSkill, 
	list<CObj*> *pMonsterList)
{
	for (list<CObj*>::iterator iter = pSkill->begin();
		iter != pSkill->end(); ++iter)
	{
		for (list<CObj*>::iterator iter2 = pMonsterList->begin();
			iter2 != pMonsterList->end();)
		{
			if ((*iter) == nullptr || (*iter2) == nullptr)
				break;

			if (CircleCol(*iter, *iter2))
			{
				// 몬스터 방어력이 플레이어 공격력보다 크면 데미지 
				if((*iter2)->GetStatas().fDefence >= (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 1;
				// 플레이어 공격력이 더 크면 몬스터 방어력을 뺀 나머지 만큼의 데미지만 적용 
				else if((*iter2)->GetStatas().fDefence < (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 
					(*iter)->GetStatas().fAttack - (*iter2)->GetStatas().fDefence;
				
				//충돌시 데미지
				SAFE_DELETE(&(*iter));
				//SAFE_DELETE(&(*iter2));


				//list의 노드를 삭제
				//iter = pSkill->erase(iter);
				//iter2 = pMonsterList->erase(iter2);
				++iter;
				++iter2;
			}
			else //충돌 실패시
			{
				//좌표를 받아 화면 밖을 벗어날 시
				if( (*iter)->GetInfo().vPos.y + CObj:: g_tScroll.y < 0		   + CObj:: g_tScroll.y  ||
					(*iter)->GetInfo().vPos.y + CObj:: g_tScroll.y > WINSIZEY  + CObj:: g_tScroll.y  ||
					(*iter)->GetInfo().vPos.x + CObj:: g_tScroll.x < 0		   + CObj:: g_tScroll.x  ||
					(*iter)->GetInfo().vPos.x + CObj:: g_tScroll.x > WINSIZEX  + CObj:: g_tScroll.x )
				{
					//해제 및 노드 삭제
					SAFE_DELETE(&(*iter));
					//iter = pSkill->erase(iter);
					++iter;
				}
				//다음 노드를 가르킨다.
				++iter2;
			}
			//iter가 끝을 가르키면 반복문을 끝냄
			if (iter == pSkill->end())
				break;
		}
		//iter가 끝을 가르키면 반복문을 끝냄
		if (iter == pSkill->end())
			break;
	}
}