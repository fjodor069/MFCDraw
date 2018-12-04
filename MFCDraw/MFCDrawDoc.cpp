
// MFCDrawDoc.cpp : implementation of the CMFCDrawDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCDraw.h"
#endif


//list of includes 

#include "Caret.h"
#include "Color.h"
#include "Font.h"
#include "MyList.h"

#include "Figure.h"
#include "LineFigure.h"


//

#include "MFCDrawDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCDrawDoc

IMPLEMENT_DYNCREATE(CMFCDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDrawDoc, CDocument)
	ON_COMMAND(ID_ADD_LINE, &CMFCDrawDoc::OnAddLine)
	ON_COMMAND(ID_ADD_ARROW, &CMFCDrawDoc::OnAddArrow)
	ON_COMMAND(ID_ADD_RECTANGLE, &CMFCDrawDoc::OnAddRectangle)
	ON_COMMAND(ID_ADD_ELLIPSE, &CMFCDrawDoc::OnAddEllipse)
	ON_COMMAND(ID_ADD_TEXT, &CMFCDrawDoc::OnAddText)
	ON_COMMAND(ID_EDIT_CUT, &CMFCDrawDoc::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CMFCDrawDoc::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMFCDrawDoc::OnEditPaste)
	ON_COMMAND(ID_FORMAT_MODIFY, &CMFCDrawDoc::OnFormatModify)
	ON_COMMAND(ID_FORMAT_COLOR, &CMFCDrawDoc::OnFormatColor)
	ON_COMMAND(ID_FORMAT_FONT, &CMFCDrawDoc::OnFormatFont)
	ON_COMMAND(ID_FORMAT_FILL, &CMFCDrawDoc::OnFormatFill)
END_MESSAGE_MAP()


// CMFCDrawDoc construction/destruction

CMFCDrawDoc::CMFCDrawDoc()
{
	// TODO: add one-time construction code here
	

}

CMFCDrawDoc::~CMFCDrawDoc()
{
}

BOOL CMFCDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCDrawDoc serialization

