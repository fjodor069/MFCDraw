
// MFCDrawView.cpp : implementation of the CMFCDrawView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCDraw.h"
#endif


#include "Caret.h"
#include "Color.h"
#include "Font.h"
#include "MyList.h"



#include "Figure.h"
#include "TwoDimensionalFigure.h"
#include "LineFigure.h"
#include "ArrowFigure.h"
#include "RectangleFigure.h"
#include "EllipseFigure.h"

#include "TextFigure.h"
#include "FigureFileManager.h"


#include "MFCDrawDoc.h"
#include "MFCDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDrawView

IMPLEMENT_DYNCREATE(CMFCDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCDrawView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCDrawView construction/destruction

CMFCDrawView::CMFCDrawView()
{
	// TODO: add construction code here

}

CMFCDrawView::~CMFCDrawView()
{
}

BOOL CMFCDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CMFCDrawView drawing
//do the actual drawing of this window...
void CMFCDrawView::OnDraw(CDC* pDC)
{
	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const FigurePointerList* pFigurePtrList = pDoc->GetFigurePtrList();

	for (POSITION position = pFigurePtrList->GetHeadPosition();
		position != NULL; pFigurePtrList->GetNext(position))
	{
		Figure* pFigure = pFigurePtrList->GetAt(position);
		CRect rcFigure = pFigure->GetArea();
		pFigure->Draw(pDC);
	}

	//looopt vast hierop
	/*const RectangleFigure* pInsideRectangle = pDoc->GetInsideRectangle();
	if (pInsideRectangle != NULL)
	{
		pInsideRectangle->Draw(pDC);
	}*/




	

	
}

// set the size of the view you can scroll
// and the mapping mode
//
void CMFCDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CMFCDrawView printing

BOOL CMFCDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCDrawView diagnostics

#ifdef _DEBUG
void CMFCDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCDrawDoc* CMFCDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDrawDoc)));
	return (CMFCDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDrawView message handlers


void CMFCDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CClientDC dc(this);
	OnPrepareDC(&dc);

	dc.DPtoLP(&point);


	BOOL bControlKeyDown = (nFlags & MK_CONTROL);

	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->MouseDown(point,bControlKeyDown, &dc);


}

//
// called after creation but before it is shown
//
int CMFCDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;


	CSize szTotal(TOTAL_WIDTH, TOTAL_HEIGHT);
	SetScrollSizes(MM_HIMETRIC, szTotal);

	return 0;
}


void CMFCDrawView::OnSetFocus(CWnd* pOldWnd)
{

	CScrollView::OnSetFocus(pOldWnd);

	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	Utility::Caret* pCaret = pDoc->GetCaret();
	pCaret->OnSetFocus(this);
	

}


void CMFCDrawView::OnKillFocus(CWnd* pNewWnd)
{
	CScrollView::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Utility::Caret* pCaret = pDoc->GetCaret();
	pCaret->OnKillFocus();

}


BOOL CMFCDrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CMFCDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->MouseDrag(point);
}


void CMFCDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->MouseUp();


}


void CMFCDrawView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->DoubleClick(point);


}


void CMFCDrawView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}



// process all keys
// first send it to the document class,
// if not used there, returns false and use for scrolling the view
//
void CMFCDrawView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);

	CMFCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	switch (nChar)
	{
		case VK_HOME:
			if (!pDoc->KeyDown(VK_HOME, &dc))
			{
				OnVScroll(SB_TOP, 0, NULL);
				OnHScroll(SB_LEFT, 0, NULL);
			}
			break;
			// ...

			
		case VK_END:
			if (!pDoc->KeyDown(VK_HOME, &dc))
			{
				OnVScroll(SB_TOP, 0, NULL);
				OnHScroll(SB_LEFT, 0, NULL);
			}
			break;





	}


}


//
// added with override
// 
void CMFCDrawView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class

	if (lHint != NULL)
	{
		CRect rcClip = *(CRect*)lHint;
		int cxMargin = (int)(0.05 * rcClip.Width());
		int cyMargin = (int)(0.05 * rcClip.Height());
		rcClip.left -= cxMargin;
		rcClip.right += cxMargin;
		rcClip.top -= cyMargin;
		rcClip.bottom += cyMargin;
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.LPtoDP(rcClip);
		InvalidateRect(rcClip);
	}
	else
	{
		Invalidate();
	}
	UpdateWindow();



}
