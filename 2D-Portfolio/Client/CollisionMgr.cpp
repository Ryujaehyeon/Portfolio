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

	//���� ����� �浹�ϸ� �߽����� �ƴ� ������ ǥ�鿡�� �浹�ؾ��ϱ� ������
	//�߽��������� ���� �Ÿ����� ������ �߽ɺ��� ǥ������� ���̸� ���� �����Ÿ� ������ ���ָ� �浹�Ÿ��� �ȴ�.

	//���ü �߽������� ǥ������� ���̿� ���ü �߽������� ǥ������� ������ ��
	float fDistance = float(
		(pDest->GetInfo().fCX * 0.5f /*��Ÿ� - 30 */)+
		(pTemp->GetInfo().fCX * 0.5f /*��Ÿ� - 30 */));

	//���ü�� ���ü�� ���� ���� �Ÿ�
	float fWidth = float((pDest->GetInfo().vPos.x+ CObj::g_tScroll.x) - (pTemp->GetInfo().vPos.x + CObj::g_tScroll.x));
		//+ CGameScene::g_tScroll.x; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

	//���ü�� ���ü�� ���� ���� �Ÿ�									   
	float fHeight = float((pDest->GetInfo().vPos.y + CObj::g_tScroll.y) - (pTemp->GetInfo().vPos.y + CObj::g_tScroll.y));
		//+ CGameScene::g_tScroll.y; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

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
				// ���� ������ �÷��̾� ���ݷº��� ũ�� ������ 
				if((*iter2)->GetStatas().fDefence >= (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 1;
				// �÷��̾� ���ݷ��� �� ũ�� ���� ������ �� ������ ��ŭ�� �������� ���� 
				else if((*iter2)->GetStatas().fDefence < (*iter)->GetStatas().fAttack)
					(*iter2)->SetStatas()->fHealthPoint -= 
					(*iter)->GetStatas().fAttack - (*iter2)->GetStatas().fDefence;
				
				//�浹�� ������
				SAFE_DELETE(&(*iter));
				//SAFE_DELETE(&(*iter2));


				//list�� ��带 ����
				//iter = pSkill->erase(iter);
				//iter2 = pMonsterList->erase(iter2);
				++iter;
				++iter2;
			}
			else //�浹 ���н�
			{
				//��ǥ�� �޾� ȭ�� ���� ��� ��
				if( (*iter)->GetInfo().vPos.y + CObj:: g_tScroll.y < 0		   + CObj:: g_tScroll.y  ||
					(*iter)->GetInfo().vPos.y + CObj:: g_tScroll.y > WINSIZEY  + CObj:: g_tScroll.y  ||
					(*iter)->GetInfo().vPos.x + CObj:: g_tScroll.x < 0		   + CObj:: g_tScroll.x  ||
					(*iter)->GetInfo().vPos.x + CObj:: g_tScroll.x > WINSIZEX  + CObj:: g_tScroll.x )
				{
					//���� �� ��� ����
					SAFE_DELETE(&(*iter));
					//iter = pSkill->erase(iter);
					++iter;
				}
				//���� ��带 ����Ų��.
				++iter2;
			}
			//iter�� ���� ����Ű�� �ݺ����� ����
			if (iter == pSkill->end())
				break;
		}
		//iter�� ���� ����Ű�� �ݺ����� ����
		if (iter == pSkill->end())
			break;
	}
}