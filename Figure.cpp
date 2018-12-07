// Figure.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDraw.h"

#include "MyList.h"
#include "Color.h"
#include "Font.h"

using namespace Utility;

#include "Figure.h"


// Figure

Figure::Figure()
	: m_figureColor(0),
	  m_bMarked(FALSE)
{
}



Figure::Figure(const Color & color, BOOL bMarked)
	: m_figureColor(color),
	  m_bMarked(bMarked)
{
}

//copy constructor
Figure::Figure(const Figure & figure)
	: m_figureColor(figure.m_figureColor),
	  m_bMarked(figure.m_bMarked)
{
}


// Figure member functions

void Figure::Serialize(CArchive & archive)
{
	CObject::Serialize(archive);
	m_figureColor.Serialize(archive);
}
