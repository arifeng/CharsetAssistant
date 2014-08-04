// RichEditGS.cpp : implementation file
//
#include "stdafx.h"
#include "RichEditCtrlGS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlGS
CRichEditCtrlGS::CRichEditCtrlGS()
{
}

CRichEditCtrlGS::~CRichEditCtrlGS()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlGS, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlGS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Character and Font Attributes
int CRichEditCtrlGS::IsBold(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_BOLD;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false; // BOLD inconsistent over the whole Selection
  if( dwSelMask & CFM_BOLD )// BOLD consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_BOLD )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsItalic(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_ITALIC;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false;   // ITALIC inconsistent over the whole Selection
  if( dwSelMask & CFM_ITALIC )// ITALIC consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_ITALIC )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsUnderlined(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_UNDERLINE;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false;      // UNDERLINE inconsistent over the whole Selection
  if( dwSelMask & CFM_UNDERLINE )// UNDERLINE consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_UNDERLINE )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsStrikeout(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_STRIKEOUT;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false;      // STRIKEOUT inconsistent over the whole Selection
  if( dwSelMask & CFM_STRIKEOUT )// STRIKEOUT consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_STRIKEOUT )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsProtected(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_PROTECTED;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false;      // PROTECTED inconsistent over the whole Selection
  if( dwSelMask & CFM_PROTECTED )// PROTECTED consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_PROTECTED )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsLinked(void)
{ CHARFORMAT cf;	
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_LINK;

  DWORD dwSelMask = GetSelectionCharFormat(cf);
  bool bConsistent = false;      // PROTECTED inconsistent over the whole Selection
  if( dwSelMask & CFM_LINK )// PROTECTED consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( cf.dwEffects & CFE_LINK )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

void CRichEditCtrlGS::SetBold(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_BOLD;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle BOLD style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_BOLD) & (dwSelMask & CFM_BOLD) )
	{ cf.dwEffects ^= CFE_BOLD; 
	}
  else
	{ cf.dwEffects |= CFE_BOLD;
	}
  cf.dwMask = CFM_BOLD;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetItalic(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_ITALIC;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle ITALIC style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_ITALIC) & (dwSelMask & CFM_ITALIC) )
	{ cf.dwEffects ^= CFE_ITALIC; 
	}
  else
	{ cf.dwEffects |= CFE_ITALIC;
	}
  cf.dwMask = CFM_ITALIC;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetUnderlined(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_UNDERLINE;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle UNDERLINE style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_UNDERLINE) & (dwSelMask & CFM_UNDERLINE) )
	{ cf.dwEffects ^= CFE_UNDERLINE; 
	}
  else
	{ cf.dwEffects |= CFE_UNDERLINE;
	}
  cf.dwMask = CFM_UNDERLINE;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetStrikeout(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_STRIKEOUT;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle STRIKEOUT style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_STRIKEOUT) & (dwSelMask & CFM_STRIKEOUT) )
	{ cf.dwEffects ^= CFE_STRIKEOUT; 
	}
  else
	{ cf.dwEffects |= CFE_STRIKEOUT;
	}
  cf.dwMask = CFM_STRIKEOUT;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetProtected(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_PROTECTED;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle PROTECTED style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_PROTECTED) & (dwSelMask & CFM_PROTECTED) )
	{ cf.dwEffects ^= CFE_PROTECTED; 
	}
  else
	{ cf.dwEffects |= CFE_PROTECTED;
	}
  cf.dwMask = CFM_PROTECTED;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetLink(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_LINK;
 
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  // If selection is all the same toggle PROTECTED style
  // turn it on otherwise over the whole selection
  if( (cf.dwMask & CFM_LINK) & (dwSelMask & CFM_LINK) )
	{ cf.dwEffects ^= CFE_LINK; 
	}
  else
	{ cf.dwEffects |= CFE_LINK;
	}
  cf.dwMask = CFM_LINK;
  SetSelectionCharFormat(cf);
}

