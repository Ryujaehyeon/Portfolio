
// MFC_000View.cpp : CMFC_000View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_000.h"
#endif

#include "MFC_000Doc.h"
#include "MFC_000View.h"
#include "Include.h"
#include "MainFrm.h"
#include "MiniView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_000View

IMPLEMENT_DYNCREATE(CMFC_000View, CScrollView)

BEGIN_MESSAGE_MAP(CMFC_000View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_000View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFC_000View 생성/소멸

CMFC_000View::CMFC_000View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_000View::~CMFC_000View()
{
}

BOOL CMFC_000View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMFC_000View 그리기

void CMFC_000View::OnDraw(CDC* /*pDC*/)
{
	CMFC_000Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	GET_SINGLE(CDevice)->Render_Begin();

	D3DXMATRIX matWorld, matScale;
	D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);
	const TEXINFO* pTex;

	// 타일
	for(int i = 0; i < WINSIZEY / (TILESIZEY * 0.5f) + 2; ++i)
	{
		for(int j = 0; j < WINSIZEX / TILESIZEX + 2; ++j)
		{
			const TEXINFO* pTex;
			int idx = (i + 
				GetScrollPos(1) / (TILESIZEY / 2))
				* TILECNTX + (j + 
				GetScrollPos(0) / (TILESIZEX));

			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"Tile"
				, m_vecTile[idx]->byDrawID);

			if(pTex == NULL)
				continue;

			D3DXMatrixTranslation(&matWorld
				, m_vecTile[idx]->vPos.x - GetScrollPos(0)
				, m_vecTile[idx]->vPos.y - GetScrollPos(1)
				, m_vecTile[idx]->vPos.z);
			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
				NULL, &D3DXVECTOR3(TILESIZEX *0.5f, TILESIZEY *0.5f, 0), NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// 타일 선택
	for(int i = 0; i < WINSIZEY / (TILESIZEY * 0.5f) + 2; ++i)
	{
		for(int j = 0; j < WINSIZEX / TILESIZEX + 2; ++j)
		{
			const TEXINFO* pTex;
			int idx = (i + 
				GetScrollPos(1) / (TILESIZEY / 2))
				* TILECNTX + (j + 
				GetScrollPos(0) / (TILESIZEX));

			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"TileSelect"
				, 0);

			if(pTex == NULL)
				continue;

			D3DXMatrixTranslation(&matWorld
				, m_vecTile[idx]->vPos.x - GetScrollPos(0)
				, m_vecTile[idx]->vPos.y - GetScrollPos(1)
				, m_vecTile[idx]->vPos.z);

			if (m_vecTile[idx]->TileSelect == true)
			{		
				GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
				GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
					NULL, &D3DXVECTOR3(TILESIZEX *0.5f, TILESIZEY *0.5f, 0), NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
	int icnt = m_vecMonster.size();
	

	// 몬스터
	for(int i = 0; i < WINSIZEY / (TILESIZEY * 0.5f) + 2; ++i)
	{
		for(int j = 0; j < WINSIZEX / TILESIZEX + 2; ++j)
		{
			int idx = (i + 
				GetScrollPos(1) / (TILESIZEY / 2))
				* TILECNTX + (j + 
				GetScrollPos(0) / (TILESIZEX));
			
			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			if (m_vecMonster[idx] == 0)
				continue;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"Monster"
				, m_vecMonster[idx]->byDrawID);

			if(pTex == NULL)
				continue;
			
			D3DXMatrixTranslation(&matWorld
				, m_vecMonster[idx]->vPos.x - GetScrollPos(0)
				, m_vecMonster[idx]->vPos.y - GetScrollPos(1)
				, m_vecMonster[idx]->vPos.z);

			// 객체가 각각 지니고 있는 자신의 크기로 자신의 이미지 크기를 결정
			matWorld = m_vecMonster[idx]->matScale * matWorld;

			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
				NULL, &D3DXVECTOR3(pTex->ImgInfo.Width*0.5f, pTex->ImgInfo.Height, 0), NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// 몬스터 선택표시
	for(int i = 0; i < WINSIZEY / (TILESIZEY * 0.5f) + 2; ++i)
	{
		for(int j = 0; j < WINSIZEX / TILESIZEX + 2; ++j)
		{
			int idx = (i + 
				GetScrollPos(1) / (TILESIZEY / 2))
				* TILECNTX + (j + 
				GetScrollPos(0) / (TILESIZEX));

			if(idx >= TILECNTX * TILECNTY || idx < 0)
				break;

			if (m_vecMonster[idx] == 0)
				continue;

			pTex = GET_SINGLE(CTextureMgr)->GetTexture(L"ObjSelEffect"
				, m_vecMonster[idx]->byDrawID);

			if(pTex == NULL)
				continue;

			D3DXMatrixTranslation(&matWorld
				, m_vecMonster[idx]->vPos.x - GetScrollPos(0)
				, m_vecMonster[idx]->vPos.y - GetScrollPos(1)
				, m_vecMonster[idx]->vPos.z);

			// 객체가 각각 지니고 있는 자신의 크기로 자신의 이미지 크기를 결정
			matWorld = m_vecMonster[idx]->matScale * matWorld;

			if (m_vecMonster[idx]->unitSelect == true)
			{
				GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
				GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
					NULL, &D3DXVECTOR3(pTex->ImgInfo.Width * 0.5f, pTex->ImgInfo.Height*0.9f, 0), NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
	GET_SINGLE(CDevice)->Render_End(m_hWnd);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	((CMiniView*)pMainFrm->m_MiniSplitte.GetPane(0, 0))->Invalidate(FALSE);
}

void CMFC_000View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = TILESIZEX * TILECNTX;
	sizeTotal.cy = TILESIZEY * TILECNTY * 0.5f;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	SetScrollSizes(MM_TEXT, sizeTotal);
	if(FAILED(GET_SINGLE(CDevice)->InitDevice(m_hWnd)))
		return;
	
	// 타일
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Tile/tile%d.png", L"Tile", TEXTYPE_MULTI, 38)))
		return;

	// 몬스터
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Stage/Monster/Monster0%d.png", L"Monster", TEXTYPE_MULTI, 3)))
		return;

	// 타일 선택
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Select/TileSelect%d.png", L"TileSelect", TEXTYPE_MULTI, 1)))
		return;

	// 몬스터 선택
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Select/SelectObj%d.png", L"ObjSelEffect", TEXTYPE_MULTI, 1)))
		return;

	// 타일 초기화
	InitTile();

	m_iSelDrawID = 0;
	m_iCurrentID = 1;
	m_fScrollval = 0.3f;

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	RECT rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);
	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);
	RECT rcMainView;
	GetClientRect(&rcMainView);
	float fRowFrm = rcWindow.bottom - rcMainView.bottom;
	float fColFrm = rcWindow.right - rcMainView.right;

	pMainFrm->SetWindowPos
		(NULL, 0, 0,
		WINSIZEX + fColFrm,
		WINSIZEY + fRowFrm, SWP_NOZORDER);
}

void CMFC_000View::InitTile()
{
	m_vecTile.resize(TILECNTX * TILECNTY);
	m_vecMonster.resize(TILECNTX * TILECNTY);

	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIdx = i * TILECNTX + j;
			m_vecTile[iIdx] = new TILE;
			m_vecTile[iIdx]->vPos
				= D3DXVECTOR3(
				j * TILESIZEX + (i % 2) * (TILESIZEX * 0.5f),
				i * (TILESIZEY * 0.5f), 0);
			m_vecTile[iIdx]->byDrawID = 2;
			m_vecTile[iIdx]->byOption = 0;
			m_vecTile[iIdx]->TileSelect = false;

			// Null 로 초기화
			m_vecMonster[iIdx] = NULL;
		}
	}
}


// CMFC_000View 인쇄


void CMFC_000View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_000View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_000View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_000View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC_000View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_000View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_000View 진단

#ifdef _DEBUG
void CMFC_000View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFC_000View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFC_000Doc* CMFC_000View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_000Doc)));
	return (CMFC_000Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_000View 메시지 처리기


void CMFC_000View::OnLButtonDown(UINT nFlags
	, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 몬스터 및 타일 생성(원클릭), 삭제(다시 클릭)
	D3DXVECTOR2 vMousePoint 
		= D3DXVECTOR2(point.x + GetScrollPos(0)
		, point.y + GetScrollPos(1));
	int iSize = m_vecTile.size();
	for(int i = 0; i < iSize; ++i)
	{
		// 충돌 체크, 마우스가 클릭한 지점의 타일을 체크
		if(CollisionMouseToTile(vMousePoint,
			m_vecTile[i]))
		{
			// 타일 페이지가 활성화 되어있는 경우
			if (m_iCurrentID == 0)
			{
				if (m_vecTile[i] != NULL)
				{
					// 타일과 바꿀 타일의 인덱스가 같지 않으면 
					if(m_vecTile[i]->byDrawID != m_iSelDrawID)
					{
						// 페이지 콤보에서 선택된 타일의 인덱스를 가져옴
						m_vecTile[i]->byDrawID = m_iSelDrawID;
					}
					else
					{
						// 타일과 바꿀 타일의 인덱스가 같으면
						// 타일 선택시 반전(true->false로 false->true로 바꿔줌)
						m_vecTile[i]->TileSelect = !m_vecTile[i]->TileSelect;
					}
				}

				// 이동 불가 영역 타일
				if (m_iSelDrawID >= 18 && m_iSelDrawID <= 22)
					m_vecTile[i]->byOption |= TILE_OPTION_DONT_MOVE;
				else
				{
					if (m_vecTile[i]->byOption & TILE_OPTION_DONT_MOVE)
						m_vecTile[i]->byOption ^= TILE_OPTION_DONT_MOVE;
				}

			}
			// 몬스터 페이지가 활성화 되어있는 경우
			else if (m_iCurrentID == 1)
			{
				// 몬스터가 있으면 선택
				if (m_vecMonster[i] != NULL)
				{
					m_vecMonster[i]->unitSelect = !m_vecMonster[i]->unitSelect;
				}
				// 몬스터가 없으면 생성
				else if (m_vecMonster[i] == NULL)
				{
					SAFE_DELETE<OBJINFO>(&m_vecMonster[i]);
					m_vecMonster[i] = new OBJINFO;
					m_vecMonster[i]->byDrawID = m_iSelDrawID;
					m_vecMonster[i]->unitSelect = false;
					D3DXMATRIX matWorld;
					D3DXMatrixIdentity(&matWorld);
					m_vecMonster[i]->matWorld = matWorld;
					m_vecMonster[i]->vPos = m_vecTile[i]->vPos;

					D3DXMatrixScaling(&m_vecMonster[i]->matScale, 0.3f, 0.3f, 0.3f);
				}
			}
				break;
		}
	}

	Invalidate(false);

	CScrollView::OnLButtonDown(nFlags, point);
}

bool CMFC_000View::CollisionMouseToTile(
	D3DXVECTOR2 vMousePos, const TILE* pTileInfo)
{

	// y = ax + b;
	// a?
	float fGrad = (TILESIZEY * 0.5f) / 
		(TILESIZEX * 0.5f);

	D3DXVECTOR2 vPoint[4];
	vPoint[0] = D3DXVECTOR2(
		pTileInfo->vPos.x, 
		pTileInfo->vPos.y + TILESIZEY * 0.5f);
	vPoint[1] = D3DXVECTOR2(
		pTileInfo->vPos.x + TILESIZEX * 0.5f, 
		pTileInfo->vPos.y);
	vPoint[2] = D3DXVECTOR2(
		pTileInfo->vPos.x, 
		pTileInfo->vPos.y - TILESIZEY * 0.5f);
	vPoint[3] = D3DXVECTOR2(
		pTileInfo->vPos.x - TILESIZEX * 0.5f, 
		pTileInfo->vPos.y);
	// y - ax = b;
	float fB[4] = 
	{
		vPoint[0].y - fGrad * vPoint[0].x,
		vPoint[1].y + fGrad * vPoint[1].x,
		vPoint[2].y - fGrad * vPoint[2].x,
		vPoint[3].y + fGrad * vPoint[3].x
	};

	if(
	vMousePos.y -  fGrad * vMousePos.x - fB[0] < 0 &&
	vMousePos.y - -fGrad * vMousePos.x - fB[1] < 0 &&
	vMousePos.y -  fGrad * vMousePos.x - fB[2] >= 0 &&
	vMousePos.y - -fGrad * vMousePos.x - fB[3] >= 0 )
	{
		return true;
	}
	return false;
}


void CMFC_000View::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GET_SINGLE(CTextureMgr)->DestroyInstance();
	GET_SINGLE(CDevice)->DestroyInstance();
	
	for(int i = 0; i < m_vecTile.size(); ++i)
	{
		if (m_vecTile[i] != NULL )
		{
			delete m_vecTile[i];
			m_vecTile[i] = 0;
		}
		if (m_vecMonster[i] != NULL)
		{
			SAFE_DELETE<OBJINFO>(&m_vecMonster[i]);
		}
	}
	m_vecTile.clear();
	m_vecMonster.clear();
}



void CMFC_000View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CMFC_000View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	D3DXVECTOR2 vMousePoint 
		= D3DXVECTOR2(m_MousePoint.x + GetScrollPos(0)
		, m_MousePoint.y + GetScrollPos(1));
	int iSize = m_vecTile.size();

	// 몬스터 페이지가 활성화 되어있는 경우
	if(m_iCurrentID == 1 )
	{
		switch(nChar)
		{
		case VK_DELETE:
			{
				for(int i = 0; i < iSize; ++i)
				{
					// 몬스터가 있으면 삭제
					if (m_vecMonster[i] != NULL && m_vecMonster[i]->unitSelect == true)
					{
						delete m_vecMonster[i];
						m_vecMonster[i] = NULL;
					}
				}
			}
		}
	}

	Invalidate(false);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFC_000View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_MousePoint = point;
	CScrollView::OnMouseMove(nFlags, point);
}
