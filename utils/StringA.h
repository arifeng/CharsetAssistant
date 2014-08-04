//////////////////////////////////////////////////////////////////////
//
// Copyright 2001 SilentBox Software
// All rights reserved.
//
// Creation: 05.05.2002
// Author: Oskar Wieland
//
// Purpose: Ascii CString class
//
// Modifications:
//
// 05.05.2002 - File created
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include guards
//////////////////////////////////////////////////////////////////////

#ifndef WTL_CSTRING_A_H__INCLUDED
#define WTL_CSTRING_A_H__INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////
// additional defines
//////////////////////////////////////////////////////////////////////

struct CStringDataA
{
    long nRefs;     // reference count
    int nDataLength;
    int nAllocLength;
    // WCHAR data[nAllocLength]

    CHAR* data()
        { return (CHAR*)(this + 1); }
};


//////////////////////////////////////////////////////////////////////
// global data
//////////////////////////////////////////////////////////////////////

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
_declspec(selectany) int rgInitDataA[] = { -1, 0, 0, 0 };
_declspec(selectany) CStringDataA* _atltmpDataNilA = (CStringDataA*)&rgInitDataA;
_declspec(selectany) LPCSTR _atltmpPchNilA = (LPCSTR)(((BYTE*)&rgInitDataA) + sizeof(CStringDataA));


//////////////////////////////////////////////////////////////////////
// CStringA class
//////////////////////////////////////////////////////////////////////

class CStringA
{
public:
// Constructors
    CStringA() 
        { Init(); }

    CStringA(const CStringA& stringSrc)
    {
        ATLASSERT(stringSrc.GetData()->nRefs != 0);
        if (stringSrc.GetData()->nRefs >= 0)
        {
            ATLASSERT(stringSrc.GetData() != _atltmpDataNilA);
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
        else
        {
            Init();
            *this = stringSrc.m_pchData;
        }
    }

    CStringA(CHAR ch, int nRepeat = 1)
    {
        Init();
        if (nRepeat >= 1)
        {
            if(AllocBuffer(nRepeat))
            {
                for (int i = 0; i < nRepeat; i++)
                    m_pchData[i] = ch;
            }
        }
    }

    CStringA(LPCSTR lpsz)
    {
        Init();
        if (lpsz != NULL && HIWORD(lpsz) == NULL)
        {
            UINT nID = LOWORD((DWORD_PTR)lpsz);
            if (!LoadString(nID))
                ATLTRACE2(atlTraceUI, 0, _T("Warning: implicit LoadString(%u) in CString failed\n"), nID);
        }
        else
        {
            int nSrcLen = (lpsz != NULL) ? lstrlenA(lpsz) : 0;
            if (nSrcLen != 0)
            {
                if(AllocBuffer(nSrcLen))
                    memcpy(m_pchData, lpsz, nSrcLen * sizeof(CHAR));
            }
        }
    }

    CStringA(LPCSTR ch, int nLength)
    {
        Init();
        if (nLength != 0)
        {
            if(AllocBuffer(nLength))
                memcpy(m_pchData, ch, nLength * sizeof(CHAR));
        }
    }

    CStringA(LPCWSTR lpsz)
    {
        Init();
        int nSrcLen = (lpsz != NULL) ? lstrlenW(lpsz) : 0;
        if (nSrcLen != 0)
        {
            if(AllocBuffer(nSrcLen))
            {
                _wcstombsz(m_pchData, lpsz, nSrcLen + 1);
                ReleaseBuffer();
            }
        }
    }

    CStringA(LPCWSTR lpch, int nLength)
    {
        Init();
        if (nLength != 0)
        {
            if(AllocBuffer(nLength))
            {
                _wcstombsz(m_pchData, lpch, nLength);
                ReleaseBuffer();
            }
        }
    }

    CStringA(const unsigned char* psz) 
        { Init(); *this = (LPCSTR)psz; }

// Attributes & Operations
    // as an array of characters
    int GetLength() const 
        { return GetData()->nDataLength; }

    BOOL IsEmpty() const
        { return GetData()->nDataLength == 0; }

    void Empty()                       // free up the data
    {
        if (GetData()->nDataLength == 0)
            return;

        if (GetData()->nRefs >= 0)
            Release();
        else
            *this = L"";

        ATLASSERT(GetData()->nDataLength == 0);
        ATLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
    }

    CHAR GetAt(int nIndex) const       // 0 based
    {
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);
        return m_pchData[nIndex];
    }

