#pragma once


// CPage3 대화 상자입니다.

class CPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3();
	virtual ~CPage3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
