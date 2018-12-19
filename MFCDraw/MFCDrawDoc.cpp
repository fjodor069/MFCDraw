
// MFCDrawDoc.cpp : implementation of the CMFCDrawDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCDraw.h"
#endif


//list of includes 
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
	ON_UPDATE_COMMAND_UI(ID_ADD_LINE, &CMFCDrawDoc::OnUpdateAddLine)
	ON_UPDATE_COMMAND_UI(ID_ADD_ARROW, &CMFCDrawDoc::OnUpdateAddArrow)
	ON_UPDATE_COMMAND_UI(ID_ADD_RECTANGLE, &CMFCDrawDoc::OnUpdateAddRectangle)
	ON_UPDATE_COMMAND_UI(ID_ADD_ELLIPSE, &CMFCDrawDoc::OnUpdateAddEllipse)
	ON_UPDATE_COMMAND_UI(ID_ADD_TEXT, &CMFCDrawDoc::OnUpdateAddText)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_MODIFY, &CMFCDrawDoc::OnUpdateFormatModify)
	ON_COMMAND(ID_EDIT_DELETE, &CMFCDrawDoc::OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CMFCDrawDoc::OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMFCDrawDoc::OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CMFCDrawDoc::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMFCDrawDoc::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_FILL, &CMFCDrawDoc::OnUpdateFormatFill)
END_MESSAGE_MAP()


// CMFCDrawDoc construction/destruction

CMFCDrawDoc::CMFCDrawDoc()
	
{
	//retrieve action state (drawing mode) from registry when initializing
	m_eNextActionState = (NextActionState)AfxGetApp()->	GetProfileInt(TEXT("MFCDraw"), TEXT("ActionMode"), MODIFY_FIGURE);
	m_eApplicationState = IDLE;
	//retrieve color and fill;  the last parameter is the default value in case nothing is stored
	m_nextColor = (COLORREF)AfxGetApp()->GetProfileInt (TEXT("MFCDraw"), TEXT("CurrentColor"), Utility::Black);
	m_bNextFill = (BOOL)AfxGetApp()->GetProfileInt	(TEXT("MFCDraw"), TEXT("CurrentFill"), FALSE);
	


}

