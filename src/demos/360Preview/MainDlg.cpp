// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "MainDlg.h"
#include "SImageSwitcher.h"

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
using namespace SOUI;

EVENT_MAP_BEGIN(CMainDlg)
	EVENT_NAME_COMMAND(L"btn_close", OnClose)
	EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
	EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
	EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
	EVENT_NAME_COMMAND(L"btn_desktop", OnDesktop)
	EVENT_NAME_COMMAND(L"btn_ucenter", OnUcenter)
	EVENT_NAME_COMMAND(L"btn_guard", OnGuard)
	EVENT_NAME_COMMAND(L"btn_diagnosescan", OnDiagnosescan)
EVENT_MAP_END()

//HostWnd真实窗口消息处理
BEGIN_MSG_MAP_EX(CMainDlg)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_CLOSE(OnClose)
	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP()

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 		MARGINS mar = {5,5,30,5};
	// 		DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnDesktop()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(0);
}
void CMainDlg::OnUcenter()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(1);
}
void CMainDlg::OnGuard()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(2);
}
void CMainDlg::OnDiagnosescan()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(3);
}