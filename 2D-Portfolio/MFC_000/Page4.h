#pragma once


// CPage4 ��ȭ �����Դϴ�.

class CPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4();
	virtual ~CPage4();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
