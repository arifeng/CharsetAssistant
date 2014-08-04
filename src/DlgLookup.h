#if !defined(AFX_DLGLOOKUP_H__2BB8D37C_7E2A_4E5F_944D_7AABB4ADEE42__INCLUDED_)
#define AFX_DLGLOOKUP_H__2BB8D37C_7E2A_4E5F_944D_7AABB4ADEE42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLookup.h : header file
//

#include "../third_party/ButtonST_src/BtnST.h"
#include "../third_party/RichEditCtrlGS.h"
#include "../third_party/Label.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLookup dialog

class CDlgLookup : public CDialog
{
// Construction
public:
	CDlgLookup(CWnd* pParent = NULL);   // standard constructor

	void ReLookup();

// Dialog Data
	//{{AFX_DATA(CDlgLookup)
	enum { IDD = IDD_DLG_LOOKUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLookup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLabel			m_lbTips;
	CButtonST		m_btHighlight;
	CComboBox		m_cbCharsets;
	CRichEditCtrlGS	m_edtText;

	CButton			m_rdHexCat;
	CButton			m_rdOctCat;
	CButton			m_rdBinCat;
	CButton			m_ckCapital;
	CButton			m_ckSigned;
	CButton			m_btLookup;
	CButton			m_btClear;
	CButton			m_btSave;

	CString			m_strCharset;
	CString			m_strInput;

	COLORREF		m_clrHighLight;
	CPPToolTip		m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CDlgLookup)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClear();
	afx_msg void OnSelectCharset(UINT nID);
	afx_msg void OnSelectScale(UINT nID);
	afx_msg void OnBnClickedBtColor();
	afx_msg void OnBnClickedBtSave();
	afx_msg void OnSelOtherCharsets();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOOKUP_H__2BB8D37C_7E2A_4E5F_944D_7AABB4ADEE42__INCLUDED_)
