#include "stdafx.h"

#include "MyList.h"
#include "Color.h"

#include "Figure.h"
#include "LineFigure.h"


LineFigure::LineFigure()
	: Figure(Utility::Black, FALSE),
	m_eDragMode(CREATE_LINE)
{
}

LineFigure::LineFigure(const Utility::Color& color, const CPoint& ptMouse)
	: Figure(color),
	  m_ptFirst(ptMouse),
	  m_ptLast(ptMouse),
	  m_eDragMode(CREATE_LINE)
{
}

//copy constructor
LineFigure::LineFigure(const LineFigure& line)
	: Figure(line.GetColor())
{
	m_ptFirst = line.m_ptFirst;
	m_ptLast = line.m_ptLast;
	m_eDragMode = line.m_eDragMode;
	
}

Figure* LineFigure::Copy() const
{
	Figure* myFigure;

	myFigure = new LineFigure(*this);
	return myFigure;
}

void LineFigure::Serialize(CArchive& archive)
{
	Figure::Serialize(archive);
	if (archive.IsStoring())
	{
		archive << m_ptFirst;
		archive << m_ptLast;
	
	}
	if (archive.IsLoading())
	{
		archive >> m_ptFirst;
		archive >> m_ptLast;
		
	}
}

HCURSOR LineFigure::GetCursor() const
{
	HCURSOR myCursor;

	switch (m_eDragMode)
	{
		case CREATE_LINE:
		case MODIFY_FIRST:
		case MODIFY_LAST:
			myCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
			break;
		case MOVE_LINE:
			myCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
			break;
	}
	return myCursor;
}

BOOL LineFigure::Click(const CPoint & ptMouse)
{

	// Did the user click on the first end point?
	CRect rcFirst(m_ptFirst.x - (SQUARE_SIDE / 2),
		m_ptFirst.y - (SQUARE_SIDE / 2),
		m_ptFirst.x + (SQUARE_SIDE / 2),
		m_ptFirst.y + (SQUARE_SIDE / 2));
	if (rcFirst.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_FIRST;
		return TRUE;
	}
	// Or the second one?
	CRect rcLast(m_ptLast.x - (SQUARE_SIDE / 2),
		m_ptLast.y - (SQUARE_SIDE / 2),
		m_ptLast.x + (SQUARE_SIDE / 2),
		m_ptLast.y + (SQUARE_SIDE / 2));

	if (rcLast.PtInRect(ptMouse))
	{
		m_eDragMode = MODIFY_LAST;
		return TRUE;
	}
	m_eDragMode = MOVE_LINE;
	// If the line completely vertical?
	if (m_ptFirst.x == m_ptLast.x)
	{
		CRect rcLine(m_ptFirst.x - (SQUARE_SIDE / 2), m_ptFirst.y,
			m_ptLast.x + (SQUARE_SIDE / 2), m_ptLast.y);



		rcLine.NormalizeRect();
		return rcLine.PtInRect(ptMouse);
	}
	// Or not?
	else
	{
		CRect rcLine(m_ptFirst, m_ptLast);
		rcLine.NormalizeRect();
		if (rcLine.PtInRect(ptMouse))
		{
			CPoint ptMin = (m_ptFirst.x < m_ptLast.x) 	? m_ptFirst : m_ptLast;
			CPoint ptMax = (m_ptFirst.x < m_ptLast.x) 	? m_ptLast : m_ptFirst;
			int cxLine = ptMax.x - ptMin.x;
			int cyLine = ptMax.y - ptMin.y;
			int cxMouse = ptMouse.x - ptMin.x;
			int cyMouse = ptMouse.y - ptMin.y;
			return fabs(cyMouse - cxMouse * (double)cyLine / cxLine) <= (SQUARE_SIDE / 2);
		}
		return FALSE;

	}


}

BOOL LineFigure::DoubleClick(const CPoint & ptMouse)
{
	return 0;
}

BOOL LineFigure::Inside(const CRect & rcInside) const
{
	return (rcInside.PtInRect(m_ptFirst) &&	rcInside.PtInRect(m_ptLast));
	
	
}

void LineFigure::MoveOrModify(const CSize & szDistance)
{
	switch (m_eDragMode)
	{
	case CREATE_LINE:
		m_ptLast += szDistance;
		break;
	case MODIFY_FIRST:
		m_ptFirst += szDistance;
		break;
	case MODIFY_LAST:
		m_ptLast += szDistance;
		break;
	case MOVE_LINE:
		Move(szDistance);
		break;
	}


}

void LineFigure::Move(const CSize & szDistance)
{
	m_ptFirst += szDistance;
	m_ptLast += szDistance;
}


//do the actual drawing
void LineFigure::Draw(CDC * pDC) const
{
	CPen pen(PS_SOLID, 0, (COLORREF)GetColor());
	

	/*TRACE(_T("First x = %d, y = %d\n", m_ptFirst.x, m_ptFirst.y));
	TRACE(_T("Last x = %d, y = %d\n", m_ptLast.x, m_ptLast.y));*/

	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_ptFirst.x, m_ptFirst.y);
	pDC->LineTo(m_ptLast.x, m_ptLast.y);
	pDC->SelectObject(pOldPen);

	if (IsMarked())
	{
		//add two squares to both endpoints
		CPen pen(PS_SOLID, 0, Utility::Black); 
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush brush(Utility::Black);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		CRect rcFirst(m_ptFirst.x - (SQUARE_SIDE / 2),
					  m_ptFirst.y - (SQUARE_SIDE / 2),
					  m_ptFirst.x + (SQUARE_SIDE / 2),
					  m_ptFirst.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcFirst);

		CRect rcLast(m_ptLast.x - (SQUARE_SIDE / 2),
					 m_ptLast.y - (SQUARE_SIDE / 2),
					 m_ptLast.x + (SQUARE_SIDE / 2),
					 m_ptLast.y + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcLast);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
	

}

CRect LineFigure::GetArea() const
{
	CRect rcLine(m_ptFirst, m_ptLast);
	
	rcLine.NormalizeRect();
	if (IsMarked())
	{
		rcLine.left -= (SQUARE_SIDE / 2);
		rcLine.right += (SQUARE_SIDE / 2);
		rcLine.top -= (SQUARE_SIDE / 2);
		rcLine.bottom += (SQUARE_SIDE / 2);
	}
	return rcLine;

		
}


