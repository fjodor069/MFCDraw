// CFigureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDraw.h"
#include "CFigureDialog.h"
#include "afxdialogex.h"


// CFigureDialog dialog

IMPLEMENT_DYNAMIC(CFigureDialog, CDialog)

CFigureDialog::CFigureDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_strName(_T(""))
{

}

CFigureDialog::~CFigureDialog()
{
}

void CFigureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_LBString(pDX, IDC_LIST1, m_strName);
	DDX_Control(pDX, IDC_LIST1, m_wndListBox);


}


BEGIN_MESSAGE_MAP(CFigureDialog, CDialog)
END_MESSAGE_MAP()


// CFigureDialog message handlers


BOOL CFigureDialog::OnInitDialog()
{
	

	// initialise the listbox control
	//note : this is poor programming:
	//CListBox* pLB = (CListBox*)GetDlgItem(IDC_LIST1);
	
	m_wndListBox.Attach(GetDlgItem(IDC_LIST1)->m_hWnd);

	m_wndListBox.AddString(_T("Test Test Test"));
	//m_wndListBox.AddString(_T("Two"));
	//m_wndListBox.AddString(_T("Three"));
	

	POSITION pos = m_strList.GetHeadPosition();
	while (pos != NULL) 
	{
		CString string = m_strList.GetNext(pos);
		m_wndListBox.InsertString(-1,string);
		//TRACE(_T("%s\n"), string);
	}

	
	
	m_wndListBox.Detach();



	/*pLB->InsertString(-1, L"Figure 1");
	pLB->InsertString(-1, L"Figure 2");
	pLB->InsertString(-1, L"Figure 3");*/

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