int CRichEditCtrlGS::GetFontSize(void)
{ CHARFORMAT cf;
  int nPointSize = 0;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_SIZE;
  DWORD dwSelMask = GetSelectionCharFormat(cf);
// return only the font size it is the same over the whole selection
  if( (cf.dwMask & CFM_SIZE) & (dwSelMask & CFM_SIZE) )
    { nPointSize = cf.yHeight/20;// convert from twips to points
	}
  return nPointSize;
}

void CRichEditCtrlGS::SetFontSize(int nPointSize)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_SIZE;
  GetSelectionCharFormat(cf);
  nPointSize *= 20;	// convert from points to twips
  cf.yHeight = nPointSize;
  cf.dwMask = CFM_SIZE;
  SetSelectionCharFormat(cf);
}

CString CRichEditCtrlGS::GetFontName(void)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_FACE;
  DWORD dwSelMask = GetSelectionCharFormat(cf);
  CString strName =_T("");
// return only the font name it is the same over the whole selection
  if( (cf.dwMask & CFM_FACE) & (dwSelMask & CFM_FACE) )
    { strName = cf.szFaceName;
	}
  return strName;
}

void CRichEditCtrlGS::SetFontName(const CString strFontName)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_FACE;
  GetSelectionCharFormat(cf);
  CString strName = strFontName.Left(31);  
  int i;
  for (i = 0; i <= strName.GetLength()-1; i++)
		cf.szFaceName[i] = strName[i];

  cf.szFaceName[i]='\0';
  cf.dwMask = CFM_FACE;
  SetSelectionCharFormat(cf);
}

void CRichEditCtrlGS::SetColour(COLORREF color)
{ CHARFORMAT cf;
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;

  GetSelectionCharFormat(cf);
  cf.crTextColor = color;
  if( cf.dwEffects & CFE_AUTOCOLOR )
    { cf.dwEffects ^= CFE_AUTOCOLOR;
	}
  SetSelectionCharFormat(cf);
}

