
// MFCDrawDoc.h : interface of the CMFCDrawDoc class
//

#pragma once

//A4 formaat 216 mm x 297 mm in logical units (1/100 mm)
const int TOTAL_WIDTH = 21600, TOTAL_HEIGHT = 27900;

enum ApplicationState {	SINGLE_DRAG, MULTIPLE_DRAG, RECTANGLE_DRAG, EDIT_TEXT, IDLE };
enum NextActionState { ADD_LINE, ADD_ARROW, ADD_RECTANGLE,ADD_ELLIPSE, ADD_TEXT, MODIFY_FIGURE };



class CMFCDrawDoc : public CDocument
{
protected: // create from serialization only
	CMFCDrawDoc();
	DECLARE_DYNCREATE(CMFCDrawDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMFCDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
private:
	Utility::Caret m_caret;
	
	Utility::Font m_nextFont;				//the current font

	ApplicationState m_eApplicationState;	//state of the application
	NextActionState  m_eNextActionState;	//action state of the document
	KeyboardState m_eKeyboardState;
	Figure* m_pSingleFigure;
	TextFigure* m_pEditText;
	RectangleFigure* m_pDragRectangle;

	FigurePointerList m_figurePtrList, m_copyPtrList;		//list of objects 

	CPoint m_ptPrevMouse;					//the previous mouse position

public:
	void MouseDown(CPoint ptMouse, BOOL bControlKeyDown, CDC* pDC);
	void MouseDrag(const CPoint& ptMouse);
	void MouseUp();
	void DoubleClick(CPoint ptMouse);
	BOOL KeyDown(UINT cChar, CDC* pDC);
	void CharDown(UINT cChar, CDC* pDC);

	Utility::Color m_nextColor;			//the current drawing color
	Utility::Color m_nextFillColor;		//not used
	BOOL m_bNextFill;					//is the figure to be filled or not

	const FigurePointerList* GetFigurePtrList() const
	{
		return &m_figurePtrList;
	}
	
	const RectangleFigure* GetInsideRectangle() const
	{
		return m_pDragRectangle;
	}
	Utility::Caret* GetCaret() { return &m_caret; }

	const HCURSOR GetCursor() const;
	
	//generated menu command handlers
	afx_msg void OnAddLine();
	afx_msg void OnAddArrow();
	afx_msg void OnAddRectangle();
	afx_msg void OnAddEllipse();
	afx_msg void OnAddText();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnFormatModify();
	afx_msg void OnFormatColor();
	afx_msg void OnFormatFont();
	afx_msg void OnFormatFill();
	afx_msg void OnUpdateAddLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddArrow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddRectangle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddText(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatModify(CCmdUI *pCmdUI);
private:
	static BOOL IsMarked(Figure* pFigure);
	static BOOL IsMarkedText(Figure* pFigure);
	static BOOL IsMarkedAndFilled(Figure* pFigure);
	static BOOL IsMarkedAndNotFilled(Figure* pFigure);
	void UnmarkAllFigures();
	void ClearCopyList();

public:
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFormatFill(CCmdUI *pCmdUI);
};