CMFCDrawDoc::~CMFCDrawDoc()
{
	//save action state state to registry on shutdown
	AfxGetApp()->WriteProfileInt(TEXT("MFCDraw"), TEXT("ActionMode"), (int)m_eNextActionState);
	// write color and fill
	AfxGetApp()->WriteProfileInt(TEXT("MFCDraw"), TEXT("CurrentColor"), (Utility::Color) m_nextColor);
	AfxGetApp()->WriteProfileInt(TEXT("MFCDraw"), TEXT("CurrentFill"), (BOOL) m_bNextFill);


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
// loads and stores the figures in the m_figurePtr list
// we cannot serialize the list itself because it contains pointers, not objects
// the list is iterated and stored one by one
// the figurefilemanager helps storing the class type of the object
void CMFCDrawDoc::Serialize(CArchive& ar)
{
	CDocument::Serialize(ar);

	if (ar.IsStoring())
	{

		ar << (int)m_figurePtrList.GetSize();
		for (POSITION position = m_figurePtrList.GetHeadPosition();
			position != NULL; m_figurePtrList.GetNext(position))
		{
			Figure* pFigure = m_figurePtrList.GetAt(position);
			FigureFileManager manager(pFigure);
			manager.Serialize(ar);
			pFigure->Serialize(ar);
		}
	}
	else
	{
		int iSize;
		ar >> iSize;
		for (int iIndex = 0; iIndex < iSize; ++iIndex)
		{
			FigureFileManager manager;
			manager.Serialize(ar);
			Figure* pFigure = manager.GetFigure();
			pFigure->Serialize(ar);
			m_figurePtrList.AddTail(pFigure);
		}
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

// the heart of the application...mousedown and mouseup
// handle objects when mouse button is pressed..
//
void CMFCDrawDoc::MouseDown(CPoint ptMouse, BOOL bControlKeyDown, CDC * pDC)
{
	//save the current mouse position
	m_ptPrevMouse = ptMouse;

	//if editing text, close it with simulating return
	if (m_eApplicationState == EDIT_TEXT)
		KeyDown(VK_RETURN, pDC);
		
	//depending on a menu choice.. one of these actions is taken
	switch (m_eNextActionState)
	{
		//if new figure is to be added...at the current mouse position.
		case ADD_LINE:
			check_memory(m_pSingleFigure = new LineFigure(m_nextColor, ptMouse));
			m_figurePtrList.AddTail(m_pSingleFigure);
			m_eApplicationState = SINGLE_DRAG;
			SetModifiedFlag();
			afxDump << "Create line \n" ;
			break;

		case ADD_ARROW:
			check_memory(m_pSingleFigure = new ArrowFigure(m_nextColor, ptMouse));
			m_figurePtrList.AddTail(m_pSingleFigure);
			m_eApplicationState = SINGLE_DRAG;
			SetModifiedFlag();
			afxDump << "Create arrow \n";
			
			break;

		case ADD_RECTANGLE:
			check_memory(m_pSingleFigure = new RectangleFigure(m_nextColor, ptMouse,m_bNextFill));
			m_figurePtrList.AddTail(m_pSingleFigure);
			m_eApplicationState = SINGLE_DRAG;
			SetModifiedFlag();
			afxDump << "Create rectangle \n";
			break;

		case ADD_ELLIPSE:
			check_memory(m_pSingleFigure = new EllipseFigure(m_nextColor, ptMouse,m_bNextFill));
			m_figurePtrList.AddTail(m_pSingleFigure);
			m_eApplicationState = SINGLE_DRAG;
			SetModifiedFlag();
			afxDump << "Create ellipse \n";
			break;

		//keep adding text until return or escape is pressed
		case ADD_TEXT:
		{
			check_memory(m_pEditText = new TextFigure(m_nextColor, ptMouse, m_nextFont, pDC));
			m_figurePtrList.AddTail(m_pEditText);
			m_eApplicationState = EDIT_TEXT;
			CRect rcCaret = m_pEditText->GetCaretArea(m_eKeyboardState);
			m_caret.SetAndShowCaret(rcCaret);
			SetModifiedFlag();
			afxDump << "Create text \n";
		}
		break;

		case MODIFY_FIGURE:
			if (!bControlKeyDown)
			{
				UnmarkAllFigures();
			}
			Figure* pClickedFigure = NULL;
			for (POSITION position = m_figurePtrList.GetTailPosition();
				 position != NULL; m_figurePtrList.GetPrev(position))
			{
				Figure* pFigure = m_figurePtrList.GetAt(position);
				if (pFigure->Click(ptMouse))
				{
					pClickedFigure = pFigure;
					break;
				}
			}
			if (pClickedFigure != NULL)
			{
				CRect rcOldFigure = pClickedFigure->GetArea();
				if (bControlKeyDown)

				{
					if (pClickedFigure->IsMarked())
					{
						pClickedFigure->Mark(FALSE);
						m_eApplicationState = IDLE;
					}
					else
					{
						pClickedFigure->Mark(TRUE);
						m_figurePtrList.Remove(pClickedFigure);
						m_figurePtrList.AddTail(pClickedFigure);
						m_eApplicationState = MULTIPLE_DRAG;
						SetModifiedFlag();
					}
				}
				else
				{
					m_pSingleFigure = pClickedFigure;
					m_pSingleFigure->Mark(TRUE);
					m_figurePtrList.Remove(m_pSingleFigure);
					m_figurePtrList.AddTail(m_pSingleFigure);
					CRect rcFigure = m_pSingleFigure->GetArea();
					UpdateAllViews(NULL, (LPARAM)&rcFigure);
					m_eApplicationState = SINGLE_DRAG;
					SetModifiedFlag();
				}
				CRect rcNewFigure = pClickedFigure->GetArea();
				UpdateAllViews(NULL, (LPARAM)&rcOldFigure);
				UpdateAllViews(NULL, (LPARAM)&rcNewFigure);
			}
			else
			{
				check_memory(m_pDragRectangle = new RectangleFigure(Utility::Gray, ptMouse, FALSE));
				m_eApplicationState = RECTANGLE_DRAG;
			}
			break;

	}//end switch



}


// when the user moves the mouse with the left button pressed
//
void CMFCDrawDoc::MouseDrag(const CPoint & ptMouse)
{
	//calculate distance the mouse has moved
	//
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
			CRect rcOldInside = m_pDragRectangle->GetArea();
			m_pDragRectangle->MoveOrModify(szDistance);
			CRect rcNewInside = m_pDragRectangle->GetArea();
			UpdateAllViews(NULL, (LPARAM)&rcOldInside);
			UpdateAllViews(NULL, (LPARAM)&rcNewInside);
		}
		break;


		case EDIT_TEXT:
		case IDLE:
			break;

	}//end switch


}


//mouse button is released
// only do something if in the rectangle drag state
void CMFCDrawDoc::MouseUp()
{
	POSITION position;
	CRect rcArea;

	switch (m_eApplicationState)
	{
		case RECTANGLE_DRAG:
			rcArea = m_pDragRectangle->GetArea();
			rcArea.NormalizeRect();
			position = m_figurePtrList.GetTailPosition();

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
			m_eApplicationState = IDLE;
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

	switch (m_eNextActionState)
	{
		//...
	case MODIFY_FIGURE:
		UnmarkAllFigures();
		m_eApplicationState = IDLE;
		CRect rcOldArea;
		Figure* pClickedFigure = NULL;
		for (POSITION position = m_figurePtrList.
			GetTailPosition(); position != NULL;
			m_figurePtrList.GetPrev(position))
		{
			Figure* pFigure = m_figurePtrList.GetAt(position);
			rcOldArea = pFigure->GetArea();
			if (pFigure->DoubleClick(ptMouse))
			{
				pClickedFigure = pFigure;
				break;
			}
		}//end for

		if (pClickedFigure != NULL)
		{
			m_figurePtrList.Remove(pClickedFigure);
			m_figurePtrList.AddTail(pClickedFigure);
			m_pEditText = dynamic_cast<TextFigure*>	(pClickedFigure);
			if (m_pEditText != NULL)
			{
				CRect rcCaret = m_pEditText->GetCaretArea(m_eKeyboardState);
				m_caret.SetAndShowCaret(rcCaret);
				m_eApplicationState = EDIT_TEXT;
			}
			else
			{
				CRect rcNewArea = pClickedFigure->GetArea();
				UpdateAllViews(NULL, (LPARAM)&rcOldArea);
				UpdateAllViews(NULL, (LPARAM)&rcNewArea);
			}
		}
		break;
	}//end switch
}

//
// WM_KEYDOWN is sent for every key, including Insert, Del, Home, End
//
BOOL CMFCDrawDoc::KeyDown(UINT cChar, CDC* pDC)
{
	// ...
	int iMarked = m_figurePtrList.CountIf(IsMarked);
	if ((cChar == VK_DELETE) && (iMarked > 0))
	{
		OnEditDelete();
		return TRUE;
	}
	return FALSE;



	return 0;
}

//in addition to KeyDown, for a writeable key a WM_CHAR message is sent

void CMFCDrawDoc::CharDown(UINT cChar, CDC * pDC)
{
	if ((m_eApplicationState == EDIT_TEXT) && isprint(cChar))
	{
		m_pEditText->CharDown(cChar, pDC, m_eKeyboardState);
		CRect rcText = m_pEditText->GetArea();
		UpdateAllViews(NULL, (LPARAM)&rcText);
		CRect rcCaret = m_pEditText->GetCaretArea
		(m_eKeyboardState);
		m_caret.SetAndShowCaret(rcCaret);
		SetModifiedFlag();
	}



}

const HCURSOR CMFCDrawDoc::GetCursor() const
{
	switch (m_eApplicationState)
	{
	case SINGLE_DRAG:
		return m_pSingleFigure->GetCursor();
	case MULTIPLE_DRAG:
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	case RECTANGLE_DRAG:
		return AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	case EDIT_TEXT:
		return AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
	case IDLE:
		return AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	}
	check(FALSE);

	return NULL;
}




void CMFCDrawDoc::OnAddLine()
{
	UnmarkAllFigures();
	m_eNextActionState = ADD_LINE;
}


void CMFCDrawDoc::OnAddArrow()
{
	UnmarkAllFigures();
	m_eNextActionState = ADD_ARROW;
}


void CMFCDrawDoc::OnAddRectangle()
{
	UnmarkAllFigures();
	m_eNextActionState = ADD_RECTANGLE;
}


void CMFCDrawDoc::OnAddEllipse()
{
	UnmarkAllFigures();
	m_eNextActionState = ADD_ELLIPSE;
}


void CMFCDrawDoc::OnAddText()
{
	UnmarkAllFigures();
	m_eNextActionState = ADD_TEXT;
}


void CMFCDrawDoc::OnEditDelete()
{
	// TODO: Add your command handler code here
}



void CMFCDrawDoc::OnEditCut()
{
	OnEditCopy();
	OnEditDelete();

}


void CMFCDrawDoc::OnEditCopy()
{
	ClearCopyList();
	for (POSITION position = m_figurePtrList.GetHeadPosition();
		position != NULL; m_figurePtrList.GetNext(position))
	{
		Figure* pFigure = m_figurePtrList.GetAt(position);
		if (pFigure->IsMarked())
		{
			Figure* pCopiedFigure = pFigure->Copy();
			m_copyPtrList.AddTail(pCopiedFigure);
		}
	}



}


void CMFCDrawDoc::OnEditPaste()
{

	CSize szDistance(1000, -1000);
	for (POSITION position = m_copyPtrList.GetHeadPosition();
		position != NULL; m_copyPtrList.GetNext(position))
	{
		Figure* pCopiedFigure = m_copyPtrList.GetAt(position);
		pCopiedFigure->Move(szDistance);
		Figure* pPastedFigure = pCopiedFigure->Copy();
		m_figurePtrList.AddTail(pPastedFigure);
		CRect rcFigure = pPastedFigure->GetArea();
		UpdateAllViews(NULL, (LPARAM)&rcFigure);
	}
	SetModifiedFlag();


}


void CMFCDrawDoc::OnFormatModify()
{
	// TODO: Add your command handler code here
}


//choose color
void CMFCDrawDoc::OnFormatColor()
{
	// show the MFC color dialog
	CColorDialog colorDialog(m_nextColor);

	if (colorDialog.DoModal() == IDOK)
	{
		m_nextColor = colorDialog.GetColor();
	}

}


//choose font
void CMFCDrawDoc::OnFormatFont()
{
	// TODO: Add your command handler code here
}


void CMFCDrawDoc::OnFormatFill()
{
	//toggle fill
	m_bNextFill = !m_bNextFill;

	//// show the MFC color dialog to choose a fill color
	//CColorDialog colorDialog(m_nextColor);

	//if (colorDialog.DoModal() == IDOK)
	//{
	//	m_nextFillColor = colorDialog.GetColor();
	//}
}


void CMFCDrawDoc::OnUpdateAddLine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == ADD_LINE);
	// also possible: 
	//pCmdUI->SetCheck(m_eNextActionState == ADD_LINE);
}


