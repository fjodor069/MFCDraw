#include "stdafx.h"

#include "MyList.h"
#include "Color.h"
#include "Font.h"
#include "Figure.h"


#include "TwoDimensionalFigure.h"

IMPLEMENT_DYNAMIC(TwoDimensionalFigure,Figure)

TwoDimensionalFigure::TwoDimensionalFigure() 
	: m_bFilled(FALSE)
{
	//empty
}



TwoDimensionalFigure::TwoDimensionalFigure(const Utility::Color & color, BOOL bFilled)
	: Figure(color),
	m_bFilled(bFilled)
{
	//empty
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


#ifdef _DEBUG
void TwoDimensionalFigure::Dump(CDumpContext & dc) const
{
	Figure::Dump(dc);

	dc << _T("TwoDimensionalFigure ");
}
#endif
