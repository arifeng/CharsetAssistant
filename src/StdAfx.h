// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__80AD3D83_494A_4BB3_8B3D_C42677A71412__INCLUDED_)
#define AFX_STDAFX_H__80AD3D83_494A_4BB3_8B3D_C42677A71412__INCLUDED_

#pragma warning (disable: 4786)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../utils/src/ccutil.h"

#include "../utils/MfcUtil.h"
using namespace mfcutil;
using namespace mfcharset;

extern const char *supported_charsets;

#include "../third_party/PPToolTipLIBu_lib/PPTooltip.h"
#ifdef _DEBUG
#pragma comment(lib, "third_party/PPToolTipLIBu_lib/PPToolTipLIBud.lib")
#else
#pragma comment(lib, "third_party/PPToolTipLIBu_lib/PPToolTipLIBu.lib")
#endif

#include "../third_party/TabCtrlLib_lib/TabCtrl.h"
#ifdef _DEBUG
#pragma comment(lib, "third_party/TabCtrlLib_lib/TabCtrlLibUD.lib")
#else
#pragma comment(lib, "third_party/TabCtrlLib_lib/TabCtrlLibU.lib")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__80AD3D83_494A_4BB3_8B3D_C42677A71412__INCLUDED_)
