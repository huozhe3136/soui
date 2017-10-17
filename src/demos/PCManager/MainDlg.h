// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
using namespace SOUI;

class CMainDlg : public SHostWnd
{
	friend class SWindow;
public:
	
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnTimer(char cTimerID);
	void OnTimer2(UINT_PTR idEvent);

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
protected:
private:
	void Home_OnRadioCmd0(); //Tool工具区Radio
	void Home_OnRadioCmd1();
	void Home_OnRadioCmd2();

	void Home_SwitchRadio(BYTE trId);
	void Home_MoveRadio();
protected:
	//soui消息
	EVENT_MAP_DECL()
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()

	SOUI_MSG_MAP_DECL()
	
private:
	BOOL			m_bLayoutInited;
	BYTE			m_nowHomeRadio, m_tarHomeRadio; //当前和目标Radio状态
	static const char  m_HomeTimerID = 100;		
};
