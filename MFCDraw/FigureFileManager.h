#pragma once
#include "afx.h"


class FigureFileManager : 	public CObject
{
public:
	FigureFileManager(Figure* pFigure  = NULL);
	Figure* GetFigure() const { return m_pFigure; }

	void Serialize(CArchive& archive);

private:
	int GetId() const;
	void CreateFigure(int iId);


	enum { LINE, ARROW, RECTANGLE, ELLIPSE, TEXT };
	Figure* m_pFigure;

};

