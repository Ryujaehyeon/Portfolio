#include "StdAfx.h"
#include "StageBackground.h"
#include "Include.h"

CStageBackGround::CStageBackGround(void)
{
}

CStageBackGround::CStageBackGround(const OBJINFO& Info, const OBJ_TYPE _ObjType)
	:CStageStatic(Info, _ObjType)
{
}


CStageBackGround::~CStageBackGround(void)
{
	Release();
}

void CStageBackGround::InitTileFromFile()
{
	HANDLE hFile;
	DWORD dwByte;

	hFile = CreateFile(L"Map.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

	while(true)
	{
		TILE* pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		if(dwByte == 0)
		{
			SAFE_DELETE<TILE>(&pTile);
			break;
		}
		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
}

HRESULT CStageBackGround::Initialize()
{
	m_Info.vPos = D3DXVECTOR3(400.f, 300.f, 1.f);
	m_eLayer = LAYER_TERRAIN;
	InitTileFromFile();
	m_pObjKey = L"StageBack";
	m_pStateKey = L"Tile";

	return S_OK;
}

SCENEID CStageBackGround::Progress()
{
	//D3DXMATRIX matTrans;

	//D3DXMatrixTranslation(&matTrans, m_Info.vPos.x
	//	, m_Info.vPos.y, m_Info.vPos.z);

	//m_Info.matWorld = matTrans;

	return SCENEID_NONPASS;
}

void CStageBackGround::Render()
{
	D3DXMATRIX matTrans;
	int iSize = m_vecTile.size();

	//DebugLog(L"%5.1f, %5.1f, %5.1f, %5.1f", 
	//	CObj::g_tScroll.x, 
	//	CObj::g_tScroll.y, 
	//	m_vecTile[0]->vPos.x, 
	//	m_vecTile[0]->vPos.y);

	for(int i = 0; i < iSize; ++i)
	{
		const TEXINFO* pTexInfo = 
			GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey
			, m_pStateKey, m_vecTile[i]->byDrawID);
		if(pTexInfo == NULL)
			return;

		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x - CObj::g_tScroll.x, 
			m_vecTile[i]->vPos.y - CObj::g_tScroll.y,
			m_vecTile[i]->vPos.z);

		m_Info.vCenter = D3DXVECTOR3(65, 34, 0);

		GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matTrans);
		GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
			NULL, &m_Info.vCenter, NULL
			, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//const TEXINFO* pTexInfo = 
	//	GET_SINGLE(CTextureMgr)->GetTexture(m_pObjKey);
	//if(pTexInfo == NULL)
	//	return;
	//m_Info.vCenter = D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f,
	//	pTexInfo->ImgInfo.Height * 0.5f, 0.0f);

	//GET_SINGLE(CDevice)->GetSprite()->SetTransform(&m_Info.matWorld);
	//GET_SINGLE(CDevice)->GetSprite()->Draw(pTexInfo->pTexture,
	//	NULL, &m_Info.vCenter, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStageBackGround::Release()
{
	int iSize = m_vecTile.size();
	for (int i = 0 ; i < iSize ; ++i)
	{
		SAFE_DELETE<TILE>(&m_vecTile[i]);
	}
	m_vecTile.clear();
	m_vecTile.~vector();
}

CObj* CStageBackGround::Clone()
{
	return new CStageBackGround(*this);
}

POINT CStageBackGround::MouseInfo()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return pt;
	//return D3DXVECTOR3(pt.x , pt.y , 0);
}

D3DXVECTOR3 CStageBackGround::MouseInfoDX()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return D3DXVECTOR3(pt.x , pt.y , 0);
}
