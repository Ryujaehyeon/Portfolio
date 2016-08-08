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
	// 어떤 오브젝트와 이미지를 불러올지 결정하는 값들
	m_eLayer = LAYER_OBJECT;
	m_pObjKey = L"Ball";
	m_pStateKey = NULL;
	return S_OK;
}

SCENEID CBall::Progress()
{
	// 각도
	static float fAngle = 0.0f;
	// 시간
	static float fTime = 0.0f;

	// 디그리(각도)을 라디안값으로 바꾼후 
	// 그 값을 시간값과 곱하면 360도 회전이 가능한 값이 생성됨
	fAngle += D3DXToRadian(360.f) * GET_SINGLE(CTimeMgr)->DeltaTime();
	// 시간값을 구한다.
	fTime += GET_SINGLE(CTimeMgr)->DeltaTime();

	// 시간이 지나면 리턴으로 원하는 씬ID로 넘겨서 씬을 바꿉니다.
	if(fTime >= 1.0f)
		return SCENEID_MENU;

	D3DXMATRIX matTrans;
	D3DXMATRIX matRotZ;

	// 구해놓은 각도를 함수에 사용
	D3DXMatrixRotationZ(&matRotZ, fAngle);
	// 자신의 위치를 지정
	D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
		, m_Info.vPos.y, m_Info.vPos.z);
	//< 크기 * 자전 * 이동 * 공전 * 부모 >필요에 의해 구해놓은 값끼리만 곱하여 월드에 대입한다.
	m_Info.matWorld = matRotZ * matTrans;

	// 씬을 넘기지 않는다.
	return SCENEID_NONPASS;
}

void CBall::Render()
{
	// 텍스쳐매니저를 통해 텍스쳐를 가져온다.
	// 이미 Initialize에서 정해진 오브젝트의 이름으로 구해온다.
	const TEXINFO* pTexInfo 
		= GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);

	// 없으면 실패하여 리턴
	if(pTexInfo == NULL)
		return;

	// 이미지의 크기를 반으로 하여 중앙값을 저장한다.
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
