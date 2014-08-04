#if !defined(AFX_DLGHEXCAT_H__E503C340_762D_481F_9648_CC0C19D00D40__INCLUDED_)
#define AFX_DLGHEXCAT_H__E503C340_762D_481F_9648_CC0C19D00D40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHexcat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHexcat dialog

class CDlgHexcat : public CDialog
{
// Construction
public:
	CDlgHexcat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHexcat)
	enum { IDD = IDD_DLG_HEXCAT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void	HexCatFile(CString strFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHexcat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHexcat)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHEXCAT_H__E503C340_762D_481F_9648_CC0C19D00D40__INCLUDED_)
