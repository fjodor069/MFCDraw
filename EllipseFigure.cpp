#include "stdafx.h"

#include "Color.h"
#include "MyList.h"
#include "Figure.h"
#include "TwoDimensionalFigure.h"
#include "RectangleFigure.h"


#include "EllipseFigure.h"


EllipseFigure::EllipseFigure()
	: TwoDimensionalFigure(),
	  RectangleFigure()
{
}

EllipseFigure::EllipseFigure(const Utility::Color & color, const CPoint & ptTopLeft, BOOL bFilled)
	: TwoDimensionalFigure(),
	  RectangleFigure()
{
}

EllipseFigure::EllipseFigure(const EllipseFigure & ellipse)
	: TwoDimensionalFigure(),
	RectangleFigure()
{
}

Figure * EllipseFigure::Copy() const
{
	return nullptr;
}

HCURSOR EllipseFigure::GetCursor() const
{
	return HCURSOR();
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
		return rgLargeArea.PtInRegion(ptMouse) &&
			!rgSmallArea.PtInRegion(ptMouse);
	}



}

void EllipseFigure::MoveOrModify(const CSize & szDistance)
{
}

void EllipseFigure::Draw(CDC * pDC) const
{
}