    CHAR operator[](int nIndex) const  // same as GetAt
    {
        // same as GetAt
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);
        return m_pchData[nIndex];
    }

    void SetAt(int nIndex, CHAR ch)
    {
        ATLASSERT(nIndex >= 0);
        ATLASSERT(nIndex < GetData()->nDataLength);

        CopyBeforeWrite();
        m_pchData[nIndex] = ch;
    }
    
    operator LPCSTR() const           // as a C string
        { return m_pchData; }

    // overloaded assignment
    const CStringA& operator=(const CStringA& stringSrc)
    {
        if (m_pchData != stringSrc.m_pchData)
        {
            if ((GetData()->nRefs < 0 && GetData() != _atltmpDataNilA) || stringSrc.GetData()->nRefs < 0)
            {
                // actual copy necessary since one of the strings is locked
                AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
            }
            else
            {
                // can just copy references around
                Release();
                ATLASSERT(stringSrc.GetData() != _atltmpDataNilA);
                m_pchData = stringSrc.m_pchData;
                InterlockedIncrement(&GetData()->nRefs);
            }
        }
        return *this;
    }

    const CStringA& operator=(WCHAR wch)
    {
        CStringA ch((LPCWSTR)wch, 1);
        AssignCopy(1, ch);
        return *this;
    }

    const CStringA& operator=(CHAR ch)
    {
        AssignCopy(1, &ch);
        return *this;
    }

    const CStringA& operator=(LPCSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        AssignCopy(SafeStrlen(lpsz), lpsz);
        return *this;
    }

    const CStringA& operator=(LPCWSTR lpsz)
    {
        int nSrcLen = (lpsz != NULL) ? lstrlenW(lpsz) : 0;
        if(AllocBeforeWrite(nSrcLen))
        {
            _wcstombsz(m_pchData, lpsz, nSrcLen + 1);
            ReleaseBuffer();
        }
        return *this;
    }
    
    const CStringA& operator=(const unsigned char* psz)
        { *this = (LPCSTR)psz; return *this; }

    // string concatenation
    const CStringA& CStringA::operator+=(LPCWSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        int nSrcLen = (lpsz != NULL) ? lstrlenW(lpsz) : 0;
        ConcatInPlace(nSrcLen, CStringA(lpsz));
        return *this;
    }

    const CStringA& operator+=(CHAR ch)
    {
        ConcatInPlace(1, &ch);
        return *this;
    }

    const CStringA& operator+=(LPCSTR lpsz)
    {
        ATLASSERT(lpsz == NULL || _IsValidString(lpsz));
        ConcatInPlace(SafeStrlen(lpsz), lpsz);
        return *this;
    }

    // Concatenation
    // NOTE: "operator+" is done as friend functions for simplicity

    friend CStringA __stdcall operator+(const CStringA& string1, const CStringA& string2);
    friend CStringA __stdcall operator+(const CStringA& string, char ch);
    friend CStringA __stdcall operator+(const CStringA& string, WCHAR ch);
    friend CStringA __stdcall operator+(WCHAR ch, const CStringA& string);
    friend CStringA __stdcall operator+(char ch, const CStringA& string);

    // string comparison
    int Compare(LPCSTR lpsz) const
        { return _cstrcmp(m_pchData, lpsz); }    // MBCS/Unicode aware

    int CompareNoCase(LPCSTR lpsz) const         // ignore case
        { return _cstrcmpi(m_pchData, lpsz); }   // MBCS/Unicode aware

    int Collate(LPCSTR lpsz) const               // NLS aware
        { return _cstrcoll(m_pchData, lpsz); }   // locale sensitive

    int CollateNoCase(LPCSTR lpsz) const         // ignore case
        { return _cstrcolli(m_pchData, lpsz); }  // locale sensitive

    // simple sub-string extraction
    CStringA Mid(int nFirst, int nCount) const
    {
        // out-of-bounds requests return sensible things
        if (nFirst < 0)
            nFirst = 0;
        if (nCount < 0)
            nCount = 0;

        if (nFirst + nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength - nFirst;
        if (nFirst > GetData()->nDataLength)
            nCount = 0;

        CStringA dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }


    CStringA Mid(int nFirst) const
    {
        return Mid(nFirst, GetData()->nDataLength - nFirst);
    }

    CStringA Left(int nCount) const
    {
        if (nCount < 0)
            nCount = 0;
        else if (nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength;

        CStringA dest;
        AllocCopy(dest, nCount, 0, 0);
        return dest;
    }

    CStringA Right(int nCount) const
    {
        if (nCount < 0)
            nCount = 0;
        else if (nCount > GetData()->nDataLength)
            nCount = GetData()->nDataLength;

        CStringA dest;
        AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
        return dest;
    }

    CStringA SpanIncluding(LPCSTR lpszCharSet) const
    {
        // strspn equivalent
        ATLASSERT(_IsValidString(lpszCharSet));
        return Left(_cstrspn(m_pchData, lpszCharSet));
    
    }
    
    CStringA SpanExcluding(LPCSTR lpszCharSet) const
    {
        // strcspn equivalent
        ATLASSERT(_IsValidString(lpszCharSet));
        return Left(_cstrcspn(m_pchData, lpszCharSet));
    }

    // upper/lower/reverse conversion
    void MakeUpper()
    {
        CopyBeforeWrite();
        CharUpperA(m_pchData);
    }

    void MakeLower()
    {
        CopyBeforeWrite();
        CharLowerA(m_pchData);
    }

    void MakeReverse()
    {
        CopyBeforeWrite();
        _cstrrev(m_pchData);
    }

    // trimming whitespace (either side)
    void TrimRight()
    {
        CopyBeforeWrite();

        // find beginning of trailing spaces by starting at beginning (DBCS aware)
        LPSTR lpsz = m_pchData;
        LPSTR lpszLast = NULL;
        while (*lpsz != '\0')
        {
            if (_cstrisspace(*lpsz))
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
            {
                lpszLast = NULL;
            }
            lpsz = ::CharNextA(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at trailing space start
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }

    void TrimLeft()
    {
        CopyBeforeWrite();

        // find first non-space character
        LPCSTR lpsz = m_pchData;
        while (_cstrisspace(*lpsz))
            lpsz = ::CharNextA(lpsz);

        // fix up data and length
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
        memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(CHAR));
        GetData()->nDataLength = nDataLength;
    }

    // remove continuous occurrences of chTarget starting from right
    void TrimRight(CHAR chTarget)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)

        CopyBeforeWrite();
        LPSTR lpsz = m_pchData;
        LPSTR lpszLast = NULL;

        while (*lpsz != '\0')
        {
            if (*lpsz == chTarget)
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
                lpszLast = NULL;
            lpsz = ::CharNextA(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }

    // remove continuous occcurrences of characters in passed string,
    // starting from right
    void TrimRight(LPCSTR lpszTargets)
    {
        // find beginning of trailing matches
        // by starting at beginning (DBCS aware)

        CopyBeforeWrite();
        LPSTR lpsz = m_pchData;
        LPSTR lpszLast = NULL;

        while (*lpsz != '\0')
        {
            if (_cstrchr(lpszTargets, *lpsz) != NULL)
            {
                if (lpszLast == NULL)
                    lpszLast = lpsz;
            }
            else
                lpszLast = NULL;
            lpsz = ::CharNextA(lpsz);
        }

        if (lpszLast != NULL)
        {
            // truncate at left-most matching character
            *lpszLast = '\0';
            GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
        }
    }


    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(CHAR chTarget)
    {
        // find first non-matching character

        CopyBeforeWrite();
        LPCSTR lpsz = m_pchData;

        while (chTarget == *lpsz)
            lpsz = ::CharNextA(lpsz);

        if (lpsz != m_pchData)
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
            memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(CHAR));
            GetData()->nDataLength = nDataLength;
        }
    }

    // remove continuous occcurrences of characters in
    // passed string, starting from left
    void TrimLeft(LPCSTR lpszTargets)
    {
        // if we're not trimming anything, we're not doing any work
        if (SafeStrlen(lpszTargets) == 0)
            return;

        CopyBeforeWrite();
        LPCSTR lpsz = m_pchData;

        while (*lpsz != '\0')
        {
            if (_cstrchr(lpszTargets, *lpsz) == NULL)
                break;
            lpsz = ::CharNextA(lpsz);
        }

        if (lpsz != m_pchData)
        {
            // fix up data and length
            int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
            memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(CHAR));
            GetData()->nDataLength = nDataLength;
        }
    }

    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(CHAR chOld, CHAR chNew)
    {
        int nCount = 0;

        // short-circuit the nop case
        if (chOld != chNew)
        {
            // otherwise modify each character that matches in the string
            CopyBeforeWrite();
            LPSTR psz = m_pchData;
            LPSTR pszEnd = psz + GetData()->nDataLength;
            while (psz < pszEnd)
            {
                // replace instances of the specified character only
                if (*psz == chOld)
                {
                    *psz = chNew;
                    nCount++;
                }
                psz = ::CharNextA(psz);
            }
        }
        return nCount;
    }

    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(LPCSTR lpszOld, LPCSTR lpszNew)
    {
        // can't have empty or NULL lpszOld

        int nSourceLen = SafeStrlen(lpszOld);
        if (nSourceLen == 0)
            return 0;
        int nReplacementLen = SafeStrlen(lpszNew);

        // loop once to figure out the size of the result string
        int nCount = 0;
        LPSTR lpszStart = m_pchData;
        LPSTR lpszEnd = m_pchData + GetData()->nDataLength;
        LPSTR lpszTarget;
        while (lpszStart < lpszEnd)
        {
            while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
            {
                nCount++;
                lpszStart = lpszTarget + nSourceLen;
            }
            lpszStart += lstrlenA(lpszStart) + 1;
        }

        // if any changes were made, make them
        if (nCount > 0)
        {
            CopyBeforeWrite();

            // if the buffer is too small, just
            //   allocate a new buffer (slow but sure)
            int nOldLength = GetData()->nDataLength;
            int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
            if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
            {
                CStringDataA* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength))
                    return -1;
                memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(CHAR));
                CStringA::Release(pOldData);
            }
            // else, we just do it in-place
            lpszStart = m_pchData;
            lpszEnd = m_pchData + GetData()->nDataLength;

            // loop again to actually do the work
            while (lpszStart < lpszEnd)
            {
                while ( (lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
                {
                    int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
                    memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(CHAR));
                    memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(CHAR));
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = '\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += lstrlenA(lpszStart) + 1;
            }
            ATLASSERT(m_pchData[nNewLength] == '\0');
            GetData()->nDataLength = nNewLength;
        }

        return nCount;
    }

    // remove occurrences of chRemove
    int Remove(CHAR chRemove)
    {
        CopyBeforeWrite();

        LPSTR pstrSource = m_pchData;
        LPSTR pstrDest = m_pchData;
        LPSTR pstrEnd = m_pchData + GetData()->nDataLength;

        while (pstrSource < pstrEnd)
        {
            if (*pstrSource != chRemove)
            {
                *pstrDest = *pstrSource;
                pstrDest = ::CharNextA(pstrDest);
            }
            pstrSource = ::CharNextA(pstrSource);
        }
        *pstrDest = '\0';
        int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
        GetData()->nDataLength -= nCount;

        return nCount;
    }

    // insert character at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, CHAR ch)
    {
        CopyBeforeWrite();

        if (nIndex < 0)
            nIndex = 0;

        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength)
            nIndex = nNewLength;
        nNewLength++;

        if (GetData()->nAllocLength < nNewLength)
        {
            CStringDataA* pOldData = GetData();
            LPSTR pstr = m_pchData;
            if(!AllocBuffer(nNewLength))
                return -1;
            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
            CStringA::Release(pOldData);
        }

        // move existing bytes down
        memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(CHAR));
        m_pchData[nIndex] = ch;
        GetData()->nDataLength = nNewLength;

        return nNewLength;
    }

    // insert substring at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, LPCSTR pstr)
    {
        if (nIndex < 0)
            nIndex = 0;

        int nInsertLength = SafeStrlen(pstr);
        int nNewLength = GetData()->nDataLength;
        if (nInsertLength > 0)
        {
            CopyBeforeWrite();
            if (nIndex > nNewLength)
                nIndex = nNewLength;
            nNewLength += nInsertLength;

            if (GetData()->nAllocLength < nNewLength)
            {
                CStringDataA* pOldData = GetData();
                LPSTR pstr = m_pchData;
                if(!AllocBuffer(nNewLength))
                    return -1;
                memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(CHAR));
                CStringA::Release(pOldData);
            }

            // move existing bytes down
            memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(CHAR));
            memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(CHAR));
            GetData()->nDataLength = nNewLength;
        }

        return nNewLength;
    }

    // delete nCount characters starting at zero-based index
    int Delete(int nIndex, int nCount = 1)
    {
        if (nIndex < 0)
            nIndex = 0;
        int nNewLength = GetData()->nDataLength;
        if (nCount > 0 && nIndex < nNewLength)
        {
            CopyBeforeWrite();
            int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

            memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(CHAR));
            GetData()->nDataLength = nNewLength - nCount;
        }

        return nNewLength;
    }

    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(CHAR ch) const                      // like "C" strchr
    {
        return Find(ch, 0);
    }

    int ReverseFind(CHAR ch) const
    {
        // find last single character
        LPSTR lpsz = _cstrrchr(m_pchData, ch);

        // return -1 if not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    int Find(CHAR ch, int nStart) const          // starting at index
    {
        int nLength = GetData()->nDataLength;
        if (nStart >= nLength)
            return -1;

        // find first single character
        LPSTR lpsz = _cstrchr(m_pchData + nStart, ch);

        // return -1 if not found and index otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    int FindOneOf(LPCSTR lpszCharSet) const
    {
        ATLASSERT(_IsValidString(lpszCharSet));
        LPSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    // look for a specific sub-string
    int Find(LPCSTR lpszSub) const        // like "C" strstr
        { return Find(lpszSub, 0); }

    int Find(LPCSTR lpszSub, int nStart) const  // starting at index
    {
        ATLASSERT(_IsValidString(lpszSub));

        int nLength = GetData()->nDataLength;
        if (nStart > nLength)
            return -1;

        // find first matching substring
        LPSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);

        // return -1 for not found, distance from beginning otherwise
        return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
    }

    // Concatentation for non strings
    const CStringA& Append(int n)
    {
        CHAR szBuffer[10];
        wsprintfA( szBuffer, "%d", n );
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }

    // simple formatting
    BOOL __cdecl Format(LPCSTR lpszFormat, ...)
    {
	    // format message into temporary buffer lpszTemp
	    va_list argList;
	    va_start(argList, lpszFormat);
	    LPSTR lpszTemp;
	    BOOL bRet = TRUE;

	    if (::FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			    lpszFormat, 0, 0, (LPSTR)&lpszTemp, 0, &argList) == 0 || lpszTemp == NULL)
		    bRet = FALSE;

	    // assign lpszTemp into the resulting string and free the temporary
	    *this = lpszTemp;
	    LocalFree(lpszTemp);
	    va_end(argList);
	    return bRet;
    }

    BOOL __cdecl Format(UINT nFormatID, ...)
    {
	    CStringA strFormat;
	    BOOL bRet = strFormat.LoadString(nFormatID);
	    ATLASSERT(bRet != 0);

	    va_list argList;
	    va_start(argList, nFormatID);
	    bRet = FormatV(strFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    BOOL FormatV(LPCSTR lpszFormat, va_list argList)
    {
	    ATLASSERT(_IsValidString(lpszFormat));

	    enum _FormatModifiers
	    {
		    FORCE_ANSI =	0x10000,
		    FORCE_UNICODE =	0x20000,
		    FORCE_INT64 =	0x40000
	    };

	    va_list argListSave = argList;

	    // make a guess at the maximum length of the resulting string
	    int nMaxLen = 0;
	    for (LPCSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
	    {
		    // handle '%' character, but watch out for '%%'
		    if (*lpsz != '%' || *(lpsz = ::CharNextA(lpsz)) == '%')
		    {
			    // this is instead of _tclen()
			    nMaxLen++;
			    continue;
		    }

		    int nItemLen = 0;

		    // handle '%' character with format
		    int nWidth = 0;
		    for (; *lpsz != '\0'; lpsz = ::CharNextA(lpsz))
		    {
			    // check for valid flags
			    if (*lpsz == '#')
				    nMaxLen += 2;   // for '0x'
			    else if (*lpsz == '*')
				    nWidth = va_arg(argList, int);
			    else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				    ;
			    else // hit non-flag character
				    break;
		    }
		    // get width and skip it
		    if (nWidth == 0)
		    {
			    // width indicated by
			    nWidth = _cstrtoi(lpsz);
			    for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextA(lpsz))
				    ;
		    }
		    ATLASSERT(nWidth >= 0);

		    int nPrecision = 0;
		    if (*lpsz == '.')
		    {
			    // skip past '.' separator (width.precision)
			    lpsz = ::CharNextA(lpsz);

			    // get precision and skip it
			    if (*lpsz == '*')
			    {
				    nPrecision = va_arg(argList, int);
				    lpsz = ::CharNextA(lpsz);
			    }
			    else
			    {
				    nPrecision = _cstrtoi(lpsz);
				    for (; *lpsz != '\0' && _cstrisdigit(*lpsz); lpsz = ::CharNextA(lpsz))
					    ;
			    }
			    ATLASSERT(nPrecision >= 0);
		    }

		    // should be on type modifier or specifier
		    int nModifier = 0;
		    if(lpsz[0] == _T('I') && lpsz[1] == _T('6') && lpsz[2] == _T('4'))
		    {
			    lpsz += 3;
			    nModifier = FORCE_INT64;
		    }
		    else
		    {
			    switch (*lpsz)
			    {
			    // modifiers that affect size
			    case 'h':
				    nModifier = FORCE_ANSI;
				    lpsz = ::CharNextA(lpsz);
				    break;
			    case 'l':
				    nModifier = FORCE_UNICODE;
				    lpsz = ::CharNextA(lpsz);
				    break;

			    // modifiers that do not affect size
			    case 'F':
			    case 'N':
			    case 'L':
				    lpsz = ::CharNextA(lpsz);
				    break;
			    }
		    }

		    // now should be on specifier
		    switch (*lpsz | nModifier)
		    {
		    // single characters
		    case 'c':
		    case 'C':
			    nItemLen = 2;
			    va_arg(argList, TCHAR);
			    break;
		    case 'c' | FORCE_ANSI:
		    case 'C' | FORCE_ANSI:
			    nItemLen = 2;
			    va_arg(argList, char);
			    break;
		    case 'c' | FORCE_UNICODE:
		    case 'C' | FORCE_UNICODE:
			    nItemLen = 2;
			    va_arg(argList, WCHAR);
			    break;

		    // strings
		    case 's':
		    {
			    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6;  // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenA(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }

		    case 'S':
		    {
#ifndef _UNICODE
			    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6;  // "(null)"
			    }
			    else
			    {
				    nItemLen = (int)wcslen(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
#else
			    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenA(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
#endif
			    break;
		    }

		    case 's' | FORCE_ANSI:
		    case 'S' | FORCE_ANSI:
		    {
			    LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = lstrlenA(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }

		    case 's' | FORCE_UNICODE:
		    case 'S' | FORCE_UNICODE:
		    {
			    LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			    if (pstrNextArg == NULL)
			    {
				    nItemLen = 6; // "(null)"
			    }
			    else
			    {
				    nItemLen = (int)wcslen(pstrNextArg);
				    nItemLen = max(1, nItemLen);
			    }
			    break;
		    }
		    }

		    // adjust nItemLen for strings
		    if (nItemLen != 0)
		    {
			    nItemLen = max(nItemLen, nWidth);
			    if (nPrecision != 0)
				    nItemLen = min(nItemLen, nPrecision);
		    }
		    else
		    {
			    switch (*lpsz)
			    {
			    // integers
			    case 'd':
			    case 'i':
			    case 'u':
			    case 'x':
			    case 'X':
			    case 'o':
				    if (nModifier & FORCE_INT64)
					    va_arg(argList, __int64);
				    else
					    va_arg(argList, int);
				    nItemLen = 32;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;

#ifndef _ATL_USE_CSTRING_FLOAT
			    case 'e':
			    case 'f':
			    case 'g':
			    case 'G':
				    ATLASSERT(!"Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class.");
#ifndef _DEBUG
				    ::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class."));
				    ::DebugBreak();
#endif //!_DEBUG
				    break;
#else //_ATL_USE_CSTRING_FLOAT
			    case 'e':
			    case 'g':
			    case 'G':
				    va_arg(argList, double);
				    nItemLen = 128;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;
			    case 'f':
				    {
					    double f;
					    LPTSTR pszTemp;

					    // 312 == strlen("-1+(309 zeroes).")
					    // 309 zeroes == max precision of a double
					    // 6 == adjustment in case precision is not specified,
					    //   which means that the precision defaults to 6
					    pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));

					    f = va_arg(argList, double);
					    _stprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					    nItemLen = _tcslen(pszTemp);
				    }
				    break;
#endif //_ATL_USE_CSTRING_FLOAT

			    case 'p':
				    va_arg(argList, void*);
				    nItemLen = 32;
				    nItemLen = max(nItemLen, nWidth + nPrecision);
				    break;

			    // no output
			    case 'n':
				    va_arg(argList, int*);
				    break;

			    default:
				    ATLASSERT(FALSE);  // unknown formatting option
			    }
		    }

		    // adjust nMaxLen for output nItemLen
		    nMaxLen += nItemLen;
	    }

	    if(GetBuffer(nMaxLen) == NULL)
		    return FALSE;
#ifndef _ATL_USE_CSTRING_FLOAT
	    int nRet = wvsprintfA(m_pchData, lpszFormat, argListSave);
#else //_ATL_USE_CSTRING_FLOAT
	    int nRet = vsprintf(m_pchData, lpszFormat, argListSave);
#endif //_ATL_USE_CSTRING_FLOAT
	    nRet;	// ref
	    ATLASSERT(nRet <= GetAllocLength());
	    ReleaseBuffer();

	    va_end(argListSave);
	    return TRUE;
    }

    // formatting for localization (uses FormatMessage API)
    BOOL __cdecl FormatMessage(LPCSTR lpszFormat, ...)
    {
	    ATLASSERT(_IsValidString(lpszFormat));

	    va_list argList;
	    va_start(argList, lpszFormat);
	    BOOL bRet = FormatV(lpszFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    BOOL __cdecl FormatMessage(UINT nFormatID, ...)
    {
	    CStringA strFormat;
	    BOOL bRet = strFormat.LoadString(nFormatID);
	    ATLASSERT(bRet != 0);

	    va_list argList;
	    va_start(argList, nFormatID);
	    bRet = FormatV(strFormat, argList);
	    va_end(argList);
	    return bRet;
    }

    // Windows support
    BOOL LoadString(UINT nID)          // load from string resource (255 chars max)
    {
        // try fixed buffer first (to avoid wasting space in the heap)
        CHAR szTemp[256];
        int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
        int nLen = _LoadString(nID, szTemp, nCount);
        if (nCount - nLen > 2)
        {
            *this = szTemp;
            return (nLen > 0);
        }

        // try buffer size of 512, then larger size until entire string is retrieved
        int nSize = 256;
        do
        {
            nSize += 256;
            LPSTR lpstr = GetBuffer(nSize - 1);
            if(lpstr == NULL)
            {
                nLen = 0;
                break;
            }
            nLen = _LoadString(nID, lpstr, nSize);
        } while (nSize - nLen <= 2);
        ReleaseBuffer();

        return (nLen > 0);
    }

#ifndef _ATL_NO_COM
    // OLE BSTR support (use for OLE automation)
    BSTR AllocSysString() const
    {
	    int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, NULL, NULL);
	    BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	    if(bstr != NULL)
		    MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, bstr, nLen);
       return bstr;
    }

    BSTR SetSysString(BSTR* pbstr) const
    {
	    int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, NULL, NULL);
	    if(::SysReAllocStringLen(pbstr, NULL, nLen))
		    MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, *pbstr, nLen);
	    ATLASSERT(*pbstr != NULL);
	    return *pbstr;
    }
#endif //!_ATL_NO_COM

    // Access to string implementation buffer as "C" character array
    LPSTR GetBuffer(int nMinBufLength)
    {
        ATLASSERT(nMinBufLength >= 0);

        if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
        {
            // we have to grow the buffer
            CStringDataA* pOldData = GetData();
            int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
            if (nMinBufLength < nOldLen)
                nMinBufLength = nOldLen;

            if(!AllocBuffer(nMinBufLength))
                return NULL;

            memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(CHAR));
            GetData()->nDataLength = nOldLen;
            CStringA::Release(pOldData);
        }
        ATLASSERT(GetData()->nRefs <= 1);

        // return a pointer to the character storage for this string
        ATLASSERT(m_pchData != NULL);
        return m_pchData;
    }

    void ReleaseBuffer(int nNewLength = -1)
    {
        CopyBeforeWrite();  // just in case GetBuffer was not called

        if (nNewLength == -1)
            nNewLength = lstrlenA(m_pchData); // zero terminated

        ATLASSERT(nNewLength <= GetData()->nAllocLength);
        GetData()->nDataLength = nNewLength;
        m_pchData[nNewLength] = '\0';
    }

    LPSTR GetBufferSetLength(int nNewLength)
    {
        ATLASSERT(nNewLength >= 0);

        if(GetBuffer(nNewLength) == NULL)
            return NULL;

        GetData()->nDataLength = nNewLength;
        m_pchData[nNewLength] = '\0';
        return m_pchData;
    }

    void FreeExtra()
    {
        ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
        if (GetData()->nDataLength != GetData()->nAllocLength)
        {
            CStringDataA* pOldData = GetData();
            if(AllocBuffer(GetData()->nDataLength))
            {
                memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(CHAR));
                ATLASSERT(m_pchData[GetData()->nDataLength] == '\0');
                CStringA::Release(pOldData);
            }
        }
        ATLASSERT(GetData() != NULL);
    }

    // Use LockBuffer/UnlockBuffer to turn refcounting off
    LPSTR LockBuffer()
    {
        LPSTR lpsz = GetBuffer(0);
        if(lpsz != NULL)
            GetData()->nRefs = -1;
        return lpsz;
    }

    void UnlockBuffer()
    {
        ATLASSERT(GetData()->nRefs == -1);
        if (GetData() != _atltmpDataNilA)
            GetData()->nRefs = 1;
    }

// Implementation
public:
    ~CStringA()
    {
        // free any attached data
        if (GetData() != _atltmpDataNilA)
        {
            if (InterlockedDecrement(&GetData()->nRefs) <= 0)
                delete[] (BYTE*)GetData();
        }
    }

    int GetAllocLength() const
        { return GetData()->nAllocLength; }

    static BOOL __stdcall _IsValidString(LPCWSTR lpsz, int nLength = -1)
    {
        if(lpsz == NULL)
            return FALSE;
        return !::IsBadStringPtrW(lpsz, nLength);
    }

    static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
    {
        if(lpsz == NULL)
            return FALSE;
        return !::IsBadStringPtrA(lpsz, nLength);
    }

protected:
    LPSTR m_pchData;   // pointer to ref counted string data

    // implementation helpers
    CStringDataA* GetData() const
        { ATLASSERT(m_pchData != NULL); return ((CStringDataA*)m_pchData) - 1; }

    void Init()
        { m_pchData = _GetEmptyString().m_pchData; }

    void AllocCopy(CStringA& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
    {
        // will clone the data attached to this string
        // allocating 'nExtraLen' characters
        // Places results in uninitialized string 'dest'
        // Will copy the part or all of original data to start of new string

        int nNewLen = nCopyLen + nExtraLen;
        if (nNewLen == 0)
        {
            dest.Init();
        }
        else
        {
            if(dest.AllocBuffer(nNewLen))
                memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(CHAR));
        }
    }


    BOOL AllocBuffer(int nLen)
    {
        // always allocate one extra character for '\0' termination
        // assumes [optimistically] that data length will equal allocation length

        ATLASSERT(nLen >= 0);
        ATLASSERT(nLen <= INT_MAX - 1);    // max size (enough room for 1 extra)

        if (nLen == 0)
        {
            Init();
        }
        else
        {
            CStringDataA* pData = NULL;
            ATLTRY(pData = (CStringDataA*)new BYTE[sizeof(CStringDataA) + (nLen + 1) * sizeof(CHAR)]);
            if(pData == NULL)
                return FALSE;

            pData->nRefs = 1;
            pData->data()[nLen] = '\0';
            pData->nDataLength = nLen;
            pData->nAllocLength = nLen;
            m_pchData = pData->data();
        }

        return TRUE;
    }

    void AssignCopy(int nSrcLen, LPCSTR lpszSrcData)
    {
        if(AllocBeforeWrite(nSrcLen))
        {
            memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(CHAR));
            GetData()->nDataLength = nSrcLen;
            m_pchData[nSrcLen] = '\0';
        }
    }

    
    BOOL ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len, LPCSTR lpszSrc2Data)
    {
      // -- master concatenation routine
      // Concatenate two sources
      // -- assume that 'this' is a new CString object

        BOOL bRet = TRUE;
        int nNewLen = nSrc1Len + nSrc2Len;
        if (nNewLen != 0)
        {
            bRet = AllocBuffer(nNewLen);
            if (bRet)
            {
                memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(CHAR));
                memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(CHAR));
            }
        }
        return bRet;
    }

    void ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData)
    {
        //  -- the main routine for += operators

        // concatenating an empty string is a no-op!
        if (nSrcLen == 0)
            return;

        // if the buffer is too small, or we have a width mis-match, just
        //   allocate a new buffer (slow but sure)
        if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
        {
            // we have to grow the buffer, use the ConcatCopy routine
            CStringDataA* pOldData = GetData();
            if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
            {
                ATLASSERT(pOldData != NULL);
                CStringA::Release(pOldData);
            }
        }
        else
        {
            // fast concatenation when buffer big enough
            memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(CHAR));
            GetData()->nDataLength += nSrcLen;
            ATLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
            m_pchData[GetData()->nDataLength] = '\0';
        }
    }

    void CopyBeforeWrite()
    {
        if (GetData()->nRefs > 1)
        {
            CStringDataA* pData = GetData();
            Release();
            if(AllocBuffer(pData->nDataLength))
                memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(CHAR));
        }
        ATLASSERT(GetData()->nRefs <= 1);
    }

    BOOL AllocBeforeWrite(int nLen)
    {
        BOOL bRet = TRUE;
        if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
        {
            Release();
            bRet = AllocBuffer(nLen);
        }
        ATLASSERT(GetData()->nRefs <= 1);
        return bRet;
    }

    void Release()
    {
        if (GetData() != _atltmpDataNilA)
        {
            ATLASSERT(GetData()->nRefs != 0);
            if (InterlockedDecrement(&GetData()->nRefs) <= 0)
                delete[] (BYTE*)GetData();
            Init();
        }
    }

    static void PASCAL Release(CStringDataA* pData)
    {
        if (pData != _atltmpDataNilA)
        {
            ATLASSERT(pData->nRefs != 0);
            if (InterlockedDecrement(&pData->nRefs) <= 0)
                delete[] (BYTE*)pData;
        }
    }
    
    static int PASCAL SafeStrlen(LPCSTR lpsz)
        { return (lpsz == NULL) ? 0 : lstrlenA(lpsz); }

    static int __stdcall _LoadString(UINT nID, LPSTR lpszBuf, UINT nMaxBuf)
    {
#ifdef _DEBUG
        // LoadString without annoying warning from the Debug kernel if the
        //  segment containing the string is not present
        if (::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE((nID>>4) + 1), RT_STRING) == NULL)
        {
            lpszBuf[0] = '\0';
            return 0; // not found
        }
