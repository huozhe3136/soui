#ifndef _MAINFRAME_DLG_H
#define _MAINFRAME_DLG_H
    
#include "ui/adapter/FirendListItemAdapter.h"
#include "ui/adapter/IListItemCallBack.h"
 
#include "res/R.h"
#pragma once 


class CMainDlg : public SOUI::SHostWnd, 
	public live::IListItemCallback 
{ 
	enum EListType
	{
		EFriendList=0, 
		ENewChannel, 
		EHotChannel,
	};
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, SOUI::CSize size);
	void OnLoginButtonClick();

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	 
	BOOL OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct);


	bool OnJoinChannel();
	bool OnQuitChannel();
	 

	int  GetReturnCode() { return m_nRetCode; }
protected:    

	//soui消息
	EVENT_MAP_DECL() 

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
	 
private:  
	//implement IListItemCallback
	virtual void OnItemDBClick(int data, int position, void *pData);
	 
	void OnSwitchFriends();
	void OnSwitchHotChannel();
	void OnSwitchNewChannel(); 
	void OnClickAvatar();

	void SyncFriendList(); //获取好友列表 
	void GetHotChannelList(); //获取热门频道列表
	void GetNewChannelList();//获取最新频道列表 
private:  
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl); 
private:  
	SOUI::SWindow *m_p_tab_friends;
	SOUI::SWindow *m_p_tab_hotchannel;
	SOUI::SWindow *m_p_tab_newchannel; 

	SOUI::STabCtrl *m_p_TabMain;


	SOUI::SStatic *m_p_Title;

	SOUI::SMCListView  *m_lv_friends;
	SOUI::SMCListView  *m_lv_hot_channel;
	SOUI::SMCListView  *m_lv_new_channel;

    SOUI::SEdit    *m_pSearchInput;
    BOOL            m_bLayoutInited;

	int             m_nRetCode;  
};

#endif //_MAINFRAME_DLG_H