// Page4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "Page4.h"
#include "afxdialogex.h"


// CPage4 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage4, CPropertyPage)

CPage4::CPage4()
	: CPropertyPage(CPage4::IDD)
{

}

CPage4::~CPage4()
{
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage4, CPropertyPage)
END_MESSAGE_MAP()


// CPage4 메시지 처리기입니다.
