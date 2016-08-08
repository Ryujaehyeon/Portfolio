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
		(pDest->GetInfoPos().fCX * 0.5f /*��Ÿ� - 30 */)+
		(pTemp->GetInfoPos().fCX * 0.5f /*��Ÿ� - 30 */));

	//���ü�� ���ü�� ���� ���� �Ÿ�
	float fWidth = float(pDest->GetInfoPos().fX - pTemp->GetInfoPos().fX);
		//+ CGameScene::g_tScroll.x; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

	//���ü�� ���ü�� ���� ���� �Ÿ�									   
	float fHeight = float(pDest->GetInfoPos().fY - pTemp->GetInfoPos().fY);
		//+ CGameScene::g_tScroll.y; // ĳ���Ͱ� �̵��� ��ŭ ����� ��ũ�Ѱ��� ����

	float fRealDistance = 
		sqrt(fWidth * fWidth + fHeight * fHeight);

	if (fDistance > fRealDistance)
		return true;

	return false;
}
