#include "stdafx.h"

#include "Color.h"
#include "MyList.h"
#include "Figure.h"
#include "TwoDimensionalFigure.h"

#include "RectangleFigure.h"


RectangleFigure::RectangleFigure()
	: TwoDimensionalFigure(),
	  m_eDragMode(CREATE_RECTANGLE)
{
	//empty
}

RectangleFigure::RectangleFigure(const Utility::Color & color, const CPoint& ptTopLeft, BOOL bFilled)
	: TwoDimensionalFigure(color,bFilled),
	m_eDragMode(CREATE_RECTANGLE)
	
{
	//initialise the starting point of this figure
	m_ptTopLeft = ptTopLeft;
	m_ptBottomRight = ptTopLeft;
}

//copy constructor
RectangleFigure::RectangleFigure(const RectangleFigure & rectangle)
{
	m_ptTopLeft = rectangle.m_ptTopLeft;
	m_ptBottomRight = rectangle.m_ptBottomRight;

}

Figure * RectangleFigure::Copy() const
{
	// TODO: copy an item
	Figure* myFigure;

	myFigure = new RectangleFigure(*this);
	return myFigure;
}

void RectangleFigure::Serialize(CArchive & archive)
{
	TwoDimensionalFigure::Serialize(archive);

	if (archive.IsStoring())
	{
		archive << m_ptTopLeft;
		archive << m_ptBottomRight;

	}
	if (archive.IsLoading())
	{
		archive >> m_ptTopLeft;
		archive >> m_ptBottomRight;

	}
	

}

HCURSOR RectangleFigure::GetCursor() const
{
	HCURSOR myCursor;


	switch (m_eDragMode)
	{
		case MODIFY_TOPLEFT:
		case MODIFY_BOTTOMRIGHT:
			myCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
			break;
		case MODIFY_TOPRIGHT:
		case MODIFY_BOTTOMLEFT:
			myCursor =  AfxGetApp()->LoadStandardCursor(IDC_SIZENESW);
			break;

		case MOVE_RECTANGLE:
			return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
			break;

	}


	return myCursor;
}


//determine which corner the user clicks and return as dragmode
//
BOOL RectangleFigure::Click(const CPoint & ptMouse)
{
	// Did the user click on the top left corner?
	CRect rcTopLeft(m_ptTopLeft.x - (SQUARE_SIDE / 2),
		m_ptTopLeft.y - (SQUARE_SIDE / 2),
		m_ptTopLeft.x + (SQUARE_SIDE / 2),
		m_ptTopLeft.y + (SQUARE_SIDE / 2));
	if (rcTopLeft.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_TOPLEFT;
		return TRUE;
	}
	// Or the top right corner?
	CRect rcTopRight(m_ptBottomRight.x - (SQUARE_SIDE / 2),
		m_ptTopLeft.y - (SQUARE_SIDE / 2),

		m_ptBottomRight.x + (SQUARE_SIDE / 2),
		m_ptTopLeft.y + (SQUARE_SIDE / 2));
	if (rcTopRight.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_TOPRIGHT;
		return TRUE;
	}
	// Or the bottom right corner?
	CRect rcBottomRight(m_ptBottomRight.x - (SQUARE_SIDE / 2),
		m_ptBottomRight.y - (SQUARE_SIDE / 2),
		m_ptBottomRight.x + (SQUARE_SIDE / 2),
		m_ptBottomRight.y + (SQUARE_SIDE / 2));
	if (rcBottomRight.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_BOTTOMRIGHT;
		return TRUE;
	}
	// Or the bottom left corner?
	CRect rcBottomLeft(m_ptTopLeft.x - (SQUARE_SIDE / 2),
		m_ptBottomRight.y - (SQUARE_SIDE / 2),
		m_ptTopLeft.x + (SQUARE_SIDE / 2),
		m_ptBottomRight.y + (SQUARE_SIDE / 2));
	if (rcBottomLeft.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_BOTTOMLEFT;
		return TRUE;
	}
	CRect rcArea(m_ptTopLeft, m_ptBottomRight);
	rcArea.NormalizeRect();
	// Is the rectangle filled?
	if (IsFilled())
	{
		m_eDragMode = MOVE_RECTANGLE;
		return rcArea.PtInRect(ptMouse);
	}
	// Or is it unfilled?
	else
	{
		CSize szMargin((SQUARE_SIDE / 2), (SQUARE_SIDE / 2));
		CRect rcSmallArea(rcArea.TopLeft() + szMargin,
			rcArea.BottomRight() - szMargin);
		CRect rcLargeArea(rcArea.TopLeft() - szMargin,
			rcArea.BottomRight() + szMargin);
		m_eDragMode = MOVE_RECTANGLE;
		return rcLargeArea.PtInRect(ptMouse) &&
			!rcSmallArea.PtInRect(ptMouse);
	}



	
}

