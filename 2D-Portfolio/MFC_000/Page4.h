#pragma once


// CPage4 대화 상자입니다.

class CPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4();
	virtual ~CPage4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
