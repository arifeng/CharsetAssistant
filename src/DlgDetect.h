#if !defined(AFX_DLGDETECT_H__9FD75EEE_B023_4B28_BDA6_B614D50668B6__INCLUDED_)
#define AFX_DLGDETECT_H__9FD75EEE_B023_4B28_BDA6_B614D50668B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDetect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDetect dialog

class CDlgDetect : public CDialog
{
// Construction
public:
	CDlgDetect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDetect)
	enum { IDD = IDD_DLG_DETECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDetect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	DetectFile(CString strFile);

	// Generated message map functions
	//{{AFX_MSG(CDlgDetect)
	afx_msg void OnBtDetScan();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDETECT_H__9FD75EEE_B023_4B28_BDA6_B614D50668B6__INCLUDED_)
