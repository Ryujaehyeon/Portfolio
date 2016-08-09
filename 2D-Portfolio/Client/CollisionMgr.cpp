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
	float fWidth = float(pDest->GetInfo().fX - pTemp->GetInfo().fX);
		//+ CGameScene::g_tScroll.x; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

	//���ü�� ���ü�� ���� ���� �Ÿ�									   
	float fHeight = float(pDest->GetInfo().fY - pTemp->GetInfo().fY);
		//+ CGameScene::g_tScroll.y; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

	float fRealDistance = 
		sqrt(fWidth * fWidth + fHeight * fHeight);

	if (fDistance > fRealDistance)
		return true;

	return false;
}
