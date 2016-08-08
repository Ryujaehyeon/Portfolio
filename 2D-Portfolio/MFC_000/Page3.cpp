// Page3.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "Page3.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFC_000View.h"


// CPage3 ��ȭ �����Դϴ�.

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

// CPage3 �޽��� ó�����Դϴ�.

BOOL CPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	UpdateData(TRUE);

	// �����̴� ��
	// ��뿵�� �� �����Ѵ�.
	m_ctrSlider.SetRange(0, 300);

	// �ּ� �� ����.
	//m_ctrSlider.SetRangeMin(-300);
	// �ִ� ���� ����.
	//m_ctrSlider.SetRangeMax(300);


	// ��ġ ����.
	m_ctrSlider.SetPos(30);

	// ���� ������ �����Ѵ�.
	// �Ӽ��� Tick Marks�� Auto Ticks�� True�� �Ǿ� �־�� �Ѵ�.
	m_ctrSlider.SetTicFreq(10);

	// Ű���� Ŀ��Ű�� �����̴��� �����϶��� ���� ũ�⸦ ����
	m_ctrSlider.SetLineSize(1);

	// Ű������ PgUp, PgDnŰ�� �����ų� ���콺�� �����̴��� ������ Ŭ���� ������ ũ�� 
	m_ctrSlider.SetPageSize(5);

	// �����̴��� Ư�� ���� ����.
	// �Ӽ��� Enable Selection Range�� True�� �Ǿ� �־�� �Ѵ�.
	//m_ctrSlider.SetSelection(-100, 100);

	// ���ÿ��� �����.
	//m_ctrSlider.ClearSel();

	//4. �����̴��� �̵� �Ǿ�����.
	//	- �����̴� ��Ʈ���� �̺�Ʈ NM_RELEASEDCAPTURE�� ����Ѵ�.
	//	- ���콺�� �����̴��� �̵��� ���� �� �߻��Ѵ�.

	//	int nPos = m_ctrSlider.GetPos();   // ���� �����̴��� ��ġ�� �˾Ƴ���.

	// �޺��ڽ�
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPage3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//5. �����̴��� �����϶� ���� ���� �˰��� �Ѵٸ�.
	//	- ��ȭ������ �޽��� WM_HSCROOL�� ����Ѵ�. ���� �����̴���� WM_HSCROOL�� ����Ѵ�.
	vector<OBJINFO*>* pMonster = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecMonster;

	if(pScrollBar)
	{
		// � �����̴����� �˻�
		if(pScrollBar == (CScrollBar*)&m_ctrSlider)
		{
			// ������ ���� �Ҽ����� ������ �����̴� �� �Ҽ��� ������ 
			// ������ �� ���� ������ ������ ���� �� �� ���� ������ �� ���� �̿� 
			// �����̴��� ��ġ�� �˻��Ѵ�.
			float nPos = m_ctrSlider.GetPos() * 0.01f;
			((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_fScrollval = nPos;
			for (vector<OBJINFO*>::iterator iter = pMonster->begin();
				iter != pMonster->end(); ++iter)
			{
				if ((*iter) != NULL && (*iter)->unitSelect == true )
				{
					// 7/21 ���� 8�� 30��
					D3DXMatrixScaling(&(*iter)->matScale, nPos, nPos, nPos);
				}
			}
		}
	}
	else
	{
		// CScrollView�� ��ӹ��� ���� ��� �������� ��ũ�ѷ� ���۽� pScrollBar�� NULL�ȴ�.
	}

	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPage3::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = m_Combo.GetCurSel();
	m_Picture.SetBitmap(m_BitMap[iIndex]);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
}


void CPage3::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

}


BOOL CPage3::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	int iIndex = m_Combo.GetCurSel();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_pMainView->m_iSelDrawID = iIndex;
	pMainFrm->m_pMainView->m_iCurrentID = 1;

	return CPropertyPage::OnSetActive();
}


void CPage3::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

