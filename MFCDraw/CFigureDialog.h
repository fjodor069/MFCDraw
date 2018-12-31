#pragma once


// CFigureDialog dialog

class CFigureDialog : public CDialog
{
	DECLARE_DYNAMIC(CFigureDialog)

public:
	CFigureDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFigureDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strName;
	CListBox m_wndListBox;
	CStringList m_strList;
};