#endif //_DEBUG
        int nLen = ::LoadStringA(AfxGetResourceHandle(), nID, lpszBuf, nMaxBuf);
        if (nLen == 0)
            lpszBuf[0] = '\0';
        return nLen;
    }

    static const CStringA& __stdcall _GetEmptyString()
        { return *(CStringA*)&_atltmpPchNilA; }

// CStringA conversion helpers
    static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL)
            return 0;

        int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
        ATLASSERT(mbstr == NULL || result <= (int)count);
        if (result > 0)
            mbstr[result - 1] = 0;
        return result;
    }

    static int __cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
    {
        if (count == 0 && wcstr != NULL)
            return 0;

        int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
        ATLASSERT(wcstr == NULL || result <= (int)count);
        if (result > 0)
            wcstr[result - 1] = 0;
        return result;
    }

// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
    static CHAR* _cstrchr(const CHAR* p, CHAR ch)
    {
        //strchr for '\0' should succeed
        while (*p != 0)
        {
            if (*p == ch)
                break;
            p = ::CharNextA(p);
        }
        return (CHAR*)((*p == ch) ? p : NULL);
    }
    
    static CHAR* _cstrchr_db(const CHAR* p, CHAR ch1, CHAR ch2)
    {
        const CHAR* lpsz = NULL;
        while (*p != 0)
        {
            if (*p == ch1 && *(p + 1) == ch2)
            {
                lpsz = p;
                break;
            }
            p = ::CharNextA(p);
        }
        return (CHAR*)lpsz;
    }
    
    static CHAR* _cstrrchr(const CHAR* p, CHAR ch)
    {
        const CHAR* lpsz = NULL;
        while (*p != 0)
        {
            if (*p == ch)
                lpsz = p;
            p = ::CharNextA(p);
        }
        return (CHAR*)lpsz;
    }
    
    static CHAR* _cstrrev(CHAR* pStr)
    {
        // Optimize NULL, zero-length, and single-char case.
        if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
            return pStr;

        CHAR* p = pStr;

        while (p[1] != 0) 
        {
            CHAR* pNext = ::CharNextA(p);
            if(pNext > p + 1)
            {
                char p1 = *(char*)p;
                *(char*)p = *(char*)(p + 1);
                *(char*)(p + 1) = p1;
            }
            p = pNext;
        }

        CHAR* q = pStr;

        while (q < p)
        {
            CHAR t = *q;
            *q = *p;
            *p = t;
            q++;
            p--;
        }
        return (CHAR*)pStr;
    }
    
    static CHAR* _cstrstr(const CHAR* pStr, const CHAR* pCharSet)
    {
        int nLen = lstrlenA(pCharSet);
        if (nLen == 0)
            return (CHAR*)pStr;

        const CHAR* pRet = NULL;
        const CHAR* pCur = pStr;
        while((pStr = _cstrchr(pCur, *pCharSet)) != NULL)
        {
            if(memcmp(pCur, pCharSet, nLen * sizeof(CHAR)) == 0)
            {
                pRet = pCur;
                break;
            }
            pCur = ::CharNextA(pCur);
        }
        return (CHAR*) pRet;
    }
    
    static int _cstrspn(const CHAR* pStr, const CHAR* pCharSet)
    {
        int nRet = 0;
        CHAR* p = (CHAR*)pStr;
        while (*p != 0)
        {
            CHAR* pNext = ::CharNextA(p);
            if(pNext > p + 1)
            {
                if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
                    break;
                nRet += 2;
            }
            else
            {
                if(_cstrchr(pCharSet, *p) == NULL)
                    break;
                nRet++;
            }
            p = pNext;
        }
        return nRet;
    }
    
    static int _cstrcspn(const CHAR* pStr, const CHAR* pCharSet)
    {
        int nRet = 0;
        CHAR* p = (CHAR*)pStr;
        while (*p != 0)
        {
            CHAR* pNext = ::CharNextA(p);
            if(pNext > p + 1)
            {
                if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
                    break;
                nRet += 2;
            }
            else
            {
                if(_cstrchr(pCharSet, *p) != NULL)
                    break;
                nRet++;
            }
            p = pNext;
        }
        return nRet;
    }
    
    static CHAR* _cstrpbrk(const CHAR* p, const CHAR* lpszCharSet)
    {
        while (*p != 0)
        {
            if (_cstrchr(lpszCharSet, *p) != NULL)
            {
                return (CHAR*)p;
                break;
            }
            p = ::CharNextA(p);
        }
        return NULL;
    }
    
    static int _cstrisdigit(CHAR ch)
    {
        WORD type;
        GetStringTypeExA(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_DIGIT) == C1_DIGIT;
    }
    
    static int _cstrisspace(CHAR ch)
    {
        WORD type;
        GetStringTypeExA(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_SPACE) == C1_SPACE;
    }
    
    static int _cstrcmp(const CHAR* pstrOne, const CHAR* pstrOther)
        { return lstrcmpA(pstrOne, pstrOther); }
    
    static int _cstrcmpi(const CHAR* pstrOne, const CHAR* pstrOther)
        { return lstrcmpiA(pstrOne, pstrOther); }
    
    static int _cstrcoll(const CHAR* pstrOne, const CHAR* pstrOther)
    {
        int nRet = CompareStringA(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
        ATLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    
    static int _cstrcolli(const CHAR* pstrOne, const CHAR* pstrOther)
    {
        int nRet = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
        ATLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    
    static int _cstrtoi(const CHAR* nptr)
    {
        int c;              /* current char */
        int total;          /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        while ( _cstrisspace(*nptr) )
            ++nptr;

        c = (int)(_TUCHAR)*nptr++;
        sign = c;           /* save sign indication */
        if (c == _T('-') || c == _T('+'))
            c = (int)(_TUCHAR)*nptr++;    /* skip sign */

        total = 0;

        while (_cstrisdigit((CHAR)c)) {
            total = 10 * total + (c - '0');     /* accumulate digit */
            c = (int)(_TUCHAR)*nptr++;    /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
    }

#else //!_ATL_MIN_CRT

    static CHAR* _cstrchr(const CHAR* p, CHAR ch)
        { return strchr(p, ch); }

    static CHAR* _cstrrchr(const CHAR* p, CHAR ch)
        { return strrchr(p, ch); }

    static CHAR* _cstrrev(CHAR* pStr)
        { return _strrev(pStr); }

    static CHAR* _cstrstr(const CHAR* pStr, const CHAR* pCharSet)
        { return strstr(pStr, pCharSet); }

    static int _cstrspn(const CHAR* pStr, const CHAR* pCharSet)
        { return (int)strspn(pStr, pCharSet); }

    static int _cstrcspn(const CHAR* pStr, const CHAR* pCharSet)
        { return (int)strcspn(pStr, pCharSet); }

    static CHAR* _cstrpbrk(const CHAR* p, const CHAR* lpszCharSet)
        { return strpbrk(p, lpszCharSet); }

    static int _cstrisdigit(CHAR ch)
        { return isdigit(ch); }

    static int _cstrisspace(CHAR ch)
        { return isspace(ch); }

    static int _cstrcmp(const CHAR* pstrOne, const CHAR* pstrOther)
        { return strcmp(pstrOne, pstrOther); }

    static int _cstrcmpi(const CHAR* pstrOne, const CHAR* pstrOther)
        { return _stricmp(pstrOne, pstrOther); }

    static int _cstrcoll(const CHAR* pstrOne, const CHAR* pstrOther)
        { return strcoll(pstrOne, pstrOther); }

    static int _cstrcolli(const CHAR* pstrOne, const CHAR* pstrOther)
        { return _stricoll(pstrOne, pstrOther); }

    static int _cstrtoi(const CHAR* nptr)
        { return atoi(nptr); }

#endif //!_ATL_MIN_CRT

};

// Compare helpers
inline bool __stdcall operator==(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) == 0; }
inline bool __stdcall operator!=(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) != 0; }
inline bool __stdcall operator<(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) > 0; }
inline bool __stdcall operator>(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) < 0; }
inline bool __stdcall operator<=(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) >= 0; }
inline bool __stdcall operator>=(const CStringA& s1, const CStringA& s2)
    { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const CStringA& s1, LPCSTR s2)
    { return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(LPCSTR s1, const CStringA& s2)
    { return s2.Compare(s1) <= 0; }

// friend functions
inline CStringA __stdcall operator+(const CStringA& string1, const CStringA& string2)
{
    CStringA s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
    return s;
}

inline CStringA __stdcall operator+(const CStringA& string1, char ch)
{
    CStringA s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
    return s;
}

inline CStringA __stdcall operator+(const CStringA& string1, WCHAR wch)
{
    CStringA s;
    CStringA ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, ch);
    return s;
}

inline CStringA __stdcall operator+(WCHAR wch, const CStringA& string1)
{
    CStringA s;
    CStringA ch((LPCWSTR)&wch, 1);
    s.ConcatCopy(1, ch, string1.GetData()->nDataLength, string1.m_pchData);
    return s;
}

inline CStringA __stdcall operator+(char ch, const CStringA& string1)
{
    CStringA s;
    s.ConcatCopy(1, &ch, string1.GetData()->nDataLength, string1.m_pchData);
    return s;
}


#endif  // WTL_CSTRING_A_H__INCLUDED
