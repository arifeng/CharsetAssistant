// AriCharsetDlg.h : header file
//

#if !defined(AFX_ARICHARSETDLG_H__6B916176_FD48_402C_9D2A_9D7AEF6B48D6__INCLUDED_)
#define AFX_ARICHARSETDLG_H__6B916176_FD48_402C_9D2A_9D7AEF6B48D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgLookup.h"
#include "DlgFileConv.h"
#include "DlgDetect.h"
#include "DlgHexcat.h"
#include "DlgAbout.h"

/////////////////////////////////////////////////////////////////////////////
// CAriCharsetDlg dialog

class CAriCharsetDlg : public CDialog
{
// Construction
public:
	CAriCharsetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAriCharsetDlg)
	enum { IDD = IDD_ARICHARSET_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAriCharsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	TabCtrl	m_TabCtrl;
	TabCtrlStyle_VS2010_client m_TabCtrlStyle;

	CDlgLookup		m_dlgLookup;						//编码查询窗口
	CDlgFileConv	m_dlgFileConv;						//文件转换窗口
	CDlgDetect		m_dlgDetect;						//未知编码检测
	CDlgHexcat		m_dlgHexcat;						//十六进制查看
	CDlgAbout		m_dlgAbout;							//帮助关于

	// Generated message map functions
	//{{AFX_MSG(CAriCharsetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARICHARSETDLG_H__6B916176_FD48_402C_9D2A_9D7AEF6B48D6__INCLUDED_)
