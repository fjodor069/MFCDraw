#include "stdafx.h"

#include "../../Utility/check.h"

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




FigureFileManager::FigureFileManager(Figure * pFigure)
{
}

void FigureFileManager::Serialize(CArchive & archive)
{

	CObject::Serialize(archive);
	if (archive.IsStoring())
	{
		archive << GetId();
	}
	if (archive.IsLoading())
	{
		int iId;
		archive >> iId;
		CreateFigure(iId);
	}


}

int FigureFileManager::GetId() const
{
	//added enum definitions
	//
	if (dynamic_cast<ArrowFigure*>(m_pFigure) != NULL)
	{
		return FigureID::ARROW;
	}
	else if (dynamic_cast<LineFigure*>(m_pFigure) != NULL)
	{
		return FigureID::LINE;
	}
	else if (dynamic_cast<RectangleFigure*>(m_pFigure) != NULL)
	{
		return FigureID::RECTANGLE;
	}
	else if (dynamic_cast<EllipseFigure*>(m_pFigure) != NULL)
	{
		return FigureID::ELLIPSE;
	}
	else if (dynamic_cast<TextFigure*>(m_pFigure) != NULL)
	{
		return FigureID::TEXT;
	}
	else
	{
		check(FALSE);
		return 0;
	}
	
}

void FigureFileManager::CreateFigure(int iId)
{
	switch (iId)
	{

		case FigureID::LINE:
			check_memory(m_pFigure = new LineFigure());
			break;
		
		case FigureID::ARROW:
			check_memory(m_pFigure = new ArrowFigure());
			break;

		case FigureID::ELLIPSE:
			check_memory(m_pFigure = new EllipseFigure());
			break;

		case FigureID::RECTANGLE:
			check_memory(m_pFigure = new RectangleFigure());
			break;

		case FigureID::TEXT:
			check_memory(m_pFigure = new TextFigure());
			break;
			
		default:
			check(FALSE);
	}

}
