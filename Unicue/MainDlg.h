﻿/************************************************************************/
/*                                                                      */
/* Unicue 1.4                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once
// maindlg.h
#ifndef MAINDLG_H_
#define MAINDLG_H_

#include <unordered_map>
#include "../common/cmdline.h"
#include "config.h"
#include "colorhyperlink.h"

// The global instance for config
extern CConfig _Config;
extern CommandLine *_CommandLine;
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>
{
protected:
    BOOL            m_bNeedConvert;            // m_RawString need to be converted or not
    char*           m_RawString;               // Raw string read from file, with BOM if it is existing
    UINT            m_RawStringLength;         // Length of m_RawString
    char*           m_String;                  // String without BOM, contained in m_RawString
    UINT            m_StringLength;            // Length of m_String, without length of BOM
    ATL::CString    m_StringCodeType;          // Encoding type.
    wchar_t*        m_UnicodeString;           // Unicode String to save converted result
    UINT            m_UnicodeLength;           // Length of m_UnicodeString
    ATL::CString    m_FilePathName;            // File path to be converted
    ATL::CString    m_CodeStatus;              // Encoding check result
    BOOL            m_bCueFile;                // Type of file to be converted is a cue
    BOOL            m_bTransferString;         // Transfer string mode if TRUE, Transfer file mode if FALSE
    CC4Context*     m_context;                 // Converting context
    CMenu           m_popupMenu;               // Popup menu
    CColorHyperLink m_fileLink;                // Hyperlink for file
    std::unordered_map<int, RECT> m_itemRects; // Orignal size of dialog items
    RECT            m_dlgRect;                 // Orinal rect of dialog

    void FixCue();
    void FixInternalCue(const ATL::CString& AudioFileName);
    void FixTTACue();
    void SetMainWndPos();
    BOOL DealFile();
    BOOL ExtractTakInternalCue(const ATL::CString& AudioFileName);
    BOOL ExtractFlacInternalCue(const ATL::CString& AudioFileName);

public:
    enum { IDD = IDD_MAINDLG };
    CMainDlg();
    ~CMainDlg();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainDlg)
        CHAIN_MSG_MAP(CUpdateUI<CMainDlg>)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
        MESSAGE_HANDLER(WM_SIZE, onDialogResize)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER(IDM_UTF_8_WITH_BOM, OnPopupUTF8)
        COMMAND_ID_HANDLER(IDM_UTF_8_WITHOUT_BOM, OnPopupUTF8NoBom)
        COMMAND_ID_HANDLER(IDM_UTF_16_LITTLE_ENDIAN, OnPopupUTF16LE)
        COMMAND_ID_HANDLER(IDM_UTF_16_BIG_ENDIAN, OnPopupUTF16BE)
        COMMAND_HANDLER(IDC_CHECK_AUTOCHECKCODE, BN_CLICKED, OnBnClickedCheckAutocheckcode)
        COMMAND_HANDLER(IDC_CHECK_ALWAYSONTOP, BN_CLICKED, OnBnClickedCheckAlwaysontop)
        COMMAND_HANDLER(IDC_COMBO_SELECTCODE, CBN_SELCHANGE, OnCbnSelchangeComboSelectcode)
        COMMAND_HANDLER(IDC_BUTTON_DO, BN_CLICKED, OnBnClickedButtonDo)
        COMMAND_HANDLER(IDC_BUTTON_SELECTSAVECODE, BN_CLICKED, OnBnClickedButtonSelectOutputCode);
        COMMAND_HANDLER(IDC_BUTTON_SAVE, BN_CLICKED, OnBnClickedButtonSave)
        COMMAND_HANDLER(IDC_BUTTON_SAVEAS, BN_CLICKED, OnBnClickedButtonSaveas)
        COMMAND_HANDLER(IDC_BUTTON_TRANSFERSTRING, BN_CLICKED, OnBnClickedButtonTransferstring)
        NOTIFY_HANDLER(IDC_STATIC_FILELINK, NM_CLICK, OnClickFileLink)
    END_MSG_MAP()

    // DDX
    BEGIN_DDX_MAP(CMainDlg)
        DDX_CHECK(IDC_CHECK_AUTOCHECKCODE, _Config.AutoCheckCode)
        DDX_CHECK(IDC_CHECK_ALWAYSONTOP, _Config.AlwaysOnTop)
    END_DDX_MAP()

// Handler prototypes (uncomment arguments if needed):
//    LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//    LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//    LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnPopupUTF8(WORD, WORD, HWND, BOOL&);
    LRESULT OnPopupUTF8NoBom(WORD, WORD, HWND, BOOL&);
    LRESULT OnPopupUTF16LE(WORD, WORD, HWND, BOOL&);
    LRESULT OnPopupUTF16BE(WORD, WORD, HWND, BOOL&);
    LRESULT OnDropFiles(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT onDialogResize(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonDo(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedCheckAutocheckcode(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedCheckAlwaysontop(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonTransferstring(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSelectOutputCode(WORD, WORD, HWND, BOOL&);
    LRESULT OnClickFileLink(int, LPNMHDR, BOOL&);

    void CloseDialog();
    void OpenFile(LPCWSTR filePath);
    void SaveFile(LPCWSTR filePath);

private:
    void getDlgItemsRelativePosition();
    void moveItem(int itemId, int deltaX, int deltaY);
    void resizeItem(int itemId, int deltaX, int deltaY);
};

#endif // MAINDLG_H_
