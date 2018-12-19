
// MFCDrawView.h : interface of the CMFCDrawView class
//

#pragma once


class CMFCDrawView : public CScrollView
{
protected: // create from serialization only
	CMFCDrawView();
	DECLARE_DYNCREATE(CMFCDrawView)

// Attributes
public:
	CMFCDrawDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	CPoint m_ptFirstPoint;			//used from sketcher
	CPoint m_ptSecondPoint;			//used from sketcher
};

#ifndef _DEBUG  // debug version in MFCDrawView.cpp
inline CMFCDrawDoc* CMFCDrawView::GetDocument() const
   { return reinterpret_cast<CMFCDrawDoc*>(m_pDocument); }
#endif

