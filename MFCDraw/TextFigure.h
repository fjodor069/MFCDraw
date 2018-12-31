#pragma once



typedef CArray<int> IntArray;

enum KeyboardState { KM_INSERT, KM_OVERWRITE };


class TextFigure :	public Figure
{
	DECLARE_DYNAMIC(TextFigure)
public:
	TextFigure();
	
	TextFigure(const Utility::Color& color, const CPoint& ptMouse,	const Utility::Font& font, CDC* pDC);
	TextFigure(const TextFigure& text);
	Figure* Copy() const;
	void Serialize(CArchive& archive);
	BOOL Click(const CPoint& ptMouse);
	BOOL DoubleClick(const CPoint& ptMouse);
	BOOL Inside(const CRect& rcInside) const;
	void MoveOrModify(const CSize& szDistance);
	void Move(const CSize& szDistance);
	BOOL KeyDown(UINT uChar, CDC* pDC);
	void CharDown(UINT uChar, CDC* pDC,
		KeyboardState eKeyboardState);
	void SetPreviousText(CDC* pDC);
	void Draw(CDC* pDC) const;
	CRect GetArea() const;
	Utility::Font* GetFont();
	void SetFont(const Utility::Font& font, CDC* pDC);

	CRect GetCaretArea(KeyboardState eKeyboardState);
	HCURSOR GetCursor() const;



private:
	void GenerateCaretArray(CDC* pDC);

	enum { CREATE_TEXT, MOVE_TEXT, EDIT_TEXT, NONE_TEXT } m_eDragMode;
	CPoint m_ptText;
	CSize m_szText;
	CString m_stText, m_stPreviousText;
	int m_iAverageWidth;
	Utility::Font m_font;
	int m_iEditIndex;
	IntArray m_caretArray;


public:
	virtual void Dump(CDumpContext& dc) const;
};

