﻿/************************************************************************/
/*                                                                      */
/* Traveller 1.4                                                        */
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
#include "resource.h"
#include "MainDlg.h"
#include "SettingDlg.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "..\common\winfile.h"

CMainDlg::CMainDlg()
    : m_configPath(L"")
{
    m_configPath += Unicue::GetProcessFolder();
    m_configPath += L"config-traveller.xml";
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    SetDefault(m_config);
    // load config file
    CWinFile file(m_configPath, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
        SaveConfigFile(m_configPath, m_config);
    else
    {
        UINT fileLength = file.length();
        char *fileBuffer = new char[fileLength+1];
        memset((void*)fileBuffer, 0, fileLength+1);
        file.seek(0, CWinFile::begin);
        file.read(fileBuffer, fileLength);
        file.close();

        TiXmlDocument *doc = new TiXmlDocument;
        doc->Parse(fileBuffer, NULL, TIXML_ENCODING_UTF8);
        if (doc->Error() || !LoadConfigFile(doc, m_config))
        {
            ::DeleteFile(m_configPath);
            SetDefault(m_config);
            SaveConfigFile(m_configPath, m_config);
        }

        delete []fileBuffer;
        delete doc;
    }
    return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    SaveConfigFile(m_configPath, m_config);
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedRegister(WORD, WORD, HWND, BOOL&)
{
    // copy dll file
    if (GetFileAttributes(_T("TravellerExt.dll")) == INVALID_FILE_ATTRIBUTES)
    {
        ATL::CString dllPath(Unicue::GetProcessFolder());
        if (Unicue::IsWow64())
            dllPath += _T("TravellerExt64.dll");
        else
            dllPath += _T("TravellerExt32.dll");

        ATL::CString target(Unicue::GetProcessFolder());
        target += _T("TravellerExt.dll");
        bool success = CWinFile::CopyFile(dllPath, target);
        if (!success)
        {
            ATL::CString errorMessage(L"Failed to copy ");
            errorMessage += dllPath;
            errorMessage += L" to ";
            errorMessage += target;
            errorMessage += L"!";
            MessageBox(errorMessage);

            return 0;
        }
        
        /*
        if (IsWow64())
            ShellExecute(NULL, _T("open"), _T("cmd"), _T("/c copy /Y TravellerExt64.dll TravellerExt.dll"), Unicue::GetProcessFolder(), SW_HIDE);
        else
            ShellExecute(NULL, _T("open"), _T("cmd"), _T("/c copy /Y TravellerExt32.dll TravellerExt.dll"), Unicue::GetProcessFolder(), SW_HIDE);
        */
    }

    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    ATL::CString dll(L"\"");
    dll += Unicue::GetProcessFolder();
    dll += _T("TravellerExt.dll\"");

    ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);

    return 0;
}

LRESULT CMainDlg::OnBnClickedUnregister(WORD, WORD, HWND, BOOL&)
{
    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    ATL::CString dll(L"/u \"");
    dll += Unicue::GetProcessFolder();
    dll += _T("TravellerExt.dll\"");

    ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);

    return 0;
}

LRESULT CMainDlg::OnBnClickedSetting(WORD, WORD, HWND, BOOL&)
{
    CSettingDlg dlg(m_config);
    if (dlg.DoModal() == IDOK)
    {
        m_config = dlg.m_config;
    }
    return 0;
}
