// Page3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "Page3.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFC_000View.h"


// CPage3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage3, CPropertyPage)

CPage3::CPage3()
	: CPropertyPage(CPage3::IDD)
{

}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrSlider);
}


BEGIN_MESSAGE_MAP(CPage3, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPage3::OnCbnSelchangeCombo1)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CPage3::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPage3::OnNMCustomdrawSlider1)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CPage3 메시지 처리기입니다.

BOOL CPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateData(TRUE);

	// 슬라이더 바
	// 사용영역 값 설정한다.
	m_ctrSlider.SetRange(0, 300);

	// 최소 값 설정.
	//m_ctrSlider.SetRangeMin(-300);
	// 최대 값을 설정.
	//m_ctrSlider.SetRangeMax(300);


	// 위치 설정.
	m_ctrSlider.SetPos(30);

	// 눈금 간격을 설정한다.
	// 속성의 Tick Marks와 Auto Ticks가 True로 되어 있어야 한다.
	m_ctrSlider.SetTicFreq(10);

	// 키보드 커서키로 슬라이더를 움직일때의 증가 크기를 설정
	m_ctrSlider.SetLineSize(1);

	// 키보드의 PgUp, PgDn키를 누르거나 마우스로 슬라이더의 몸동을 클릭시 움직일 크기 
	m_ctrSlider.SetPageSize(5);

	// 슬라이더의 특정 영역 선택.
	// 속성의 Enable Selection Range가 True로 되어 있어야 한다.
	//m_ctrSlider.SetSelection(-100, 100);

	// 선택영역 지운기.
	//m_ctrSlider.ClearSel();

	//4. 슬라이더가 이동 되었을때.
	//	- 슬라이더 컨트롤의 이벤트 NM_RELEASEDCAPTURE를 사용한다.
	//	- 마우스로 슬라이더의 이동이 끝난 후 발생한다.

	//	int nPos = m_ctrSlider.GetPos();   // 현재 슬라이더의 위치를 알아낸다.

	// 콤보박스
	TCHAR szTmp[128] = L"";
	for(int i = 0; i < 3; ++i)
	{
		wsprintf(szTmp, L"Monster0%d", i);
		m_Combo.AddString(szTmp);
	}
	m_Combo.SetCurSel(0);

	RECT rcPicture;

	m_Picture.GetWindowRect(&rcPicture);
	SetRect(&rcPicture, 0, 0, 150, 150);

	for(int i = 0; i < 3; ++i)
	{
		wsprintf(szTmp, L"../Resource/Texture/Stage/Monster/Monster0%d.bmp", i);
		m_BitMap[i] = (HBITMAP)LoadImage(NULL, szTmp,
			IMAGE_BITMAP, rcPicture.right, rcPicture.bottom, 
			LR_LOADFROMFILE);
	}

	m_Picture.SetBitmap(m_BitMap[0]);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPage3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//5. 슬라이더가 움직일때 마다 값을 알고자 한다면.
	//	- 대화상자의 메시지 WM_HSCROOL를 사용한다. 가로 슬라이더라면 WM_HSCROOL을 사용한다.
	vector<OBJINFO*>* pMonster = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecMonster;

	if(pScrollBar)
	{
		// 어떤 슬라이더인지 검사
		if(pScrollBar == (CScrollBar*)&m_ctrSlider)
		{
			// 스케일 값은 소수값을 받지만 슬라이더 값 소수점 단위로 
			// 설정할 수 없기 때문에 단위를 높인 후 그 값을 나눈후 그 값을 이용 
			// 슬라이더의 위치를 검사한다.
			float nPos = m_ctrSlider.GetPos() * 0.01f;
			((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_fScrollval = nPos;
			for (vector<OBJINFO*>::iterator iter = pMonster->begin();
				iter != pMonster->end(); ++iter)
			{
				if ((*iter) != NULL && (*iter)->unitSelect == true )
				{
					// 7/21 오후 8시 30분
					D3DXMatrixScaling(&(*iter)->matScale, nPos, nPos, nPos);
				}
			}
		}
	}
	else
	{
		// CScrollView를 상속받은 뷰의 경우 프래임의 스크롤롤 동작시 pScrollBar이 NULL된다.
	}

	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPage3::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_Combo.GetCurSel();
	m_Picture.SetBitmap(m_BitMap[iIndex]);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
}


void CPage3::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


BOOL CPage3::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int iIndex = m_Combo.GetCurSel();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
	pMainFrm->m_pMainView->m_iCurrentID = 1;

	return CPropertyPage::OnSetActive();
}


void CPage3::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	vector<OBJINFO*>* pMonster = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecMonster;

	for(vector<OBJINFO*>::iterator iter = pMonster->begin();
		iter != pMonster->end(); ++iter)
	{
		if ( (*iter) != NULL)
		{
			delete (*iter);
			(*iter) = NULL;
		}
	}

	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}


void CPage3::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