BOOL RectangleFigure::DoubleClick(const CPoint & ptMouse)
{
	// TODO : fill or unfill the rectangle
	
	return 0;
}

BOOL RectangleFigure::Inside(const CRect & rcInside) const
{
	// TODO : return true when top left and bottom right corners are enclosed 
	return 0;
}

void RectangleFigure::MoveOrModify(const CSize & szDistance)
{
	switch (m_eDragMode)
	{
	case CREATE_RECTANGLE:
		m_ptBottomRight += szDistance;
		break;
	case MODIFY_TOPLEFT:
		m_ptTopLeft += szDistance;
		
		break;
	case MODIFY_TOPRIGHT:
		m_ptBottomRight.x += szDistance.cx;
		m_ptTopLeft.y += szDistance.cy;
		break;
	case MODIFY_BOTTOMRIGHT:
		m_ptBottomRight += szDistance;
		break;
	case MODIFY_BOTTOMLEFT:
		m_ptTopLeft.x += szDistance.cx;
		m_ptBottomRight.y += szDistance.cy;
		break;
	case MOVE_RECTANGLE:
		Move(szDistance);
		break;
	}
}

void RectangleFigure::Move(const CSize & szDistance)
{
	//Invalidate();
	m_ptTopLeft += szDistance;
	m_ptBottomRight += szDistance; 
//	Invalidate();
	
}

void RectangleFigure::Draw(CDC * pDC) const
{
	

	if (IsFilled())
	{
		//fillrectangle
		
		CPen pen(PS_SOLID, 0, (COLORREF)GetColor());

		CPen* pOldPen = pDC->SelectObject(&pen);

		//fill color is the same as the edge
		CBrush brush((COLORREF)GetColor());
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		//pDC->Rectangle(m_ptTopLeft.x, m_ptTopLeft.y, m_ptBottomRight.x, m_ptBottomRight.y);
	
		pDC->FillRect(CRect(m_ptTopLeft, m_ptBottomRight), &brush);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);


	}
	else
	{
		//drawrectangle
		CPen pen(PS_SOLID, 0, (COLORREF)GetColor());
		
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(m_ptTopLeft.x, m_ptTopLeft.y, m_ptBottomRight.x, m_ptBottomRight.y);
		pDC->SelectObject(pOldPen);
	}
	if (IsMarked())
	{
		//add four squares to all corners
		//use fillsolidrect ??
		CPen pen(PS_SOLID, 0, Utility::Black);
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush brush(Utility::Black);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CRect rcTopLeft(m_ptTopLeft.x - (SQUARE_SIDE / 2),
			m_ptTopLeft.y - (SQUARE_SIDE / 2),
			m_ptTopLeft.x + (SQUARE_SIDE / 2),
			m_ptTopLeft.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcTopLeft);

		CRect rcTopRight(m_ptBottomRight.x - (SQUARE_SIDE / 2),
			m_ptTopLeft.y - (SQUARE_SIDE / 2),
			m_ptBottomRight.x + (SQUARE_SIDE / 2),
			m_ptTopLeft.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcTopRight);


		CRect rcBottomLeft(m_ptTopLeft.x - (SQUARE_SIDE / 2),
			m_ptBottomRight.y - (SQUARE_SIDE / 2),
			m_ptTopLeft.x + (SQUARE_SIDE / 2),
			m_ptBottomRight.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcBottomLeft);


		CRect rcBottomRight(m_ptBottomRight.x - (SQUARE_SIDE / 2),
			m_ptBottomRight.y - (SQUARE_SIDE / 2),
			m_ptBottomRight.x + (SQUARE_SIDE / 2),
			m_ptBottomRight.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcBottomRight);
		


		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);



	}


}

CRect RectangleFigure::GetArea() const
{
	CRect rcRectangle(m_ptTopLeft, m_ptBottomRight);

	rcRectangle.NormalizeRect();
	if (IsMarked())
	{
		rcRectangle.left -= (SQUARE_SIDE / 2);
		rcRectangle.right += (SQUARE_SIDE / 2);
		rcRectangle.top -= (SQUARE_SIDE / 2);
		rcRectangle.bottom += (SQUARE_SIDE / 2);
	}

	return rcRectangle;

}



