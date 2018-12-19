#include "stdafx.h"

#include "Color.h"
#include "MyList.h"
#include "Figure.h"
#include "TwoDimensionalFigure.h"
#include "RectangleFigure.h"


#include "EllipseFigure.h"


EllipseFigure::EllipseFigure()
	: TwoDimensionalFigure(),
	  RectangleFigure(),
	m_eDragMode(CREATE_ELLIPSE)
{
	
}

EllipseFigure::EllipseFigure(const Utility::Color & color, const CPoint& ptTopLeft, BOOL bFilled)
	: 
	  RectangleFigure(color,ptTopLeft,bFilled),
	TwoDimensionalFigure(color,bFilled),
	m_eDragMode(CREATE_ELLIPSE)
{

}


//copy constructor
EllipseFigure::EllipseFigure(const EllipseFigure & ellipse)
	: TwoDimensionalFigure(),
	RectangleFigure()
{
	//empty
}

Figure * EllipseFigure::Copy() const
{
	Figure* myFigure;

	myFigure = new EllipseFigure(*this);
	return myFigure;
}

HCURSOR EllipseFigure::GetCursor() const
{
	HCURSOR myCursor;


	switch (m_eDragMode)
	{
	case CREATE_ELLIPSE:
		myCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
		break;
	case MODIFY_LEFT:
	case MODIFY_RIGHT:
		myCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		break;
	case MODIFY_TOP:
	case MODIFY_BOTTOM:
		myCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
		break;

	case MOVE_ELLIPSE:
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		break;

	}


	return myCursor;


	
}

BOOL EllipseFigure::Click(const CPoint & ptMouse)
{
	int xCenter = (m_ptTopLeft.x + m_ptBottomRight.x) / 2;
	int yCenter = (m_ptTopLeft.y + m_ptBottomRight.y) / 2;
	// Has the user clicked at the leftmost point?
	CRect rcLeft(m_ptTopLeft.x - (SQUARE_SIDE / 2),
		yCenter - (SQUARE_SIDE / 2),
		m_ptTopLeft.x + (SQUARE_SIDE / 2),
		yCenter + (SQUARE_SIDE / 2));
	if (rcLeft.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_LEFT;
		return TRUE;
	}
	// Or the rightmost point?
	CRect rcRight(m_ptBottomRight.x - (SQUARE_SIDE / 2),
		yCenter - (SQUARE_SIDE / 2),
		m_ptBottomRight.x + (SQUARE_SIDE / 2),
		yCenter + (SQUARE_SIDE / 2));
	if (rcRight.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_RIGHT;
		return TRUE;
	}
	// Or the topmost point?
	CRect rcTop(xCenter - (SQUARE_SIDE / 2),
		m_ptTopLeft.y - (SQUARE_SIDE / 2),
		xCenter + (SQUARE_SIDE / 2),
		m_ptTopLeft.y + (SQUARE_SIDE / 2));
	if (rcTop.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_TOP;
		return TRUE;
	}
	// Or the bottommost point?
	CRect rcBottom(xCenter - (SQUARE_SIDE / 2),
		m_ptBottomRight.y - (SQUARE_SIDE / 2),
		xCenter + (SQUARE_SIDE / 2),
		m_ptBottomRight.y + (SQUARE_SIDE / 2));
	if (rcBottom.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_BOTTOM;
		return TRUE;
	}
	CRgn rgArea;
	rgArea.CreateEllipticRgn(m_ptTopLeft.x, m_ptTopLeft.y,
		m_ptBottomRight.x,
		m_ptBottomRight.y);
	// Is the ellipse filled?
	if (IsFilled())
	{
		m_eDragMode = MOVE_ELLIPSE;
		return rgArea.PtInRegion(ptMouse);
	}
	// Or unfilled?
	else
	{
		int xMin = min(m_ptTopLeft.x, m_ptBottomRight.x);
		int xMax = max(m_ptTopLeft.x, m_ptBottomRight.x);
		int yMin = min(m_ptTopLeft.y, m_ptBottomRight.y);
		int yMax = max(m_ptTopLeft.y, m_ptBottomRight.y);
		CRgn rgSmallArea, rgLargeArea;
		rgSmallArea.CreateEllipticRgn(xMin + (SQUARE_SIDE / 2),
			yMin + (SQUARE_SIDE / 2),
			xMax - (SQUARE_SIDE / 2),
			yMax - (SQUARE_SIDE / 2));
		rgLargeArea.CreateEllipticRgn(xMin - (SQUARE_SIDE / 2),
			yMin - (SQUARE_SIDE / 2),
			xMax + (SQUARE_SIDE / 2),
			yMax + (SQUARE_SIDE / 2));
		m_eDragMode = MOVE_ELLIPSE;
		return rgLargeArea.PtInRegion(ptMouse) && !rgSmallArea.PtInRegion(ptMouse);
	}



}

void EllipseFigure::MoveOrModify(const CSize & szDistance)
{
	switch (m_eDragMode)
	{
	case CREATE_ELLIPSE:
		m_ptTopLeft += szDistance;
		break;
	case MODIFY_LEFT:
		m_ptTopLeft.x += szDistance.cx;
		break;
	case MODIFY_RIGHT:
		m_ptBottomRight.x += szDistance.cx;
		break;
	case MODIFY_TOP:
		m_ptTopLeft.y += szDistance.cy;
		break;
	case MODIFY_BOTTOM:
		m_ptBottomRight.y += szDistance.cy;
		break;

	case MOVE_ELLIPSE:
		Move(szDistance);
		break;
	}
}

void EllipseFigure::Draw(CDC * pDC) const
{

	if (IsFilled())
	{

		CPen pen(PS_SOLID, 0, (COLORREF)GetColor());

		CPen* pOldPen = pDC->SelectObject(&pen);

		//fill color is the same as the edge
		CBrush brush((COLORREF)GetColor());
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->Ellipse(m_ptTopLeft.x, m_ptTopLeft.y, m_ptBottomRight.x, m_ptBottomRight.y);
	

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);



	}
	else
	{

		CPen pen(PS_SOLID, 0, (COLORREF)GetColor());

		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Ellipse(m_ptTopLeft.x, m_ptTopLeft.y, m_ptBottomRight.x, m_ptBottomRight.y);


		pDC->SelectObject(pOldPen);
	}
	if (IsMarked())
	{
		//do TODO:
	}
}



