
// MFC_000View.h : CMFC_000View Ŭ������ �������̽�
//

#pragma once
#include "Defines.h"
#include "MFC_000Doc.h"

class CMFC_000View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMFC_000View();
	DECLARE_DYNCREATE(CMFC_000View)

// Ư���Դϴ�.
public:
	CMFC_000Doc* GetDocument() const;

// �۾��Դϴ�.
public:
	vector<TILE*> m_vecTile;
	vector<OBJINFO*> m_vecMonster;
	int m_iSelDrawID;
	int m_iCurrentID;
	float m_fScrollval;
	CPoint m_MousePoint;
	void InitTile(); /* Ÿ���� �ʱ�ȭ�ϴ� �Լ� */
	bool CollisionMouseToTile(D3DXVECTOR2 vMousePos,
		const TILE* pTileInfo);

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC_000View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // MFC_000View.cpp�� ����� ����
inline CMFC_000Doc* CMFC_000View::GetDocument() const
   { return reinterpret_cast<CMFC_000Doc*>(m_pDocument); }
#endif

