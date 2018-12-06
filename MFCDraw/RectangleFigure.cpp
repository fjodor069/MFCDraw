#include "stdafx.h"

#include "Color.h"
#include "MyList.h"
#include "Figure.h"
#include "TwoDimensionalFigure.h"

#include "RectangleFigure.h"


RectangleFigure::RectangleFigure()
	: TwoDimensionalFigure()
{
}

RectangleFigure::RectangleFigure(const Utility::Color & color, const CPoint & ptTopLeft, BOOL bFilled)
{
}

RectangleFigure::RectangleFigure(const RectangleFigure & rectangle)
{
}

Figure * RectangleFigure::Copy() const
{
	return nullptr;
}

void RectangleFigure::Serialize(CArchive & archive)
{
}

HCURSOR RectangleFigure::GetCursor() const
{
	return HCURSOR();
}

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
	return 0;
}

BOOL RectangleFigure::Inside(const CRect & rcInside) const
{
	return 0;
}

void RectangleFigure::MoveOrModify(const CSize & szDistance)
{
}

void RectangleFigure::Move(const CSize & szDistance)
{
}

void RectangleFigure::Draw(CDC * pDC) const
{
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



