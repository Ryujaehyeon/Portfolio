#pragma once


// CPage3 ��ȭ �����Դϴ�.

class CPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3();
	virtual ~CPage3();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_Combo;
	CSliderCtrl m_ctrSlider;
	CStatic m_Picture;

	HBITMAP m_BitMap[38];
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnSetActive();
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
