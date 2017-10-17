// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SwndFrame.h"
#include "SCaptureButton.h"
using namespace SOUI;

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose()
	{
		AnimateHostWindow(200,AW_CENTER|AW_HIDE);
        DestroyWindow();
	}
	void OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
	}
	void OnRestore()
	{
		SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	}
	void OnMinimize()
	{
		SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	}
    
    void OnBtnSearch();
protected:
	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);
		if(!m_bLayoutInited) return;
		if(nType==SIZE_MAXIMIZED)
		{
			FindChildByName(L"btn_restore")->SetVisible(TRUE);
			FindChildByName(L"btn_max")->SetVisible(FALSE);
		}else if(nType==SIZE_RESTORED)
		{
            FindChildByName(L"btn_restore")->SetVisible(FALSE);
            FindChildByName(L"btn_max")->SetVisible(TRUE);
		}
	}
        
	void OnShowWindow(BOOL bShow, UINT nStatus);
    BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

    BOOL OnEventCaptureHost(EventArgs *pEvt);
    BOOL OnEventCaptureHostFinish(EventArgs *pEvt);

    BOOL OnEventCaptureSwnd(EventArgs *pEvt);
    BOOL OnEventCaptureSwndFinish(EventArgs *pEvt);
    
    BOOL OnEventTreeSelChanged(EventArgs *pEvt);
    
    BOOL OnCopyData(HWND hSend, PCOPYDATASTRUCT pCopyDataStruct);

    void EnumSwnd(HWND hHost ,DWORD dwSwnd, HSTREEITEM hItem);
protected:

	EVENT_MAP_DECL()

	BEGIN_MSG_MAP_EX_DECL()
private:
	BOOL			m_bLayoutInited;

    STreeCtrl     * m_pTreeCtrl;

    SwndFrame       m_wndFrame;
};
