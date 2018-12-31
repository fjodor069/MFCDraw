#pragma once

//
// direct subclass of Figure by virtual inheritance 
// similar to linefigure
// 
class ArrowFigure : public virtual Figure, private LineFigure
{
	DECLARE_DYNAMIC(ArrowFigure)
public:
	ArrowFigure();
	ArrowFigure(const Utility::Color& color, const CPoint& ptMouse);
	ArrowFigure(const ArrowFigure& arrow);
	Figure* Copy() const;
	void Serialize(CArchive& archive);
	HCURSOR GetCursor() const {	return LineFigure::GetCursor();	}
	BOOL Click(const CPoint& ptMouse) 	{ return LineFigure::Click(ptMouse); }
	BOOL DoubleClick(const CPoint& ptMouse);
	BOOL Inside(const CRect& rcInside) const { return LineFigure::Inside(rcInside); }
	void MoveOrModify(const CSize& szDistance);
	void Move(const CSize& szDistance);
	void Draw(CDC *pDC) const;
	CRect GetArea() const;
private:
	void SetArrowPoints();
	CPoint m_ptLeft, m_ptRight;
	static const int ARROW_LENGTH = 500;


public:
	virtual void Dump(CDumpContext& dc) const;
};

