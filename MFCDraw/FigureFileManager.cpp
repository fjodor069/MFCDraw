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
	if (dynamic_cast<ArrowFigure*>(m_pFigure) != NULL)
	{
		return ARROW;
	}
	else if (dynamic_cast<LineFigure*>(m_pFigure) != NULL)
	{
		return LINE;
	}
	// ...

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

	case LINE:
		check_memory(m_pFigure = new LineFigure());
		break;
		// ...
	default:
		check(FALSE);
	}

}
