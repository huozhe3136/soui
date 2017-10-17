﻿
// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <core/Swnd.h>
#include <control/SCmnCtrl.h>
#include "DlgCreatePro.h"
#include "DesignerView.h"
#include "SMoveWnd.h"
#include "propgrid/SPropertyGrid.h"
#include "STabCtrl2.h"
#include "SToolbox.h"
#include "SListBoxDrop.h"
#include "trayicon/SShellNotifyIcon.h"

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

	void OnBtnOpen(); //打开工程

	void OnBtnSaveAll();
	void OnBtnSaveLayout(); //保存布局

	void OnBtnNewDialog(); //新建Dialog
	void OnBtnNewInclude(); //新建Include

	void OnBtnWndLayout();
	void OnbtnPreview();

	void OnBtnZYGL();
	void OnBtnYSGL();
	void OnbtnAbout();

	bool OnTreeItemDbClick(EventArgs *pEvtBase);

	bool OnLbControlSelChanged(EventArgs *pEvtBase); //控件列表事件
    bool OnDesinerTabSelChanged(EventArgs *pEvtBase);
	bool OnRightTabSelChanged(EventArgs *pEvtBase);

	//托盘通知消息处理函数
	LRESULT OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
	//演示如何响应菜单事件
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnShowWindow(BOOL bShow, UINT nStatus);
	void test();


protected:

	    void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	//soui消息
	EVENT_MAP_DECL()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX_DECL()
private:
	BOOL			m_bLayoutInited;
	SShellNotifyIcon shellNotifyIcon;

public:
	void OpenProject(SStringT strFileName);

public:
	SButton* btn_new;
	STreeCtrl * m_treePro;
	SButton* btn_save;

	//SList<SDesignerView*> m_ViewList;
	SDesignerView *m_pDesignerView;

	SWindow *m_pLayoutContainer;

	//加载控件列表
	pugi::xml_document xmlDocCtrl;
	SMap<SStringT, pugi::xml_node> m_mapCtrlList;

	SListBoxDrop *m_lbControl;  //控件列表框
	STreeCtrl *m_treeXmlStruct;  //控件结构


	SPropertyGrid *m_pPropgrid;

	//SMap<SStringT, pugi::xml_node> m_mapCommonProperty; //属性通用样式列表   <"skin", xmlnode> <"pos", xmlNode>

	//SMap<SStringT, pugi::xml_node> m_mapProperty;  //控件属性列表 <"button",xmlnode>  <"checkbox",xmlnode>

	SStringT m_strProPath;
	STabCtrl *m_pPropertyPage;  
	SWindow* m_wndPropContainer;  //属性控件的容器

	SRichEdit* m_edtDesc; //属性描述

	SRealWnd *m_RealWnd;

	STabCtrl *m_tabDesigner;
	STabCtrl *m_tabRight;

	SToolbox *m_btnPreview;
	SScrollView *m_scrView;
	BOOL m_bIsOpen;  //工程是否打开

};