void CMFCDrawDoc::OnUpdateAddArrow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == ADD_ARROW);
}


void CMFCDrawDoc::OnUpdateAddRectangle(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == ADD_RECTANGLE);
}


void CMFCDrawDoc::OnUpdateAddEllipse(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == ADD_ELLIPSE);
}


void CMFCDrawDoc::OnUpdateAddText(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == ADD_TEXT);
}


void CMFCDrawDoc::OnUpdateFormatModify(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_eApplicationState == IDLE);
	pCmdUI->SetRadio(m_eNextActionState == MODIFY_FIGURE);
}







BOOL CMFCDrawDoc::IsMarked(Figure* pFigure)
{
	return pFigure->IsMarked();


}


BOOL CMFCDrawDoc::IsMarkedText(Figure* pFigure)
{
	return 0;
}


BOOL CMFCDrawDoc::IsMarkedAndFilled(Figure* pFigure)
{
	return 0;
}


BOOL CMFCDrawDoc::IsMarkedAndNotFilled(Figure* pFigure)
{
	return 0;
}


void CMFCDrawDoc::UnmarkAllFigures()
{
}


void CMFCDrawDoc::ClearCopyList()
{

	m_copyPtrList.RemoveAll();
	

}


void CMFCDrawDoc::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}



void CMFCDrawDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMFCDrawDoc::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	int iMarked = m_figurePtrList.CountIf(IsMarked);
	pCmdUI->Enable((m_eApplicationState == IDLE) && (iMarked > 0));


}


void CMFCDrawDoc::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	int iMarked = m_figurePtrList.CountIf(IsMarked);
	pCmdUI->Enable((m_eApplicationState == IDLE) &&	(iMarked > 0));

}


void CMFCDrawDoc::OnUpdateFormatFill(CCmdUI *pCmdUI)
{
	switch (m_eNextActionState)
	{
		// ...
	case MODIFY_FIGURE:
		int iFilled = m_figurePtrList.CountIf(IsMarkedAndFilled);
		int iNotFilled = m_figurePtrList.CountIf(IsMarkedAndNotFilled);
		BOOL bAtLeastOne = ((iFilled > 0) || (iNotFilled > 0));
		pCmdUI->Enable(bAtLeastOne);
		pCmdUI->SetCheck(bAtLeastOne && (iFilled >= iNotFilled));
		break;
	}
	pCmdUI->SetCheck(m_bNextFill);
}