// Paragraph Attributes
int CRichEditCtrlGS::IsRight()
{ PARAFORMAT pf;
  pf.cbSize = sizeof(PARAFORMAT);
  pf.dwMask = PFM_ALIGNMENT | PFA_RIGHT;    

  DWORD dwSelMask = GetParaFormat(pf);
  bool bConsistent = false;  // RIGHT alignment inconsistent over the whole Selection
  if( dwSelMask & PFA_RIGHT )// RIGHT alignment consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( (pf.wAlignment & 0xFF) == PFA_RIGHT )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsLeft(void)
{ PARAFORMAT pf;
  pf.cbSize = sizeof(PARAFORMAT);
  pf.dwMask = PFM_ALIGNMENT;    

  DWORD dwSelMask = GetParaFormat(pf);
  bool bConsistent = false; // LEFT alignment inconsistent over the whole Selection
  if( dwSelMask & PFA_LEFT )// LEFT alignment consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( (pf.wAlignment & 0xFF) == PFA_LEFT )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsCentered(void)
{ PARAFORMAT pf;
  pf.cbSize = sizeof(PARAFORMAT);
  pf.dwMask = PFM_ALIGNMENT;    

  DWORD dwSelMask = GetParaFormat(pf);
  bool bConsistent = false;   // CENTER alignment inconsistent over the whole Selection
  if( (dwSelMask & PFA_CENTER) == PFA_CENTER)// CENTER alignment consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( (pf.wAlignment & 0xFF) == PFA_CENTER )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsJustified(void)
{ PARAFORMAT pf;
  pf.cbSize = sizeof(PARAFORMAT);
  pf.dwMask = PFM_ALIGNMENT;    

  DWORD dwSelMask = GetParaFormat(pf);
  bool bConsistent = false;    // JUSTIFIED alignment inconsistent over the whole Selection
  if( dwSelMask & PFA_JUSTIFY )// JUSTIFIED alignment consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( (pf.wAlignment & 0xFF) == PFA_JUSTIFY )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}

int CRichEditCtrlGS::IsBulleted(void)
{ PARAFORMAT pf;
  pf.cbSize = sizeof(PARAFORMAT);
  pf.dwMask = PFM_ALIGNMENT | PFM_NUMBERING;    

  DWORD dwSelMask = GetParaFormat(pf);
  bool bConsistent = false;      // Paragraph NUMBERING inconsistent over the whole Selection
  if( dwSelMask & PFM_NUMBERING )// Paragraph NUMBERING consistent over the whole Selection?
    { bConsistent = true; 
    }
  if( !bConsistent )
    { return (-1); // Set Button to indeterminate
    }
  if( pf.wNumbering & PFN_BULLET )
    { return(1);   // Set Button to checked
    }
  return 0;        // Set Button to unchecked
}


void CRichEditCtrlGS::SetRight(void)
{ PARAFORMAT pf;    
  pf.cbSize     = sizeof(PARAFORMAT);
  pf.dwMask     = PFM_ALIGNMENT;    
  pf.wAlignment = PFA_RIGHT;

  SetParaFormat(pf);	// Set the paragraph.
}

void CRichEditCtrlGS::SetLeft(void)
{ PARAFORMAT pf;    
  pf.cbSize     = sizeof(PARAFORMAT);
  pf.dwMask     = PFM_ALIGNMENT;    
  pf.wAlignment = PFA_LEFT;

  SetParaFormat(pf);	// Set the paragraph.
}

void CRichEditCtrlGS::SetCenter(void)
{ PARAFORMAT pf;    
  pf.cbSize     = sizeof(PARAFORMAT);
  pf.dwMask     = PFM_ALIGNMENT;    
  pf.wAlignment = PFA_CENTER;

  SetParaFormat(pf);	// Set the paragraph.
}

void CRichEditCtrlGS::SetJustify(void)
{ PARAFORMAT pf;    
  pf.cbSize     = sizeof(PARAFORMAT);
  pf.dwMask     = PFM_ALIGNMENT;    
  pf.wAlignment = PFA_JUSTIFY;

  SetParaFormat(pf);	// Set the paragraph.
}

void CRichEditCtrlGS::SetBullet(void)
{ int iFormatted = IsBulleted();
  PARAFORMAT pf;
  pf.cbSize     = sizeof(PARAFORMAT);
  pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
  GetParaFormat(pf);
  
  if( 1 == iFormatted )
	{ pf.wNumbering = 0;
	  pf.dxOffset = 0;
	  pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
  else
	{ pf.wNumbering = PFN_BULLET;
	  pf.dwMask = PFM_NUMBERING;
	  if( pf.dxOffset == 0 )
		{ pf.dxOffset = 160;
		  pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
  SetParaFormat(pf);
}

// Visual Appearance
void CRichEditCtrlGS::SetWordWrap(const bool bOn/*= true*/,const int iLineWidth/* = 0*/)
{ if( bOn )
    { SetTargetDevice(NULL, iLineWidth);
	}
  else
    { if( 0 == iLineWidth )
	    { SetTargetDevice(NULL, 1);
		}
      else
	    { SetTargetDevice(NULL, iLineWidth);
		}
	}
}


// Reading and Writing
void CRichEditCtrlGS::SetRTF(const CString strText)
{ SCookieString CookieRTF;
  CookieRTF.lSize    = strText.GetLength();
  CookieRTF.lStart   = 0;
  CookieRTF.pInText  = &strText;
  CookieRTF.pOutText = NULL;
  
  CString strRTF = _T("");
  if( strText.GetLength() > 5 )
    { strRTF = strText.Left(5);
	}

// Read the text in
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamInCString;   // Set the callback
  es.dwCookie = (DWORD)&CookieRTF; // and the informations
  if( strRTF.CompareNoCase(_T("{\\rtf")) == 0 )
    { StreamIn(SF_RTF,es);
	}
  else
    { StreamIn(SF_TEXT,es);
	}
}

void CRichEditCtrlGS::SetRTF(const CByteArray& arrRTF)
{ SCookieByteArray CookieRTF;
  CookieRTF.lSize    = arrRTF.GetSize();
  CookieRTF.lStart   = 0;
  CookieRTF.pInText  = &arrRTF;
  CookieRTF.pOutText = NULL;
  
  CString strRTF = _T("");
  if( arrRTF.GetSize() > 5 )
    { strRTF  = TCHAR(arrRTF[0]);
      strRTF += TCHAR(arrRTF[1]);
  	  strRTF += TCHAR(arrRTF[2]);
	  strRTF += TCHAR(arrRTF[3]);
	  strRTF += TCHAR(arrRTF[4]);
	}

// Read the text in
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamInCByteArray; // Set the callback
  es.dwCookie = (DWORD)&CookieRTF;     // and the informations
  if( strRTF.CompareNoCase(_T("{\\rtf")) == 0 )
    { StreamIn(SF_RTF,es);
	}
  else
    { StreamIn(SF_TEXT,es);
	}
}

void CRichEditCtrlGS::SetRTF(const UINT resID)
{ // Obtain a handle and the memory to the resource
  HINSTANCE hApp = ::GetModuleHandle(0);
  ASSERT(hApp);
  HRSRC hResInfo = ::FindResource(hApp, MAKEINTRESOURCE(resID), TEXT("RTF"));
  if( NULL == hResInfo ) return;
  HGLOBAL hRes = ::LoadResource(hApp, hResInfo);
  if( NULL == hRes ) return;
  LPVOID pRTFText = ::LockResource(hRes);
  if( NULL == pRTFText ) return;
  DWORD dwRTFSize = ::SizeofResource(hApp, hResInfo);
  if( 0 == dwRTFSize )
    { ::FreeResource(hRes);
	  return;
    }
  CByteArray arrbRTF;
  arrbRTF.SetSize(dwRTFSize);
  LPBYTE pArrRTF = arrbRTF.GetData();
  ::CopyMemory(pArrRTF,pRTFText,dwRTFSize);
  ::FreeResource(hRes);
  SetRTF(arrbRTF);
}

void CRichEditCtrlGS::LoadRTF(const CString strFilename)
{ SCookieFile CookieRTF;
  CFileStatus fileStatus;
  CString strRTF = _T("");
  CookieRTF.lStart    = 0;
  CookieRTF.pFilename = &strFilename;
// Close all filehandles
  if( CFile::hFileNull != CookieRTF.fileInText.m_hFile )
    { CookieRTF.fileInText.Close();
	}
  if( CFile::hFileNull != CookieRTF.fileOutText.m_hFile )
    { CookieRTF.fileOutText.Close();
	}
// Is the filename the name of a file?
  if( CFile::GetStatus(strFilename, fileStatus) )
	{ if( (fileStatus.m_attribute & ( CFile::volume | CFile::directory )) )
	    { AfxMessageBox(AFX_IDP_INVALID_FILENAME);
		  return; // Filename is not the name of a file
	    }
    }
  else
    { AfxMessageBox(AFX_IDP_INVALID_FILENAME);
	  return;// The name is not existing
    }
  
  CookieRTF.lSize = fileStatus.m_size;
  if( !CookieRTF.fileInText.Open( *CookieRTF.pFilename,CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary ) )
    { AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
	  return; // File could not be opened
	}
  CookieRTF.fileInText.SeekToBegin();
  LPTSTR pBuff = strRTF.GetBuffer(15);
  CookieRTF.fileInText.Read(pBuff,5);
  pBuff[5] = 0;
  strRTF.ReleaseBuffer();
  
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamInCFile;  // Set the callback
  es.dwCookie = (DWORD)&CookieRTF; // and the informations

  if( strRTF.CompareNoCase(_T("{\\rtf")) == 0 )
    { StreamIn(SF_RTF,es);
	}
  else
    { StreamIn(SF_TEXT,es);
	}
  CookieRTF.fileInText.Close();
}

void CRichEditCtrlGS::GetRTF(CString& strText,const bool bAsRTF/*=true*/)
{ SCookieString CookieRTF;
  strText.Empty();
  CookieRTF.lSize    = 0;
  CookieRTF.lStart   = 0;
  CookieRTF.pInText  = NULL;
  CookieRTF.pOutText = &strText;
  
// Pull the text out
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamOutCString;  // Set the callback
  es.dwCookie = (DWORD)&CookieRTF; // and the informations
  if( bAsRTF )	
    { StreamOut(SF_RTF,es);
	}
  else
    { StreamOut(SF_TEXT,es);
	}
  return;
}

void CRichEditCtrlGS::GetRTF(CByteArray& arrText, const bool bAsRTF/*=true*/)
{ SCookieByteArray CookieRTF;
  arrText.RemoveAll();
  CookieRTF.lSize    = 0;
  CookieRTF.lStart   = 0;
  CookieRTF.pInText  = NULL;
  CookieRTF.pOutText = &arrText;
  
// Pull the text out
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamOutCByteArray;  // Set the callback
  es.dwCookie = (DWORD)&CookieRTF; // and the informations
  if( bAsRTF )	
    { StreamOut(SF_RTF,es);
	}
  else
    { StreamOut(SF_TEXT,es);
	}
}

void CRichEditCtrlGS::WriteRTF(const CString strFilename, const bool bAsRTF/*=true*/ )
{ SCookieFile CookieRTF;
  CFileStatus fileStatus;
  CString strRTF = _T("");
  CookieRTF.lStart    = 0;
  CookieRTF.pFilename = &strFilename;
// Close all filehandles
  if( CFile::hFileNull != CookieRTF.fileInText.m_hFile )
    { CookieRTF.fileInText.Close();
	}
  if( CFile::hFileNull != CookieRTF.fileOutText.m_hFile )
    { CookieRTF.fileOutText.Close();
	}
  if( !CookieRTF.fileOutText.Open( *CookieRTF.pFilename,CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary ) )
    { AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC);
	  return; // File could not be opened for saving
	}
  CookieRTF.fileOutText.SeekToBegin();
  
  EDITSTREAM es;
  es.dwError = 0;
  es.pfnCallback = StreamOutCFile; // Set the callback
  es.dwCookie = (DWORD)&CookieRTF; // and the informations
  if( bAsRTF )	
    { StreamOut(SF_RTF,es);
	}
  else
    { StreamOut(SF_TEXT,es);
	}
  CookieRTF.fileOutText.Close();
}

// StreamIn StreamOut methods
DWORD CALLBACK CRichEditCtrlGS::StreamInCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{ SCookieString *pCookie = (SCookieString *)dwCookie;
  long lMaxSize  = cb; // We may only read such much
  long lReadSize = cb; // We can only read such much
  *pcb = 0;            // Till now we have nothing read so far
  
  if( pCookie->lStart < 0 )
	{ return(0); // Nothing to do
	}
  if( NULL == pCookie->pInText )
	{ return(0); // Nothing to do
	}
  if( pCookie->pInText->GetLength() < cb )
    { lMaxSize = pCookie->pInText->GetLength();
	}
  lReadSize = lMaxSize;
  if( (pCookie->lSize - pCookie->lStart) < lReadSize )
    { lReadSize = pCookie->lSize - pCookie->lStart;
	}
  if( lReadSize <= 0 )
	{ return (0); // Nothing to do
	}
  LPCTSTR pText = (LPCTSTR)(*(pCookie->pInText));
  pText += pCookie->lStart;
  pCookie->lStart += lReadSize;
  *pcb = lReadSize;
  memcpy(pbBuff,pText,lReadSize);
  return 0;
}

DWORD CALLBACK CRichEditCtrlGS::StreamInCByteArray(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{ SCookieByteArray *pCookie = (SCookieByteArray *)dwCookie;
  long lMaxSize  = cb; // We may only read such much
  long lReadSize = cb; // We can only read such much
  *pcb = 0;            // Till now we have nothing read so far

  if( pCookie->lStart < 0 )
	{ return(0); // Nothing to do
	}
  if( NULL == pCookie->pInText )
	{ return(0); // Nothing to do
	}
  if( pCookie->pInText->GetSize() < cb )
    { lMaxSize = pCookie->pInText->GetSize();
	}
  lReadSize = lMaxSize;
  if( (pCookie->lSize - pCookie->lStart) < lReadSize )
    { lReadSize = pCookie->lSize - pCookie->lStart;
	}
  if( lReadSize <= 0 )
	{ return (0); // Nothing to do
	}
  LPCTSTR pText = (LPCTSTR)(pCookie->pInText->GetData());
  pText += pCookie->lStart;
  pCookie->lStart += lReadSize;
  *pcb = lReadSize;
  memcpy(pbBuff,pText,lReadSize);
  return 0;
}

DWORD CALLBACK CRichEditCtrlGS::StreamInCFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{ SCookieFile *pCookie = (SCookieFile *)dwCookie;
  UINT uiReadSize = 0; 
  *pcb = 0;            // Till now we have nothing read so far
  if( pCookie->lStart < 0 )
	{ return (0); // Nothing to do
	}
  if( 0 == cb )
	{ return(0); // Nothing to do
	}
  if( CFile::hFileNull == pCookie->fileInText.m_hFile )
	{ return(0); // Nothing to do
	}

  pCookie->fileInText.Seek(pCookie->lStart,CFile::begin);
  uiReadSize = pCookie->fileInText.Read(pbBuff,cb);
  *pcb = uiReadSize;
  pCookie->lStart += uiReadSize;
  return 0;
}

DWORD CALLBACK CRichEditCtrlGS::StreamOutCString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{ SCookieString *pCookie = (SCookieString *)dwCookie;
  long lWriteSize = cb; // We can only Write such much
  *pcb = 0;             // Till now we have nothing written so far

  if( pCookie->lStart < 0 )
	{ return (0); // Nothing to do
	}
  if( 0 == cb )
	{ return(0); // Nothing to do
	}
  if( NULL == pCookie->pOutText )
	{ return(0); // Nothing to do
	}
  CString strBuffer = _T("");
  strBuffer = (CString)pbBuff;
  (*(pCookie->pOutText)) += strBuffer.Left(lWriteSize);
  *pcb = lWriteSize;   
  pCookie->lStart += lWriteSize;
  return 0;
}

DWORD CALLBACK CRichEditCtrlGS::StreamOutCByteArray(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{ SCookieByteArray *pCookie = (SCookieByteArray *)dwCookie;
  long lWriteSize = cb; // We can only Write such much
  *pcb = 0;             // Till now we have nothing written so far

  if( pCookie->lStart < 0 )
	{ return (0); // Nothing to do
	}
  if( 0 == cb )
	{ return(0); // Nothing to do
	}
  if( NULL == pCookie->pOutText )
	{ return(0); // Nothing to do
	}
  CByteArray arrBuffer;
  arrBuffer.SetSize(lWriteSize);
  BYTE *pByte = arrBuffer.GetData();
  memcpy(pByte,pbBuff,lWriteSize);
  pCookie->pOutText->Append(arrBuffer);
  *pcb = lWriteSize;   
  pCookie->lStart += lWriteSize;
  return 0;
}

DWORD CALLBACK CRichEditCtrlGS::StreamOutCFile(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{ SCookieFile *pCookie = (SCookieFile *)dwCookie;
  *pcb = 0;             // Till now we have nothing written so far
  if( pCookie->lStart < 0 )
	{ return (0); // Nothing to do
	}
  if( 0 == cb )
	{ return(0); // Nothing to do
	}
  if( CFile::hFileNull == pCookie->fileOutText.m_hFile )
	{ return(0); // Nothing to do
	}
  try
  { pCookie->fileOutText.SeekToEnd();
    pCookie->fileOutText.Write(pbBuff,cb);
  }
  catch (CFileException *e)
    { AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC);
	  e->Delete();
	  return (1); // File could not be opened for saving so stop saving
	}
  *pcb = cb;
  return 0;
}