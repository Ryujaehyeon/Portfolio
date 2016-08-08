// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "MFC_000View.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.
void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pMySheet = new MySheet;
	m_pMySheet->Create(this, WS_VISIBLE | WS_CHILD);
	m_pMySheet->MoveWindow(0, 0, 300, 600, FALSE);
}


void CMyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pMySheet;
	m_pMySheet = NULL;
}
