#pragma once
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"

// MySheet

class MySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(MySheet)
public:
	CPage1 m_Page1;
	CPage2 m_Page2;
	CPage3 m_Page3;
public:
	MySheet();
	MySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	MySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~MySheet();

protected:
	DECLARE_MESSAGE_MAP()
};


