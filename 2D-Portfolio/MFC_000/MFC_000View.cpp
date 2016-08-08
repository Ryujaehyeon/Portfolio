
// MFC_000View.cpp : CMFC_000View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CMFC_000View ����/�Ҹ�

CMFC_000View::CMFC_000View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_000View::~CMFC_000View()
{
}

BOOL CMFC_000View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMFC_000View �׸���

void CMFC_000View::OnDraw(CDC* /*pDC*/)
{
	CMFC_000Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	GET_SINGLE(CDevice)->Render_Begin();

	D3DXMATRIX matWorld, matScale;
	D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);
	const TEXINFO* pTex;

	// Ÿ��
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

	// Ÿ�� ����
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
	

	// ����
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

			// ��ü�� ���� ���ϰ� �ִ� �ڽ��� ũ��� �ڽ��� �̹��� ũ�⸦ ����
			matWorld = m_vecMonster[idx]->matScale * matWorld;

			GET_SINGLE(CDevice)->GetSprite()->SetTransform(&matWorld);
			GET_SINGLE(CDevice)->GetSprite()->Draw(pTex->pTexture,
				NULL, &D3DXVECTOR3(pTex->ImgInfo.Width*0.5f, pTex->ImgInfo.Height, 0), NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// ���� ����ǥ��
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

			// ��ü�� ���� ���ϰ� �ִ� �ڽ��� ũ��� �ڽ��� �̹��� ũ�⸦ ����
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
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	SetScrollSizes(MM_TEXT, sizeTotal);
	if(FAILED(GET_SINGLE(CDevice)->InitDevice(m_hWnd)))
		return;
	
	// Ÿ��
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Tile/tile%d.png", L"Tile", TEXTYPE_MULTI, 38)))
		return;

	// ����
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Stage/Monster/Monster0%d.png", L"Monster", TEXTYPE_MULTI, 3)))
		return;

	// Ÿ�� ����
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Select/TileSelect%d.png", L"TileSelect", TEXTYPE_MULTI, 1)))
		return;

	// ���� ����
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Select/SelectObj%d.png", L"ObjSelEffect", TEXTYPE_MULTI, 1)))
		return;

	// Ÿ�� �ʱ�ȭ
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

			// Null �� �ʱ�ȭ
			m_vecMonster[iIdx] = NULL;
		}
	}
}


// CMFC_000View �μ�


void CMFC_000View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_000View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_000View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_000View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMFC_000View ����

#ifdef _DEBUG
void CMFC_000View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFC_000View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFC_000Doc* CMFC_000View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_000Doc)));
	return (CMFC_000Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_000View �޽��� ó����


void CMFC_000View::OnLButtonDown(UINT nFlags
	, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// ���� �� Ÿ�� ����(��Ŭ��), ����(�ٽ� Ŭ��)
	D3DXVECTOR2 vMousePoint 
		= D3DXVECTOR2(point.x + GetScrollPos(0)
		, point.y + GetScrollPos(1));
	int iSize = m_vecTile.size();
	for(int i = 0; i < iSize; ++i)
	{
		// �浹 üũ, ���콺�� Ŭ���� ������ Ÿ���� üũ
		if(CollisionMouseToTile(vMousePoint,
			m_vecTile[i]))
		{
			// Ÿ�� �������� Ȱ��ȭ �Ǿ��ִ� ���
			if (m_iCurrentID == 0)
			{
				if (m_vecTile[i] != NULL)
				{
					// Ÿ�ϰ� �ٲ� Ÿ���� �ε����� ���� ������ 
					if(m_vecTile[i]->byDrawID != m_iSelDrawID)
					{
						// ������ �޺����� ���õ� Ÿ���� �ε����� ������
						m_vecTile[i]->byDrawID = m_iSelDrawID;
					}
					else
					{
						// Ÿ�ϰ� �ٲ� Ÿ���� �ε����� ������
						// Ÿ�� ���ý� ����(true->false�� false->true�� �ٲ���)
						m_vecTile[i]->TileSelect = !m_vecTile[i]->TileSelect;
					}
				}

				// �̵� �Ұ� ���� Ÿ��
				if (m_iSelDrawID >= 18 && m_iSelDrawID <= 22)
					m_vecTile[i]->byOption |= TILE_OPTION_DONT_MOVE;
				else
				{
					if (m_vecTile[i]->byOption & TILE_OPTION_DONT_MOVE)
						m_vecTile[i]->byOption ^= TILE_OPTION_DONT_MOVE;
				}

			}
			// ���� �������� Ȱ��ȭ �Ǿ��ִ� ���
			else if (m_iCurrentID == 1)
			{
				// ���Ͱ� ������ ����
				if (m_vecMonster[i] != NULL)
				{
					m_vecMonster[i]->unitSelect = !m_vecMonster[i]->unitSelect;
				}
				// ���Ͱ� ������ ����
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CMFC_000View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	D3DXVECTOR2 vMousePoint 
		= D3DXVECTOR2(m_MousePoint.x + GetScrollPos(0)
		, m_MousePoint.y + GetScrollPos(1));
	int iSize = m_vecTile.size();

	// ���� �������� Ȱ��ȭ �Ǿ��ִ� ���
	if(m_iCurrentID == 1 )
	{
		switch(nChar)
		{
		case VK_DELETE:
			{
				for(int i = 0; i < iSize; ++i)
				{
					// ���Ͱ� ������ ����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_MousePoint = point;
	CScrollView::OnMouseMove(nFlags, point);
}