void CMFCDrawDoc::Serialize(CArchive& ar)
{
	CDocument::Serialize(ar);
	if (ar.IsStoring())
	{

		/*archive << (int)m_figurePtrList.GetSize();
		for (POSITION position = m_figurePtrList.GetHeadPosition();
			position != NULL; m_figurePtrList.GetNext(position))
		{
			Figure* pFigure = m_figurePtrList.GetAt(position);
			FigureFileManager manager(pFigure);
			manager.Serialize(archive);
			pFigure->Serialize(archive);
		}*/
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCDrawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCDrawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCDrawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCDrawDoc diagnostics

#ifdef _DEBUG
void CMFCDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCDrawDoc commands

// the heart of the application...
// handle objects when mouse button is pressed..
//
void CMFCDrawDoc::MouseDown(CPoint ptMouse, BOOL bControlKeyDown, CDC * pDC)
{
	//save the current position
	m_ptPrevMouse = ptMouse;

	if (m_eApplicationState == EDIT_TEXT)
		KeyDown(VK_RETURN, pDC);

	//.....just some code for testing...

	//MyList<Figure*>  ObjectList;


	m_ptPoint = ptMouse;
	UpdateAllViews(NULL);


	//....just some code for testing..

	switch (m_eNextActionState)
	{
	case ADD_LINE:
		check_memory(m_pSingleFigure = new LineFigure(m_nextColor, ptMouse));
		m_figurePtrList.AddTail(m_pSingleFigure);
		m_eApplicationState = SINGLE_DRAG;
		SetModifiedFlag();
		break;

	case ADD_ARROW:
		// ...
		break;

	case ADD_TEXT:
	{
		//check_memory(m_pEditText = new TextFigure(m_nextColor, ptMouse, m_nextFont, pDC));
		//m_figurePtrList.AddTail(m_pEditText);
		/*m_eApplicationState = EDIT_TEXT;
		CRect rcCaret = m_pEditText->GetCaretArea
		(m_eKeyboardState);
		m_caret.SetAndShowCaret(rcCaret);
		SetModifiedFlag();*/
	}
	break;
	case MODIFY_FIGURE:
		//if (!bControlKeyDown)
		//{
		//	//UnmarkAllFigures();
		//}
		//Figure* pClickedFigure = NULL;
		//for (POSITION position = m_figurePtrList.GetTailPosition();
		//	//position != NULL; m_figurePtrList.GetPrev(position))
		//{
		//	Figure* pFigure = m_figurePtrList.GetAt(position);
		//	if (pFigure->Click(ptMouse))
		//	{
		//		pClickedFigure = pFigure;
		//		break;
		//	}
		//}
		//if (pClickedFigure != NULL)
		//{
		//	CRect rcOldFigure = pClickedFigure->GetArea();
		//	if (bControlKeyDown)

		//	{
		//		if (pClickedFigure->IsMarked())
		//		{
		//			pClickedFigure->Mark(FALSE);
		//			m_eApplicationState = IDLE;
		//		}
		//		else
		//		{
		//			pClickedFigure->Mark(TRUE);
		//			m_figurePtrList.Remove(pClickedFigure);
		//			m_figurePtrList.AddTail(pClickedFigure);
		//			m_eApplicationState = MULTIPLE_DRAG;
		//			SetModifiedFlag();
		//		}
		//	}
		//	else
		//	{
		//		m_pSingleFigure = pClickedFigure;
		//		m_pSingleFigure->Mark(TRUE);
		//		m_figurePtrList.Remove(m_pSingleFigure);
		//		m_figurePtrList.AddTail(m_pSingleFigure);
		//		CRect rcFigure = m_pSingleFigure->GetArea();
		//		UpdateAllViews(NULL, (LPARAM)&rcFigure);
		//		m_eApplicationState = SINGLE_DRAG;
		//		SetModifiedFlag();
		//	}
		//	CRect rcNewFigure = pClickedFigure->GetArea();
		//	UpdateAllViews(NULL, (LPARAM)&rcOldFigure);
		//	UpdateAllViews(NULL, (LPARAM)&rcNewFigure);
		//}
		//else
		//{
		//	check_memory(m_pDragRectangle = new RectangleFigure(GRAY, ptMouse, FALSE));
		//	m_eApplicationState = RECTANGLE_DRAG;
		//}
		break;

	}//end switch



}


// when the user moves the mouse with the left button pressed
//
void CMFCDrawDoc::MouseDrag(const CPoint & ptMouse)
{
	CSize szDistance = ptMouse - m_ptPrevMouse;
	m_ptPrevMouse = ptMouse;

	switch (m_eApplicationState)
	{
	case SINGLE_DRAG:
	{
		CRect rcOldFigure = m_pSingleFigure->GetArea();
		m_pSingleFigure->MoveOrModify(szDistance);
		CRect rcNewFigure = m_pSingleFigure->GetArea();
		UpdateAllViews(NULL, (LPARAM)&rcOldFigure);
		UpdateAllViews(NULL, (LPARAM)&rcNewFigure);
	}
	break;

	case MULTIPLE_DRAG:
	{
		for (POSITION position = m_figurePtrList.GetHeadPosition(); position != NULL;
			m_figurePtrList.GetNext(position))
		{
			Figure* pFigure = m_figurePtrList.GetAt(position);
			if (pFigure->IsMarked())
			{
				CRect rcOldFigure = pFigure->GetArea();
				pFigure->Move(szDistance);
				CRect rcNewFigure = pFigure->GetArea();
				UpdateAllViews(NULL, (LPARAM)&rcOldFigure);
				UpdateAllViews(NULL, (LPARAM)&rcNewFigure);
			}
		}
	}
	break;

	case RECTANGLE_DRAG:
	{
		/*CRect rcOldInside = m_pDragRectangle->GetArea();
		m_pDragRectangle->MoveOrModify(szDistance);
		CRect rcNewInside = m_pDragRectangle->GetArea();
		UpdateAllViews(NULL, (LPARAM)&rcOldInside);
		UpdateAllViews(NULL, (LPARAM)&rcNewInside);*/
	}
	break;


	case EDIT_TEXT:
	case IDLE:
		break;

	}//end switch


}


// do something if in the rectangle drag state
void CMFCDrawDoc::MouseUp()
{
	switch (m_eApplicationState)
	{
		case RECTANGLE_DRAG:
			/*CRect rcArea = m_pDragRectangle->GetArea();
			rcArea.NormalizeRect();
			POSITION position = m_figurePtrList.GetTailPosition();

			while (position != NULL)
			{
				Figure* pFigure = m_figurePtrList.GetPrev(position);
				if (pFigure->Inside(rcArea))
				{
					pFigure->Mark(TRUE);
					m_figurePtrList.Remove(pFigure);
					m_figurePtrList.AddTail(pFigure);
				}
			}
			delete m_pDragRectangle;
			m_pDragRectangle = NULL;
			UpdateAllViews(NULL, (LPARAM)&rcArea);
			m_eApplicationState = IDLE;*/
			break;

		case SINGLE_DRAG:
		case MULTIPLE_DRAG:
			m_eApplicationState = IDLE;
			break;

		case EDIT_TEXT:
		case IDLE:
			break;
	}


}


void CMFCDrawDoc::DoubleClick(CPoint ptMouse)
{
}


BOOL CMFCDrawDoc::KeyDown(UINT cChar, CDC* pDC)
{
	return 0;
}

void CMFCDrawDoc::CharDown(UINT cChar, CDC * pDC)
{
}

const HCURSOR CMFCDrawDoc::GetCursor() const
{
	return HCURSOR();
}






void CMFCDrawDoc::OnAddLine()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnAddArrow()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnAddRectangle()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnAddEllipse()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnAddText()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnEditCut()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnEditCopy()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnEditPaste()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnFormatModify()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnFormatColor()
{
	// show the MFC color dialog
	CColorDialog colorDialog(m_nextColor);

	if (colorDialog.DoModal() == IDOK)
	{
		m_nextColor = colorDialog.GetColor();
	}

}


void CMFCDrawDoc::OnFormatFont()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnFormatFill()
{
	// show the MFC color dialog
	CColorDialog colorDialog(m_nextColor);

	if (colorDialog.DoModal() == IDOK)
	{
		m_nextFillColor = colorDialog.GetColor();
	}
}
