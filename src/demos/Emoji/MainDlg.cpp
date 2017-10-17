// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "MainDlg.h"
#include <helper/SMenuEx.h>
#include <helper/SAdapterBase.h>
#include "FileHelper.h"
#include "gif/SMyGifPlayer.h"
#include "SMyImage.h"

#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif

namespace SOUI
{
	EVENT_MAP_BEGIN(CEmojiDlg)
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btnset", OnBtnSet)
		EVENT_NAME_COMMAND(L"btnDel", OnDelete)
		EVENT_NAME_COMMAND(L"quitManage", OnBtnQuitManage)
		EVENT_NAME_COMMAND(L"btnSelectAll", OnBtnSelectAll)
	EVENT_MAP_END()

		//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CEmojiDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_COMMAND(OnCommand)
		MSG_WM_ACTIVATE(OnActivate)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MESSAGE_HANDLER_EX(WM_SELECT_EMOJI, OnSelectEmoji)
		MESSAGE_HANDLER_EX(WM_HOVER_EMOJI, OnHoverEmoji)
		MESSAGE_HANDLER_EX(WM_LEAVE_EMOJI, OnLeaveEmoji)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	class CEmojiTileAdapter : public SAdapterBase
	{
	public:
		CEmojiTileAdapter(CFaceList_SOUI* pFaceList, int iBegin, int iEnd, HWND hwnd) :
			m_pFaceList(pFaceList),
			m_iBegin(iBegin),
			m_iEnd(iEnd),
			m_hwnd(hwnd)
		{
		}
		virtual int getCount()
		{
			return m_iEnd - m_iBegin + 1;
		}

		virtual void getView(int position, SWindow *pItem, pugi::xml_node xmlTemplate)
		{
			if (pItem->GetChildrenCount() == 0)
			{
				pItem->InitFromXml(xmlTemplate);
			}
			SMyImage *pImg = pItem->FindChildByName2<SMyImage>(L"img_file_img");
			CFaceInfo_SOUI* info = m_pFaceList->GetFaceInfoById(position + m_iBegin);
			if (info)
			{
				SStringT path = info->m_strFileName;
				pItem->SetAttribute(L"margin", L"0,0,1,1");
				pItem->SetAttribute(L"colorBorder", L"#dfe6f6");
				pImg->SetImage(info->m_pBitmap, kMedium_FilterLevel);
				pImg->GetRoot()->SetUserData(position);
				pItem->SetAttribute(L"tip", S_CT2W(info->m_strTip));
				if (info->m_nId >= 1000)
				{
					SImageWnd* pSelectd = pItem->FindChildByName2<SImageWnd>(L"selectd");
					if (info->m_bSelectd)
					{
						pSelectd->SetAttribute(L"show", L"1");
					}
					else
					{
						pSelectd->SetAttribute(L"show", L"0");
					}
				}
				pItem->GetEventSet()->subscribeEvent(EventItemPanelHover::EventID, Subscriber(&CEmojiTileAdapter::OnButtonHover, this));
				pItem->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CEmojiTileAdapter::OnButtonClick, this));
				pItem->GetEventSet()->subscribeEvent(EventItemPanelLeave::EventID, Subscriber(&CEmojiTileAdapter::OnButtonLeave, this));
			}
		}

		bool OnButtonHover(EventArgs* pEvt)
		{
			SWindow* pWND = sobj_cast<SWindow>(pEvt->sender);
			pWND->SetAttribute(L"margin", L"1,1,1,1");
			pWND->SetAttribute(L"colorBorder", L"#188ce0");
			int index = pWND->GetRoot()->GetUserData();
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(m_hwnd, &pt);
			CRect rect;
			GetWindowRect(m_hwnd, rect);
			bool bLeft;
			if (pt.x <= rect.Width() / 2)
			{
				bLeft = false;
			}
			else
			{
				bLeft = true;
			}
			SStringT mm;
			mm.Format(_T("OnButtonHover: id: %d pt: %d,%d %s\n"), index, pt.x, pt.y, bLeft ? _T("left") : _T("right"));
			OutputDebugString(mm);
			::SendMessage(m_hwnd, WM_HOVER_EMOJI, (WPARAM)bLeft, (LPARAM)index + m_iBegin);
			return true;
		}

		bool OnButtonClick(EventArgs *pEvt)
		{
			SWindow* pWnd = sobj_cast<SWindow>(pEvt->sender);
			int index = pWnd->GetRoot()->GetUserData();
			::SendMessage(m_hwnd, WM_SELECT_EMOJI, (WPARAM)index + m_iBegin, 0);
			return true;
		}

		bool OnButtonLeave(EventArgs* pEvt)
		{
			SWindow* pWND = sobj_cast<SWindow>(pEvt->sender);
			pWND->SetAttribute(L"margin", L"0,0,1,1");
			pWND->SetAttribute(L"colorBorder", L"#dfe6f6");
			int index = pWND->GetRoot()->GetUserData();
			SStringT mm;
			mm.Format(_T("OnButtonLeave: id: %d\n"), index);
			OutputDebugString(mm);
			::SendMessage(m_hwnd, WM_LEAVE_EMOJI, 0, 0);
			return true;
		}
		void ChangeFace()
		{
			m_iEnd = m_pFaceList->m_arrFaceInfo.size() + 999;
			notifyDataSetChanged();
		}

	private:
		CFaceList_SOUI* m_pFaceList;
		int        m_iBegin;
		int        m_iEnd;
		HWND       m_hwnd;
	};

	CEmojiDlg::CEmojiDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
	{
		m_bLayoutInited = FALSE;
		m_bManaging = false;
		m_bPopSetMenu = false;
	}

	CEmojiDlg::~CEmojiDlg()
	{
	}

	int CEmojiDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
