// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "MiniView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MFC_000View.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	GET_SINGLE(CDevice)->Render_Begin();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	D3DXMATRIX matWorld, matScale;

	D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);

	vector<TILE*>* pTile =
		&pMainFrm->m_pMainView->m_vecTile;
	vector<OBJINFO*>* pMonster = 
		&pMainFrm->m_pMainView->m_vecMonster;

	const TEXINFO* pTex;

	// 미니맵 타일그리기
	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int idx = i * TILECNTX + j;

			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"Tile"
				, (*pTile)[idx]->byDrawID);
			if(pTex == NULL)
				continue;

			D3DXMatrixTranslation(&matWorld
				, (*pTile)[idx]->vPos.x * 0.3f
				, (*pTile)[idx]->vPos.y * 0.3f
				, (*pTile)[idx]->vPos.z * 0.3f);

			matWorld = matScale * matWorld;

			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
				NULL, &D3DXVECTOR3(TILESIZEX * 0.5f, TILESIZEY * 0.5f, 0), NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// 미니맵 몬스터 그리기
	
	// 몬스터 페이지(page3)에서 조절한 값으로 
	float miniScale = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_fScrollval;
	D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);
	
	for(int i = 0; i < WINSIZEY / (TILESIZEY * 0.5f) + 2; ++i)
	{
		for(int j = 0; j < WINSIZEX / TILESIZEX + 2; ++j)
		{
			int idx = i * TILECNTX + j;

			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			if((*pMonster)[idx] == 0)
				continue;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"Monster"
				, (*pMonster)[idx]->byDrawID);
			if(pTex == NULL)
				continue;
						
			// 미니맵 크기에 맞게 스케일을 재조정
			// 0724 개별 스케일 
			 //if( (*pMonster)[idx] != NULL && (*pMonster)[idx]->unitSelect == true)
			 //{
				// D3DXMatrixScaling(&matScale, miniScale, miniScale, miniScale);
			 //}

			D3DXMatrixTranslation(&matWorld
				, (*pMonster)[idx]->vPos.x * 0.3f
				, (*pMonster)[idx]->vPos.y * 0.3f
				, (*pMonster)[idx]->vPos.z * 0.3f);

			matWorld = matScale * matWorld;

			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(
				pTex->pTexture,	NULL
				, &D3DXVECTOR3(pTex->ImgInfo.Width * 0.5f
				, pTex->ImgInfo.Height, 0), NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	GET_SINGLE(CDevice)->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
