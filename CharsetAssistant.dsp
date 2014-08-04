# Microsoft Developer Studio Project File - Name="CharsetAssistant" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CharsetAssistant - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CharsetAssistant.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CharsetAssistant.mak" CFG="CharsetAssistant - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CharsetAssistant - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CharsetAssistant - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CharsetAssistant - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_STLP_USE_STATIC_LIB" /D "_STLP_VERBOSE" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CharsetAssistant - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_STLP_USE_STATIC_LIB" /D "_STLP_VERBOSE" /D "_STLP_DEBUG" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CharsetAssistant - Win32 Release"
# Name "CharsetAssistant - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AriCharsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CharsetAssistant.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CharsetAssistant.rc
# End Source File
# Begin Source File

SOURCE=.\src\DlgAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DlgDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DlgFileConv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DlgHexcat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DlgLookup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\src\WubiCode.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\AriCharsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\CharsetAssistant.h
# End Source File
# Begin Source File

SOURCE=.\src\DlgAbout.h
# End Source File
# Begin Source File

SOURCE=.\src\DlgDetect.h
# End Source File
# Begin Source File

SOURCE=.\src\DlgFileConv.h
# End Source File
# Begin Source File

SOURCE=.\src\DlgHexcat.h
# End Source File
# Begin Source File

SOURCE=.\src\DlgLookup.h
# End Source File
# Begin Source File

SOURCE=.\src\Resource.h
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\src\WubiCode.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\res\AriCharset.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\CharsetAssistant.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\CharsetAssistant.rc2
# End Source File
# End Group
# Begin Group "utils"

# PROP Default_Filter ""
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utils\src\ccutil.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\utils\src\ccutil.h
# End Source File
# Begin Source File

SOURCE=.\utils\src\cutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\utils\src\cutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\utils\MfcUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\utils\MfcUtil.h
# End Source File
# Begin Source File

SOURCE=.\utils\utils_config.h
# End Source File
# End Group
# Begin Group "third_party"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\third_party\ButtonST_src\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\ColorDef.h
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\EnBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\ButtonST_src\EnBitmap.h
# End Source File
# Begin Source File

SOURCE=.\third_party\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\Label.h
# End Source File
# Begin Source File

SOURCE=.\third_party\RichEditCtrlGS.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\RichEditCtrlGS.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\third_party\ReadMe.txt
# End Source File
# End Target
# End Project
