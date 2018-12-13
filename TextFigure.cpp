#include "stdafx.h"

#include "../../Utility/check.h"
#include "Color.h"
#include "Font.h"
#include "MyList.h"
#include "Figure.h"


#include "TextFigure.h"


TextFigure::TextFigure()
	: Figure()
{
}

TextFigure::TextFigure(const Utility::Color & color, const CPoint& ptMouse, const Utility::Font& font, CDC * pDC)
	: Figure(color),
	   m_ptText(ptMouse),
	   m_font(font)
{
}

TextFigure::TextFigure(const TextFigure & text)
{
}

Figure * TextFigure::Copy() const
{
	return nullptr;
}

void TextFigure::Serialize(CArchive & archive)
{
	Figure::Serialize(archive);
	m_font.Serialize(archive);
	m_caretArray.Serialize(archive);
	if (archive.IsStoring())
	{
		archive << m_ptText << m_stText << m_szText
			<< m_iAverageWidth;
	}
	if (archive.IsLoading())
	{
		archive >> m_ptText >> m_stText >> m_szText >> m_iAverageWidth;
		m_iEditIndex = 0;
	}
}

BOOL TextFigure::Click(const CPoint & ptMouse)
{

	m_eDragMode = MOVE_TEXT;
	CRect rcText(m_ptText, m_szText);
	return rcText.PtInRect(ptMouse);
}



BOOL TextFigure::DoubleClick(const CPoint & ptMouse)
{

	CRect rcText(m_ptText, m_szText);
	if (rcText.PtInRect(ptMouse))
	{
		CPoint ptTextMouse = ptMouse - m_ptText;
		int iSize = m_stText.GetLength();
		for (int iIndex = 0; iIndex < iSize; ++iIndex)
		{
			int iFirstPos = m_caretArray[iIndex];
			int iLastPos = m_caretArray[iIndex + 1] - 1;
			if ((ptTextMouse.x >= iFirstPos) &&
				(ptTextMouse.x <= iLastPos))
			{
				if ((ptTextMouse.x - iFirstPos) <
					(iLastPos - ptTextMouse.x))
				{
					m_iEditIndex = iIndex;
				}
				else
				{
					m_iEditIndex = iIndex + 1;
				}
				break;
			}
		}
		m_eDragMode = EDIT_TEXT;
		m_stPreviousText = m_stText;
		return TRUE;
	}

	check(FALSE);
	return FALSE;
	
}

BOOL TextFigure::Inside(const CRect & rcInside) const
{
	CRect rcText(m_ptText, m_szText);
	rcText.NormalizeRect();
	return rcInside.PtInRect(rcText.TopLeft()) &&  rcInside.PtInRect(rcText.BottomRight());
	
}

void TextFigure::MoveOrModify(const CSize & szDistance)
{
}

void TextFigure::Move(const CSize & szDistance)
{
}

BOOL TextFigure::KeyDown(UINT uChar, CDC * pDC)
{

	int iLength = m_stText.GetLength();
	switch (uChar)
	{
	case VK_HOME:
		if (m_iEditIndex > 0)
		{
			m_iEditIndex = 0;
		}
		break;
		// ...
	}
	return FALSE;

	
	
}

void TextFigure::CharDown(UINT uChar, CDC * pDC, KeyboardState eKeyboardState)
{
	if (m_iEditIndex == m_stText.GetLength())
	{
		m_stText.AppendChar((TCHAR)uChar);
	}
	else
	{
		switch (eKeyboardState)
		{
		case KM_INSERT:
			m_stText.Insert(m_iEditIndex, (TCHAR)uChar);
			break;
		case KM_OVERWRITE:
			m_stText.SetAt(m_iEditIndex, (TCHAR)uChar);
			break;
		}
	}
	++m_iEditIndex;
	GenerateCaretArray(pDC);

}

void TextFigure::SetPreviousText(CDC * pDC)
{
}

