#include "StdAfx.h"
#include "Ball.h"
#include "Include.h"

CBall::CBall(void)
{}

CBall::CBall(const OBJINFO& Info, const OBJ_TYPE _OjbType)
	:CLogoDynamic(Info, _OjbType)
{}

CBall::~CBall(void)
{
	Release();
}

HRESULT CBall::Initialize()
{
	// � ������Ʈ�� �̹����� �ҷ����� �����ϴ� ����
	m_eLayer = LAYER_OBJECT;
	m_pObjKey = L"Ball";
	m_pStateKey = NULL;
	return S_OK;
}

SCENEID CBall::Progress()
{
	// ����
	static float fAngle = 0.0f;
	// �ð�
	static float fTime = 0.0f;

	// ��׸�(����)�� ���Ȱ����� �ٲ��� 
	// �� ���� �ð����� ���ϸ� 360�� ȸ���� ������ ���� ������
	fAngle += D3DXToRadian(360.f) * GET_SINGLE(CTimeMgr)->DeltaTime();
	// �ð����� ���Ѵ�.
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// �ð��� ������ �������� ���ϴ� ��ID�� �Ѱܼ� ���� �ٲߴϴ�.
	if(fTime >= 1.0f)
		return SCENEID_MENU;

	D3DXMATRIX matTrans;
	D3DXMATRIX matRotZ;

	// ���س��� ������ �Լ��� ���
	D3DXMatrixRotationZ(&matRotZ, fAngle);
	// �ڽ��� ��ġ�� ����
	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);
	//< ũ�� * ���� * �̵� * ���� * �θ� >�ʿ信 ���� ���س��� �������� ���Ͽ� ���忡 �����Ѵ�.
	m_Info.matWorld = matRotZ * matTrans;

	// ���� �ѱ��� �ʴ´�.
	return SCENEID_NONPASS;
}

void CBall::Render()
{
	// �ؽ��ĸŴ����� ���� �ؽ��ĸ� �����´�.
	// �̹� Initialize���� ������ ������Ʈ�� �̸����� ���ؿ´�.
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	// ������ �����Ͽ� ����
	if(pTexInfo == NULL)
		return;

	// �̹����� ũ�⸦ ������ �Ͽ� �߾Ӱ��� �����Ѵ�.
	m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
		pTexInfo->ImgInfo.Height * 0.5f, 0);

	GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
		NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(0, 255, 255, 255));
}

void CBall::Release()
{

}

CObj* CBall::Clone()
{
	return new CBall(*this);
}

POINT CBall::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CBall::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
