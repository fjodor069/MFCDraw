#include "stdafx.h"

#include "MyList.h"
#include "Color.h"

#include "Figure.h"
#include "LineFigure.h"

#include "ArrowFigure.h"


ArrowFigure::ArrowFigure()
	: Figure(),
	  LineFigure()
{
}

ArrowFigure::ArrowFigure(const Utility::Color & color, const CPoint & ptMouse)
{
}

ArrowFigure::ArrowFigure(const ArrowFigure & arrow)
{
}

Figure * ArrowFigure::Copy() const
{
	return nullptr;
}

void ArrowFigure::Serialize(CArchive & archive)
{
}

BOOL ArrowFigure::DoubleClick(const CPoint & ptMouse)
{
	return 0;
}

void ArrowFigure::MoveOrModify(const CSize & szDistance)
{
}

void ArrowFigure::Move(const CSize & szDistance)
{
}

void ArrowFigure::Draw(CDC * pDC) const
{
}

CRect ArrowFigure::GetArea() const
{
	return CRect();
}

void ArrowFigure::SetArrowPoints()
{
}



