
// MFC_000View.h : CMFC_000View 클래스의 인터페이스
//

#pragma once
#include "Defines.h"
#include "MFC_000Doc.h"

class CMFC_000View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CMFC_000View();
	DECLARE_DYNCREATE(CMFC_000View)

// 특성입니다.
public:
	CMFC_000Doc* GetDocument() const;

// 작업입니다.
public:
	vector<TILE*> m_vecTile;
	vector<OBJINFO*> m_vecMonster;
	int m_iSelDrawID;
	int m_iCurrentID;
	float m_fScrollval;
	CPoint m_MousePoint;
	void InitTile(); /* 타일을 초기화하는 함수 */
	bool CollisionMouseToTile(D3DXVECTOR2 vMousePos,
		const TILE* pTileInfo);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFC_000View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFC_000View.cpp의 디버그 버전
inline CMFC_000Doc* CMFC_000View::GetDocument() const
   { return reinterpret_cast<CMFC_000Doc*>(m_pDocument); }
#endif

