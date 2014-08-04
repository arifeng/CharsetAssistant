#if !defined(RICHEDITCTRL_H__GS_20030325__INCLUDED_)
#define RICHEDITCTRL_H__GS_20030325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditGS.h : header file
//
#include <afxpriv.h>
#include <afxcmn.h>
#include <AfxRich.h>


/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlGS window
// Helper Structures
struct SCookieString
{ long lStart;
  long lSize;
  const CString *pInText;
  CString *pOutText;
};
struct SCookieByteArray
{ long lStart;
  long lSize;
  const CByteArray *pInText;
  CByteArray *pOutText;
};

struct SCookieFile
{ long lStart;
  uint64_t lSize;
  const CString *pFilename;
  CFile   fileInText;
  CFile   fileOutText;
};

class CRichEditCtrlGS : public CRichEditCtrl
{

// Construction / Destruction
public:
	CRichEditCtrlGS();
	virtual ~CRichEditCtrlGS();

// Attributes
public:
// Character and Font Attributes
	int IsBold(void);
	int IsItalic(void);
	int IsUnderlined(void);
	int IsStrikeout(void);
	int IsProtected(void);
	int IsLinked(void);

    void SetBold(void);
	void SetItalic(void);
	void SetUnderlined(void);
	void SetStrikeout(void);
	void SetProtected(void);
	void SetLink(void);

	int  GetFontSize(void);
	void SetFontSize(int nPointSize);
	CString GetFontName(void);
	void SetFontName(const CString strFontName);
    void SetColour(COLORREF color);

// Paragraph Attributes
	int IsRight(void);
	int IsLeft(void);
	int IsCentered(void);
	int IsJustified(void);
	int IsBulleted(void);

	void SetRight(void);
	void SetLeft(void);
	void SetCenter(void);
	void SetJustify(void);
	void SetBullet(void);

//	PARAFORMAT GetParagraphFormat();
// Visual Appearance
    void SetWordWrap(const bool bOn=true, const int iLineWidth=0);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlGS)
	//}}AFX_VIRTUAL

// Implementation
public:
// Reading and Writing
	void SetRTF(const CString strText);
	void SetRTF(const CByteArray& arrRTF);
	void SetRTF(const UINT resID);
    void LoadRTF(const CString strFilename);

    void GetRTF(CString& strText, const bool bAsRTF=true);
    void GetRTF(CByteArray& arrText, const bool bAsRTF=true);
    void WriteRTF(const CString strFilename, const bool bAsRTF=true);

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlGS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
// StreamIn and StreamOut methods
	static DWORD CALLBACK StreamInCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK StreamInCByteArray(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK StreamInCFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

	static DWORD CALLBACK StreamOutCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
	static DWORD CALLBACK StreamOutCByteArray(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
	static DWORD CALLBACK StreamOutCFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(RICHEDITCTRL_H__GS_20030325__INCLUDED_)