void TextFigure::Draw(CDC * pDC) const
{
	CFont cFont;
	cFont.CreateFontIndirect(m_font.PointsToMeters());
	CFont* pPrevFont = pDC->SelectObject(&cFont);
	pDC->SetTextColor((COLORREF)GetColor());
	pDC->TextOut(m_ptText.x, m_ptText.y + m_szText.cy,
		m_stText);
	pDC->SelectObject(pPrevFont);
	if (IsMarked())
	{
		CPen pen(PS_SOLID, 0, Utility::Black);
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush brush(Utility::Black);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		int xLeft = m_ptText.x;
		int xRight = m_ptText.x + m_szText.cx;
		int yTop = m_ptText.y;
		int yBottom = m_ptText.y + m_szText.cy;
		int xCenter = m_ptText.x + m_szText.cx / 2;
		int yCenter = m_ptText.y + m_szText.cy / 2;
		CRect rcLeft(xLeft - (SQUARE_SIDE / 2),
			yCenter - (SQUARE_SIDE / 2),
			xLeft + (SQUARE_SIDE / 2),
			yCenter + (SQUARE_SIDE / 2));
		CRect rcRight(xRight - (SQUARE_SIDE / 2),
			yCenter - (SQUARE_SIDE / 2),
			xRight + (SQUARE_SIDE / 2),
			yCenter + (SQUARE_SIDE / 2));
		CRect rcTop(xCenter - (SQUARE_SIDE / 2),
			yTop - (SQUARE_SIDE / 2),
			xCenter + (SQUARE_SIDE / 2),
			yTop + (SQUARE_SIDE / 2));
		CRect rcBottom(xCenter - (SQUARE_SIDE / 2),
			yBottom - (SQUARE_SIDE / 2),
			xCenter + (SQUARE_SIDE / 2),
			yBottom + (SQUARE_SIDE / 2));
		pDC->Rectangle(rcLeft);
		pDC->Rectangle(rcRight);
		pDC->Rectangle(rcTop);
		pDC->Rectangle(rcBottom);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

}

CRect TextFigure::GetArea() const
{
	CRect rcText(m_ptText, m_szText);
	rcText.NormalizeRect();
	if (IsMarked())
	{
		rcText.left -= (SQUARE_SIDE / 2);
		rcText.right += (SQUARE_SIDE / 2);
		rcText.top -= (SQUARE_SIDE / 2);
		rcText.bottom += (SQUARE_SIDE / 2);
	}
	return rcText;



}

Utility::Font* TextFigure::GetFont()
{
	return &m_font;
}

void TextFigure::SetFont(const Utility::Font & font, CDC * pDC)
{
	m_font = font;
	GenerateCaretArray(pDC);


}

CRect TextFigure::GetCaretArea(KeyboardState eKeyboardState)
{
	CPoint ptCaret(m_ptText.x + m_caretArray[m_iEditIndex],
		m_ptText.y);
	switch (eKeyboardState)
	{
	case KM_INSERT:
	{
		CSize szCaret(1, m_szText.cy);
		return CRect(ptCaret, ptCaret + szCaret);
	}
	break;
	case KM_OVERWRITE:
	{
		CSize szCaret(m_iAverageWidth, m_szText.cy);
		return CRect(ptCaret, ptCaret + szCaret);
	}
	break;
	}
	


	return CRect();
}

HCURSOR TextFigure::GetCursor() const
{
	return HCURSOR();
}

void TextFigure::GenerateCaretArray(CDC * pDC)
{
	CFont cFont;
	cFont.CreateFontIndirect(m_font.PointsToMeters());
	CFont* pPrevFont = pDC->SelectObject(&cFont);
	TEXTMETRIC textMetric;
	pDC->GetTextMetrics(&textMetric);
	m_iAverageWidth = textMetric.tmAveCharWidth;
	if (!m_stText.IsEmpty())
	{
		m_szText = pDC->GetTextExtent(m_stText);
	}
	else
	{
		m_szText.SetSize(0, textMetric.tmHeight);
	}
	int iWidth = 0, iSize = m_stText.GetLength();
	m_caretArray.SetSize(iSize + 1);
	for (int iIndex = 0; iIndex < iSize; ++iIndex)
	{
		CSize szChar = pDC->GetTextExtent
		(m_stText.Mid(iIndex, 1));
		m_caretArray[iIndex] = iWidth;
		iWidth += szChar.cx;
	}
	m_caretArray[iSize] = m_szText.cx;
	pDC->SelectObject(pPrevFont);


}


