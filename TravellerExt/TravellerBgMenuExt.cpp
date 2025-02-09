﻿/************************************************************************/
/*                                                                      */
/* Traveller 1.2                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "TravellerBgMenuExt.h"

STDMETHODIMP CTravellerBgMenuExt::Initialize(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDO, HKEY hkeyProgID)
{
    return SHGetPathFromIDList(pIDFolder, m_szPathName) ? S_OK : E_INVALIDARG;
}

STDMETHODIMP CTravellerBgMenuExt::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

    InsertMenu(hMenu, indexMenu, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, indexMenu+1, MF_BYPOSITION, idCmdFirst, _T("Unicue Traveller"));
    InsertMenu(hMenu, indexMenu+2, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 1);
}

STDMETHODIMP CTravellerBgMenuExt::InvokeCommand(LPCMINVOKECOMMANDINFO pInfo)
{
    if ( 0 != HIWORD(pInfo->lpVerb))    // 如果lpVerb指向字符串，则忽略此次调用并退出
        return E_FAIL;
    
    if (0 != LOWORD( pInfo->lpVerb))    // 菜单项的ID必须为0，因为只添加了一个菜单项
        return E_FAIL;
    ATL::CString folder(_T("\""));
    folder += m_szPathName;
    folder += _T("\"");
    ShellExecute(NULL, _T("open"), m_travellerPath, folder, NULL, SW_SHOWNORMAL);
    return S_OK;
}

STDMETHODIMP CTravellerBgMenuExt::GetCommandString(UINT_PTR uCmd, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    static CHAR szHelpTextA[] = "Unicue Traveller Extension";
    static WCHAR szHelpTextW[] = L"Unicue Traveller Extension";

    if (uCmd != 0)
        return E_INVALIDARG;

    if ( uType & GCS_HELPTEXT )
    {        
        if ( uType & GCS_UNICODE )
            lstrcpynW((LPWSTR)pszName, szHelpTextW, cchMax);
        else
            lstrcpynA(pszName, szHelpTextA, cchMax);
    }

    return S_OK;
}
