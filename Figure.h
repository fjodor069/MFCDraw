#pragma once

// Figure MFC class

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
	static const int SQUARE_SIDE = 200;


};


typedef MyList<Figure*>  FigurePointerList;