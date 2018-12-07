#include "stdafx.h"

#include "MyList.h"
#include "Color.h"
#include "Font.h"
#include "Figure.h"


#include "TwoDimensionalFigure.h"


TwoDimensionalFigure::TwoDimensionalFigure() 
	: m_bFilled(FALSE)
{
}



TwoDimensionalFigure::TwoDimensionalFigure(const Utility::Color & color, BOOL bFilled)
	: Figure(color,bFilled),
	m_bFilled(bFilled)
{
}

void TwoDimensionalFigure::Serialize(CArchive & archive)
{
	Figure::Serialize(archive);
	if (archive.IsStoring())
	{
		archive << m_bFilled;
	}
	if (archive.IsLoading())
	{
		archive >> m_bFilled;
	}
}
