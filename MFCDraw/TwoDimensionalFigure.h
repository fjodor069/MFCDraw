#pragma once

//
// sub class of Figure and base class of RectangleFigure and EllipseFigure
//

class TwoDimensionalFigure : public Figure
{
	DECLARE_DYNAMIC(TwoDimensionalFigure)
public:
	TwoDimensionalFigure();


	TwoDimensionalFigure(const Utility::Color& color, BOOL bFilled);
	BOOL IsFilled() const { return m_bFilled; }
	void Fill(const BOOL bFill) { m_bFilled = bFill; }
	void Serialize(CArchive& archive);

private:
	BOOL m_bFilled;
public:
	virtual void Dump(CDumpContext& dc) const;
};

