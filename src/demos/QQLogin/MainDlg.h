// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "..\controls.extend\STurn3DView.h"
using namespace SOUI;
class CMainDlg : public SHostWnd
{
public:
    CMainDlg();
    ~CMainDlg();

    void OnClose()
    {
        AnimateHostWindow(200, AW_CENTER | AW_HIDE);
        DestroyWindow();
    }
    void OnMaximize()
    {
        SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
    }
    void OnRestore()
    {
        SendMessage(WM_SYSCOMMAND, SC_RESTORE);
    }
    void OnMinimize()
    {
        SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
    }

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

    void OnBtnSetting();
    void OnBtnSettingOK();
    void OnBtnSettingCancel();
    BOOL OnTurn3D(EventArgs *pEvt);
    BOOL OnUserChanged(EventArgs *pEvt);
    void OnBtnLogin();

    int  GetReturnCode() { return m_nRetCode; }

protected:

    EVENT_MAP_DECL()

    BEGIN_MSG_MAP_EX_DECL()
private:
    BOOL			m_bLayoutInited;
    int             m_nRetCode;
};
