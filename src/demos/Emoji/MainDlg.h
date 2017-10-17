// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "FaceList_soui.h"
#define	FACE_CTRL_SEL  WM_USER + 1001

namespace SOUI
{
	enum
	{
		WM_SELECT_EMOJI = WM_USER + 100,
		WM_HOVER_EMOJI = WM_USER + 101,
		WM_LEAVE_EMOJI = WM_USER + 102,
	};
	class CEmojiDlg : public SHostWnd
	{
	public:
		CEmojiDlg();
		~CEmojiDlg();

		void OnClose();
		void OnMaximize();
		void OnRestore();
		void OnMinimize();
		void OnBtnSet();
		void OnDelete();
		void OnBtnSelectAll();
		void UnSelectAll();

		void OnBtnMsgBox();
		int OnCreate(LPCREATESTRUCT lpCreateStruct);
		BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
		void SetFaceList(CFaceList_SOUI* lpSysFaceList, CFaceList_SOUI* lpMyFaceList, HWND hParentWnd, SStringT szMyPath = _T(""));


	protected:
		//soui消息
		EVENT_MAP_DECL() 

		//HostWnd真实窗口消息处理
		BEGIN_MSG_MAP_EX_DECL() 

		void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
		// 添加表情
		void OnAddEmoji();
		// 管理表情
		void OnManageEmoji();
		// 退出管理
		void OnBtnQuitManage();
		LRESULT OnSelectEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnHoverEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnLeaveEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
		bool GetManageStatus();
		bool OnEmojiPageChanging(EventArgs* pEvt);
		void OnLButtonDown(UINT nFlags, CPoint point);
	private:
		BOOL			m_bLayoutInited;
		CFaceList_SOUI*		m_pSystemFaceList;
		CFaceList_SOUI*		m_pMyFaceList;
		bool     m_bManaging;
		bool     m_bPopSetMenu;
		HWND     m_hParenWND;
		SStringT m_MyFacePath;
	public:
		SStringT m_strSelectEmoji;
	};

}
