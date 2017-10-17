// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "res/R.h"

using namespace SOUI;


class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	
	void OnBtnMenu();
	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:
	virtual HRESULT OnLanguageChanged();

	void OnBtnScale(int nID);
	void OnLanguage(int nID);
	void OnMenuCmd(UINT uNotifyCode, int nID, HWND wndCtl);

	void InitListView();

	//soui消息
	EVENT_MAP_DECL() 
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
private:
	int	m_LangID;
};
