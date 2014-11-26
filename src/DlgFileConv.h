#if !defined(AFX_DLGFILECONV_H__3D74F47C_BB47_44FE_B3F5_9F99328D103D__INCLUDED_)
#define AFX_DLGFILECONV_H__3D74F47C_BB47_44FE_B3F5_9F99328D103D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileConv.h : header file
//

typedef struct 
{
	utils::string_list src_files;
	std::string src_charset;
	std::string src_bom_charset;
	
	utils::string_list dst_files;
	std::string dst_charset;
	
	int total;
	int success;
	int failed;
	int cline;
	FILE* fp;

}ConvFileInfo;

/////////////////////////////////////////////////////////////////////////////
// CDlgFileConv dialog

class CDlgFileConv : public CDialog
{
// Construction
public:
	CDlgFileConv(CWnd* pParent = NULL);   // standard constructor

	void	ConvThreadProc();

protected:
	void SwitchSrcType();

	bool ConvertFileToCharset(
		FILE* fps, FILE* fpd, 
		const std::string& src_charset,
		const std::string& dst_charset);

// Dialog Data
	//{{AFX_DATA(CDlgFileConv)
	enum { IDD = IDD_DLG_FILECONV };
	CButton	m_rdSrcFile;
	CButton	m_rdSrcDir;
	CProgressCtrl	m_progTotal;
	CString	m_strSrcFile;
	CString	m_strSrcDir;
	CString	m_strDstDir;
	BOOL	m_bSaveSameDir;
	BOOL	m_bRecurDir;
	BOOL	m_bWriteBOM;
	CString	m_strDstCharset;
	CString	m_strSrcCharset;
	//}}AFX_DATA

	ConvFileInfo m_convFileInfo;
	uthread_t	m_thread;

	BOOL	m_bConverting;
	BOOL	m_bTerminate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFileConv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFileConv)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectSrcType(UINT nID);
	afx_msg void OnSelectSrcFile();
	afx_msg void OnSelectSrcDir();
	afx_msg void OnSelectDstDir();
	afx_msg void OnChkSaveSameDir();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnMsgConvProg(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMsgConvDone(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnMsgConvStop(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILECONV_H__3D74F47C_BB47_44FE_B3F5_9F99328D103D__INCLUDED_)
