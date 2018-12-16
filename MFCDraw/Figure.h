#pragma once

// Figure MFC class
//
// this is an abstract base class
// it handles the color and mark status of a drawing figure
// the virtual methods are to be defined in the subclasses
//
class Figure : public CObject
{
public:
	Figure();
	

	Figure(const Utility::Color& color, BOOL bMarked = FALSE);
	Figure(const Figure& figure);
	virtual Figure* Copy() const = 0;
	BOOL IsMarked() const { return m_bMarked; }
	void Mark(const BOOL bMarked) { m_bMarked = bMarked; }
	Utility::Color GetColor() const { return m_figureColor; }
	void SetColor(const Utility::Color& color) { m_figureColor = color; }
	virtual void Serialize(CArchive& archive);
	virtual HCURSOR GetCursor() const = 0;
	virtual BOOL Click(const CPoint& ptMouse) = 0;
	virtual BOOL DoubleClick(const CPoint& ptMouse) = 0;
	virtual BOOL Inside(const CRect& rcInside) const = 0;
	virtual void MoveOrModify(const CSize& szDistance) = 0;
	virtual void Move(const CSize& szDistance) = 0;
	virtual void Draw(CDC* pDC) const = 0;
	virtual CRect GetArea() const = 0;

private:
	BOOL m_bMarked;
	Utility::Color m_figureColor;

protected:
	static const int SQUARE_SIDE = 200;		//size of the black squares marking a figure
											//in 200 logical units ; or 200*1/100 mm = 2 mm

};


typedef MyList<Figure*>  FigurePointerList;