#ifdef DWMBLUR	//win7毛玻璃开关
		MARGINS mar = { 5,5,30,5 };
		DwmExtendFrameIntoClientArea(m_hWnd, &mar);
#endif

		SetMsgHandled(FALSE);
		return 0;
	}

	BOOL CEmojiDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
	{
		m_bLayoutInited = TRUE;
		//隐藏任务栏图标
		//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
		STileView *pTileEmoji1 = FindChildByName2<STileView>("tile_emoji1");
		if (pTileEmoji1)
		{
			CEmojiTileAdapter *pAdapter1 = new CEmojiTileAdapter(m_pSystemFaceList, 58, 74, m_hWnd);
			pTileEmoji1->SetAdapter(pAdapter1);
			pAdapter1->Release();
		}
		STileView *pTileEmoji2 = FindChildByName2<STileView>("tile_emoji2");
		if (pTileEmoji2)
		{
			CEmojiTileAdapter *pAdapter2 = new CEmojiTileAdapter(m_pSystemFaceList, 40, 57, m_hWnd);
			pTileEmoji2->SetAdapter(pAdapter2);
			pAdapter2->Release();
		}
		STileView *pTileEmoji3 = FindChildByName2<STileView>("tile_emoji3");
		if (pTileEmoji3)
		{
			CEmojiTileAdapter *pAdapter3 = new CEmojiTileAdapter(m_pSystemFaceList, 75, 90, m_hWnd);
			pTileEmoji3->SetAdapter(pAdapter3);
			pAdapter3->Release();
		}
		STileView *pTileEmoji4 = FindChildByName2<STileView>("tile_emoji4");
		if (pTileEmoji4)
		{
			CEmojiTileAdapter *pAdapter4 = new CEmojiTileAdapter(m_pSystemFaceList, 0, 39, m_hWnd);
			pTileEmoji4->SetAdapter(pAdapter4);
			pAdapter4->Release();
		}
		STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
		if (pTileEmoji5)
		{
			CEmojiTileAdapter *pAdapter5 = new CEmojiTileAdapter(m_pMyFaceList, 1000, m_pMyFaceList->m_arrFaceInfo.size() + 1000 - 1, m_hWnd);
			pTileEmoji5->SetAdapter(pAdapter5);
			pAdapter5->Release();
		}

		STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_emoji");
		pTabCtrl->GetEventSet()->subscribeEvent(EventTabSelChanging::EventID, Subscriber(&CEmojiDlg::OnEmojiPageChanging, this));
		return 0;
	}


	void CEmojiDlg::SetFaceList(CFaceList_SOUI* lpSysFaceList, CFaceList_SOUI* lpMyFaceList, HWND hParentWnd, SStringT szMyPath /*= _T("")*/)
	{
		m_pSystemFaceList = lpSysFaceList;
		m_pMyFaceList = lpMyFaceList;
		m_hParenWND = hParentWnd;
		m_MyFacePath = szMyPath;
	}

	void CEmojiDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
	{
		if (uNotifyCode == 0)
		{
			if (nID == 1)
			{
				// 添加表情
				OnAddEmoji();
			}
			else if (nID == 2)
			{
				// 管理表情
				OnManageEmoji();
			}
			else if (nID == 8)
			{
				PostQuitMessage(0);
			}
		}
	}

	void CEmojiDlg::OnAddEmoji()
	{
		CFileDialogEx dlg(TRUE, NULL, 0, 0,
			_T("图片文件(*.gif;*.bmp;*.jpg;*.png)\0*.gif;*.bmp;*.jpg;*.png\0")
			_T("动态图片(*.gif)\0*.gif\0")
			_T("静态图片(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0")
		);

		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		TCHAR szPath[MAX_PATH];
		TCHAR szFileName[80 * MAX_PATH];

		TCHAR* p;
		int nLen = 0;

		std::vector<SStringT> vecFilePath;
		//把第一个文件名前的复制到szPath,即:  
		//如果只选了一个文件,就复制到最后一个'/'  
		//如果选了多个文件,就复制到第一个NULL字符  
		lstrcpyn(szPath, dlg.m_szFileName, dlg.m_ofn.nFileOffset);

		//当只选了一个文件时,下面这个NULL字符是必需的.  
		//这里不区别对待选了一个和多个文件的情况  

		szPath[dlg.m_ofn.nFileOffset] = '\0';
		nLen = lstrlen(szPath);

		if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'//'  
		{
			lstrcat(szPath, TEXT("\\"));
		}


		p = dlg.m_szFileName + dlg.m_ofn.nFileOffset; //把指针移到第一个文件  
		memset(szFileName, 0, sizeof(szFileName));

		while (*p)
		{
			lstrcpy(szFileName, szPath);  //给文件名加上路径    
			lstrcat(szFileName, p);    //加上文件名 
			vecFilePath.push_back(szFileName);
			lstrcat(szFileName, TEXT("\0")); //换行     
			p += lstrlen(p) + 1;     //移至下一个文件  
		}

		std::vector<SStringT>::iterator it = vecFilePath.begin();
		if (_taccess(_T("Resource\\MyFace"), 0) != 0)
		{
			CreateDirectory(_T("Resource\\MyFace"), NULL);
		}

		std::vector<SStringT> vecNewPath;
		while (it != vecFilePath.end())
		{
			if ((*it).Right(4).CompareNoCase(_T(".lnk")) == 0)
			{
				++it;
				continue;
			}

			SStringT path;
			path.Format(_T("%s\\%s"), m_MyFacePath, m_pMyFaceList->GetFileName(*it));
			CopyFile(*it, path, TRUE);
			path.Format(_T("%s"), m_pMyFaceList->GetFileName(*it));
			vecNewPath.push_back(path);
			++it;
		}

		m_pMyFaceList->AddEmoji(vecNewPath, m_MyFacePath);

		STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
		if (pTileEmoji5)
		{
			CEmojiTileAdapter *pAdapter = (CEmojiTileAdapter*)pTileEmoji5->GetAdapter();
			pAdapter->ChangeFace();
		}

		STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_emoji");
		if (!pTabCtrl)
			return;

		pTabCtrl->SetCurSel(4);
	}

	void CEmojiDlg::OnManageEmoji()
	{
		STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_emoji");
		SWindow* pWindow = FindChildByName2<SWindow>(L"managerWnd");
		SCaption* pTextmyFavorite = FindChildByName2<SCaption>(L"myFavorite");
		if (!pTabCtrl || !pWindow || !pTextmyFavorite)
			return;

		pTextmyFavorite->SetVisible(FALSE, TRUE);
		pWindow->SetVisible(TRUE, TRUE);
		pTabCtrl->SetCurSel(4);
		m_bManaging = true;
	}

	void CEmojiDlg::OnBtnQuitManage()
	{
		STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_emoji");
		SWindow* pWindow = FindChildByName2<SWindow>(L"managerWnd");
		SCaption* pTextmyFavorite = FindChildByName2<SCaption>(L"myFavorite");
		if (!pTabCtrl || !pWindow || !pTextmyFavorite)
			return;

		UnSelectAll();

		pTextmyFavorite->SetVisible(TRUE, TRUE);
		pWindow->SetVisible(FALSE, TRUE);
		m_bManaging = false;
	}

	LRESULT CEmojiDlg::OnSelectEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SetMsgHandled(FALSE);
		if (lParam == 0)
		{
			int index = (int)wParam;
			if (index < 1000)
				m_strSelectEmoji = m_pSystemFaceList->GetFaceInfoById(index)->m_strTip;
			else
				m_strSelectEmoji = m_pMyFaceList->GetFaceInfoById(index)->m_strFileName;

			if (!GetManageStatus())
			{
				::PostMessage(m_hParenWND, FACE_CTRL_SEL, 1, index);
				PostMessage(WM_CLOSE);
				return 0;
			}

			if (index >= 1000)
			{
				m_pMyFaceList->GetFaceInfoById(index)->ChangeSelectStatus();
								
				STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
				if (pTileEmoji5)
				{
					CEmojiTileAdapter *pAdapter = (CEmojiTileAdapter*)pTileEmoji5->GetAdapter();
					pAdapter->ChangeFace();
				}
			}
		}
		return 0;
	}

	LRESULT CEmojiDlg::OnHoverEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SetMsgHandled(FALSE);
		if (GetManageStatus())
			return 0;

		bool bLeft = wParam != 0;
		int index = (int)lParam;

		SWindow* pWnd = FindChildByName2<SWindow>(L"play");
		pWnd->SetAttribute(L"show", L"1");
		if (bLeft)
		{
			pWnd->SetAttribute(L"pos", L"4,30");
			pWnd->SetAttribute(L"offset", L"0,0");
		}
		else
		{
			pWnd->SetAttribute(L"pos", L"-5,30");
			pWnd->SetAttribute(L"offset", L"-1,0");
		}

		SMyGifPlayer* gifplay = pWnd->FindChildByName2<SMyGifPlayer>(L"gifplayer");

		CFaceInfo_SOUI* info;
		SStringT path;
		if (index < 1000)
		{
			info = m_pSystemFaceList->GetFaceInfoById(index);
			path = SApplication::getSingleton().GetAppDir() + _T("\\Resource\\") + info->m_strFileName;
		}
		else
		{
			info = m_pMyFaceList->GetFaceInfoById(index);
			path = m_MyFacePath + _T("\\") + info->m_strFileName;
		}
		gifplay->SetAttribute(L"show", L"1");
		gifplay->PlayGifFile(path);
		return 0;
	}

	LRESULT CEmojiDlg::OnLeaveEmoji(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SetMsgHandled(FALSE);
		SWindow* pWnd = FindChildByName2<SWindow>(L"play");
		pWnd->SetAttribute(L"show", L"0");
		return 0;
	}

	void CEmojiDlg::OnActivate(UINT nState, BOOL bMinimized, HWND wndOther)
	{
		SetMsgHandled(FALSE);
		// 失活后关闭窗口
		//if (nState == WA_INACTIVE && !m_bPopSetMenu && !GetManageStatus())
		//{
		//	PostMessage(WM_CLOSE);
		//}
	}

	bool CEmojiDlg::GetManageStatus()
	{
		return m_bManaging;
	}

	bool CEmojiDlg::OnEmojiPageChanging(EventArgs* pEvt)
	{
		m_bPopSetMenu = false;
		EventTabSelChanging* evt = (EventTabSelChanging*)pEvt;
		if (evt->uOldSel == 4 && evt->uOldSel != evt->uNewSel)
		{
			SWindow* pWindow = FindChildByName2<SWindow>(L"managerWnd");
			SCaption* pTextmyFavorite = FindChildByName2<SCaption>(L"myFavorite");

			pTextmyFavorite->SetVisible(TRUE, TRUE);
			pWindow->SetVisible(FALSE, TRUE);
			m_bManaging = false;
			UnSelectAll();
		}
		return true;
	}

	void CEmojiDlg::OnLButtonDown(UINT nFlags, CPoint point)
	{
		m_bPopSetMenu = false;
		SetMsgHandled(FALSE);
	}

	//TODO:消息映射
	void CEmojiDlg::OnClose()
	{
		//::PostMessage(m_hParenWND, FACE_CTRL_SEL, NULL, NULL);
		CSimpleWnd::DestroyWindow();
	}

	void CEmojiDlg::OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	void CEmojiDlg::OnRestore()
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	void CEmojiDlg::OnMinimize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}

	void CEmojiDlg::OnBtnSet()
	{
		m_bPopSetMenu = true;
		SImageButton* pSettingBtn = FindChildByName2<SImageButton>(L"btnset");
		if (!pSettingBtn)
			return;

		CRect rect = pSettingBtn->GetWindowRect();
		ClientToScreen(rect);
		//使用模拟菜单
		SMenuEx menu;
		menu.LoadMenu(_T("smenuex:menuex_setting"));
		menu.TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTALIGN, rect.left, rect.top - 1, m_hWnd);
	}

	void CEmojiDlg::OnDelete()
	{
		std::vector<int> vecDel;
		m_pMyFaceList->GetDelList(vecDel);
		if (vecDel.empty())
		{
			SMessageBox(m_hWnd, _T("您还没有选择表情？"), _T("提示"), MB_OK | MB_ICONWARNING);
			return;
		}

		if (IDOK != SMessageBox(m_hWnd, _T("确定删除选择的表情吗？"), _T("提示"), MB_OKCANCEL | MB_ICONWARNING))
			return;

		m_pMyFaceList->RemoveFaceVec(vecDel);
		
		STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
		if (pTileEmoji5)
		{
			CEmojiTileAdapter *pAdapter = (CEmojiTileAdapter*)pTileEmoji5->GetAdapter();
			pAdapter->ChangeFace();
		}
	}

	void CEmojiDlg::OnBtnSelectAll()
	{
		std::vector<CFaceInfo_SOUI *>::iterator it = m_pMyFaceList->m_arrFaceInfo.begin();
		while (it != m_pMyFaceList->m_arrFaceInfo.end())
		{
			(*it)->m_bSelectd = true;
			++it;
		}
		STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
		if (pTileEmoji5)
		{
			CEmojiTileAdapter *pAdapter = (CEmojiTileAdapter*)pTileEmoji5->GetAdapter();
			pAdapter->ChangeFace();
		}
	}

	void CEmojiDlg::UnSelectAll()
	{
		std::vector<CFaceInfo_SOUI *>::iterator it = m_pMyFaceList->m_arrFaceInfo.begin();
		while (it != m_pMyFaceList->m_arrFaceInfo.end())
		{
			(*it)->m_bSelectd = false;
			++it;
		}
		STileView *pTileEmoji5 = FindChildByName2<STileView>("tile_emoji5");
		if (pTileEmoji5)
		{
			CEmojiTileAdapter *pAdapter = (CEmojiTileAdapter*)pTileEmoji5->GetAdapter();
			pAdapter->ChangeFace();
		}
	}

}