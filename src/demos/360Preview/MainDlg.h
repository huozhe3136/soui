// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once


/**
by 多点免费WIFI
 */

class CMainDlg : public SOUI::SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnDesktop();
	void OnUcenter();
	void OnGuard();
	void OnDiagnosescan();

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);


protected:
	//soui事件处理映射表
	EVENT_MAP_DECL() 
	
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
private:
	BOOL			m_bLayoutInited;
};
