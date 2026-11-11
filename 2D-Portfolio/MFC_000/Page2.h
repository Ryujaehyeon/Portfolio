#pragma once

#include "Defines.h"

// 타일 파일(Map.dat)을 읽어 _rTiles 뒤에 붙인다. 반환: 읽은 타일 수, 파일을 못 열면 -1.
// 「타일 불러오기」 버튼과 스모크 모드(-smoke)가 같이 쓴다.
int ReadTileFile(const TCHAR* const _pPath, vector<TILE*>& _rTiles);


// CPage2 대화 상자입니다.

class CPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2();
	virtual ~CPage2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
