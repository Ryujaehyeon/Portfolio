// Page1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "Page1.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFC_000View.h"

// CPage1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage1, CPropertyPage)

CPage1::CPage1()
	: CPropertyPage(CPage1::IDD)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPage1::OnCbnSelchangeCombo1)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CPage1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage1::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPage1 메시지 처리기입니다.


BOOL CPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	TCHAR szTmp[128] = L"";
	for(int i = 0; i < 38; ++i)
	{
		wsprintf(szTmp, L"Tile%d", i);
		m_Combo.AddString(szTmp);
	}
	m_Combo.SetCurSel(0);

	RECT rcPicture;

	m_Picture.GetWindowRect(&rcPicture);
	SetRect(&rcPicture, 0, 0, 150, 150);

	for(int i = 0; i < 38; ++i)
	{
		wsprintf(szTmp, L"../Resource/Texture/TileBmp/Tile%d.bmp", i);
		m_BitMap[i] = (HBITMAP)LoadImage(NULL, szTmp,
			IMAGE_BITMAP, rcPicture.right, rcPicture.bottom, 
			LR_LOADFROMFILE);
	}

	m_Picture.SetBitmap(m_BitMap[0]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage1::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_Combo.GetCurSel();
	m_Picture.SetBitmap(m_BitMap[iIndex]);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
}



void CPage1::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CPage1::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int iIndex = m_Combo.GetCurSel();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
	pMainFrm->m_pMainView->m_iCurrentID = 0;
	return CPropertyPage::OnSetActive();
}


void CPage1::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 타일 초기화
	vector<TILE*> pTile = 
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecTile;

	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIdx = i * TILECNTX + j;
			pTile[iIdx]->byDrawID = 2;
			pTile[iIdx]->byOption = 0;
		}
	}
	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}


void CPage1::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택된 타일로 전부 교체
	vector<TILE*> pTile = 
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecTile;
	int iIndex = m_Combo.GetCurSel();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIdx = i * TILECNTX + j;
			pTile[iIdx]->byDrawID = iIndex;
			pTile[iIdx]->byOption = 0;
		}
	}
	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}


void CPage1::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	vector<TILE*> pTile = 
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecTile;

	int iIndex = m_Combo.GetCurSel();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIdx = i * TILECNTX + j;
			if (pTile[iIdx]->TileSelect == true)
			{
				pTile[iIdx]->byDrawID = iIndex;
				pTile[iIdx]->byOption = 0;
				pTile[iIdx]->TileSelect = false;
			}
		}
	}

	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}
