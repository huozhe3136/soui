﻿// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "MainDlg.h"
#include "helper/SMenu.h"
#include "../controls.extend/FileHelper.h"
#include "../controls.extend/SChatEdit.h"
#include "../controls.extend/reole/richeditole.h"
#include "TipWnd.h"
#include "FormatMsgDlg.h"
#include <math.h>
#include <helper/SAdapterBase.h>
#include <helper/SMenuEx.h>
#include <helper/SDibHelper.h>
#include "SRatingBar.h"
#include "SWkeWebkit.h"
#include "gif/SGifPlayer.h"
#include "SInterpolatorView.h"
#include "SPathView.h"

#include "skin/SSkinLoader.h"
//#define SHOW_AERO //open aero for vista and win7

#define TIMER_QUIT 1000

#ifdef SHOW_AERO
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif

using namespace SOUI;
#include <shellapi.h>
#include "skin/SDemoSkin.h"
#include "skin/SetSkinWnd2.h"
#include "../controls.extend/SMcListViewEx/SMCListViewEx.h"
#include "adapter.h"
#include "trayicon/SShellNotifyIcon.h"
class CTestDropTarget:public IDropTarget
{
public:
    CTestDropTarget()
    {
        nRef=0;
    }
    
    virtual ~CTestDropTarget(){}
    
    //////////////////////////////////////////////////////////////////////////
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
    {
        HRESULT hr=S_FALSE;
        if(riid==__uuidof(IUnknown))
            *ppvObject=(IUnknown*) this,hr=S_OK;
        else if(riid==__uuidof(IDropTarget))
            *ppvObject=(IDropTarget*)this,hr=S_OK;
        if(SUCCEEDED(hr)) AddRef();
        return hr;

    }

    virtual ULONG STDMETHODCALLTYPE AddRef( void){return ++nRef;}

    virtual ULONG STDMETHODCALLTYPE Release( void) { 
        ULONG uRet= -- nRef;
        if(uRet==0) delete this;
        return uRet;
    }

    //////////////////////////////////////////////////////////////////////////
    // IDropTarget

    virtual HRESULT STDMETHODCALLTYPE DragEnter( 
        /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
    {
        *pdwEffect=DROPEFFECT_LINK;
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE DragOver( 
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
    {
        *pdwEffect=DROPEFFECT_LINK;
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE DragLeave( void)
    {
        return S_OK;
    }


protected:
    int nRef;
};

class CTestDropTarget1 : public CTestDropTarget
{
protected:
    SWindow *m_pEdit;
public:
    CTestDropTarget1(SWindow *pEdit):m_pEdit(pEdit)
    {
        if(m_pEdit) m_pEdit->AddRef();
    }
    ~CTestDropTarget1()
    {
        if(m_pEdit) m_pEdit->Release();
    }
public:
    virtual HRESULT STDMETHODCALLTYPE Drop( 
        /* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ __RPC__inout DWORD *pdwEffect)
    {
        FORMATETC format =
        {
            CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL
        };
        STGMEDIUM medium;
        if(FAILED(pDataObj->GetData(&format, &medium)))
        {
            return S_FALSE;
        }

        HDROP hdrop = static_cast<HDROP>(GlobalLock(medium.hGlobal));

        if(!hdrop)
        {
            return S_FALSE;
        }

        bool success = false;
        TCHAR filename[MAX_PATH];
        success=!!DragQueryFile(hdrop, 0, filename, MAX_PATH);
        DragFinish(hdrop);
        GlobalUnlock(medium.hGlobal);

        if(success && m_pEdit)
        {
            m_pEdit->SetWindowText(filename);
        }

        *pdwEffect=DROPEFFECT_LINK;
        return S_OK;
    }
};



class CTestAdapterFix : public SAdapterBase
{
    int * m_pCbxSel;
public:
    
    CTestAdapterFix()
    {
        m_pCbxSel = new int[getCount()];
        memset(m_pCbxSel,0,sizeof(int)*getCount());
    }
    
    ~CTestAdapterFix()
    {
        delete []m_pCbxSel;        
    }
    
    virtual int getCount()
    {
        return 50000;
    }   

    virtual void getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate)
    {
        if(pItem->GetChildrenCount()==0)
        {
            pItem->InitFromXml(xmlTemplate);
        }

        SButton *pBtn = pItem->FindChildByName2<SButton>(L"btn_test");
        pBtn->SetWindowText(SStringT().Format(_T("button %d"),position));
        pBtn->GetRoot()->SetUserData(position);

        //由网友“从未来过” 修改的新事件订阅方式,采用模板函数从响应函数中自动提取事件类型，2016.12.13
        pBtn->GetEventSet()->subscribeEvent(&CTestAdapterFix::OnButtonClick,this);

        SComboBox * pCbx = pItem->FindChildByName2<SComboBox>(L"cbx_in_lv");
        if(pCbx)
        {
            pCbx->SetCurSel(m_pCbxSel[position]);
            pCbx->GetEventSet()->subscribeEvent(&CTestAdapterFix::OnCbxSelChange,this);
        }
    }
    
    bool OnCbxSelChange(EventCBSelChange *pEvt)
    {
        SComboBox *pCbx = sobj_cast<SComboBox>(pEvt->sender);
        int iItem = pCbx->GetRoot()->GetUserData();
        m_pCbxSel[iItem] = pCbx->GetCurSel();
        return true;
    }
    
    bool OnButtonClick(EventCmd *pEvt)
    {
        SButton *pBtn = sobj_cast<SButton>(pEvt->sender);
        int iItem = pBtn->GetRoot()->GetUserData();
        SMessageBox(NULL,SStringT().Format(_T("button of %d item was clicked"),iItem),_T("haha"),MB_OK);
        return true;
    }
};


const wchar_t * KAttrName_Height[] = {
    L"oddHeight",
    L"evenHeight",
    L"evenSelHeight"
};

const wchar_t* KNodeName_Item[] = {
    L"itemOdd",
    L"itemEven",
    L"itemEvenHover"
};
class CTestAdapterFlex : public SAdapterBase
{
public:
    int m_nItemHeight[3];
    
    
    CTestAdapterFlex()
    {

    }
    
    virtual void InitByTemplate(pugi::xml_node xmlTemplate)
    {
        m_nItemHeight[0] = xmlTemplate.attribute(KAttrName_Height[0]).as_int(50);
        m_nItemHeight[1] = xmlTemplate.attribute(KAttrName_Height[1]).as_int(60);
        m_nItemHeight[2] = xmlTemplate.attribute(KAttrName_Height[2]).as_int(70);
    }
    
    virtual int getCount()
    {
        return 12340;
    }   

    virtual int getViewTypeCount(){return 3;}
    
    virtual int getItemViewType(int position,DWORD dwState)
    {
        if(position%2 == 0) 
            return 0;//1,3,5,... odd lines
        else if(dwState & WndState_Hover)
            return 2;//even lines with check state
        else
            return 1;//even lines 
    }
    
    virtual SIZE getViewDesiredSize(int position,SWindow *pItem, LPCRECT prcContainer)
    {
        DWORD dwState = pItem->GetState();
        int viewType = getItemViewType(position,dwState);
        return CSize(0,m_nItemHeight[viewType]);//cx在listview，mclistview中没有使用，不需要计算
    }
    
    virtual void getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate)
    {
        if(pItem->GetChildrenCount()==0)
        {
            int nViewType = getItemViewType(position,pItem->GetState());
            pItem->InitFromXml(xmlTemplate.child(KNodeName_Item[nViewType]));
        }
        pItem->GetEventSet()->subscribeEvent(EventSwndStateChanged::EventID,Subscriber(&CTestAdapterFlex::OnItemStateChanged,this));
        
        SButton *pBtn = pItem->FindChildByName2<SButton>(L"btn_test");
        pBtn->SetWindowText(SStringT().Format(_T("button %d"),position));
        pBtn->SetUserData(position);
        pBtn->GetEventSet()->subscribeEvent(EVT_CMD,Subscriber(&CTestAdapterFlex::OnButtonClick,this));
    }
    
    bool OnItemStateChanged(EventArgs *e)
    {
        EventSwndStateChanged *e2 = sobj_cast<EventSwndStateChanged>(e);
        if(!e2->CheckState(WndState_Hover)) return false;
        //通知界面重绘
        notifyDataSetInvalidated();
        return true;
    }
    
    bool OnButtonClick(EventArgs *pEvt)
    {
        SButton *pBtn = sobj_cast<SButton>(pEvt->sender);
        int iItem = pBtn->GetUserData();
        SMessageBox(NULL,SStringT().Format(_T("button of %d item was clicked"),iItem),_T("haha"),MB_OK);
        return true;
    }
    
};

class CTestMcAdapterFix : public SMcAdapterBase
{
#define NUMSCALE 100000
public:
    struct SOFTINFO
    {
        const wchar_t * pszSkinName;
		const wchar_t * pszName;
		const wchar_t * pszDesc;
        float     fScore;
        DWORD     dwSize;
		const wchar_t * pszInstallTime;
		const wchar_t * pszUseTime;
    };

    SArray<SOFTINFO> m_softInfo;

  bool IsColumnVisible(int iCol) const override
 {
	return true;
 }

public:
    CTestMcAdapterFix()
    {
        SOFTINFO info[] =
        {
            {
                L"skin_icon1",
                L"鲁大师",
                L"鲁大师是一款专业的硬件检测，驱动安装工具",
                5.4f,
                15*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon2",
                L"PhotoShop",
                L"强大的图片处理工具",
                9.0f,
                150*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon3",
                L"QQ7.0",
                L"腾讯公司出品的即时聊天工具",
                8.0f,
                40*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon4",
                L"Visual Studio 2008",
                L"Microsoft公司的程序开发套件",
                9.0f,
                40*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon5",
                L"YY8",
                L"YY语音",
                9.0f,
                20*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon6",
                L"火狐浏览器",
                L"速度最快的浏览器",
                8.5f,
                35*(1<<20),
                L"2015-8-5",
                L"今天"
            },
            {
                L"skin_icon7",
                L"迅雷",
                L"迅雷下载软件",
                7.3f,
                17*(1<<20),
                L"2015-8-5",
                L"今天"
            }
        };


        for(int i=0;i<ARRAYSIZE(info);i++)
        {
            m_softInfo.Add(info[i]);
        }
    }

    virtual int getCount()
    {
        return m_softInfo.GetCount()*NUMSCALE;
    }   

    SStringT getSizeText(DWORD dwSize)
    {
        int num1=dwSize/(1<<20);
        dwSize -= num1 *(1<<20);
        int num2 = dwSize*100/(1<<20);
        return SStringT().Format(_T("%d.%02dM"),num1,num2);
    }
    
    virtual void getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate)
    {
        if(pItem->GetChildrenCount()==0)
        {
            pItem->InitFromXml(xmlTemplate);
        }
        
        SOFTINFO *psi =m_softInfo.GetData()+position%m_softInfo.GetCount();
        pItem->FindChildByName(L"img_icon")->SetAttribute(L"skin",psi->pszSkinName);
        pItem->FindChildByName(L"txt_name")->SetWindowText(S_CW2T(psi->pszName));
        pItem->FindChildByName(L"txt_desc")->SetWindowText(S_CW2T(psi->pszDesc));
        pItem->FindChildByName(L"txt_score")->SetWindowText(SStringT().Format(_T("%1.2f 分"),psi->fScore));
        pItem->FindChildByName(L"txt_installtime")->SetWindowText(S_CW2T(psi->pszInstallTime));
        pItem->FindChildByName(L"txt_usetime")->SetWindowText(S_CW2T(psi->pszUseTime));
        pItem->FindChildByName(L"txt_size")->SetWindowText(getSizeText(psi->dwSize));
        pItem->FindChildByName2<SRatingBar>(L"rating_score")->SetValue(psi->fScore/2);
        pItem->FindChildByName(L"txt_index")->SetWindowText(SStringT().Format(_T("第%d行"),position+1));
        
        SButton *pBtnUninstall = pItem->FindChildByName2<SButton>(L"btn_uninstall");
        pBtnUninstall->SetUserData(position);
        pBtnUninstall->GetEventSet()->subscribeEvent(EVT_CMD,Subscriber(&CTestMcAdapterFix::OnButtonClick,this));
    }

    bool OnButtonClick(EventArgs *pEvt)
    {
        SButton *pBtn = sobj_cast<SButton>(pEvt->sender);
        int iItem = pBtn->GetUserData();
        
        if(SMessageBox(NULL,SStringT().Format(_T("Are you sure to uninstall the selected [%d] software?"),iItem),_T("uninstall"),MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
        {//删除一条记录
            DeleteItem(iItem);
        }
        return true;
    }
    
    //删除一行，提供外部调用。
    void DeleteItem(int iPosition)
    {
        if(iPosition>=0 && iPosition<getCount())
        {
            int iItem = iPosition % m_softInfo.GetCount();
            m_softInfo.RemoveAt(iItem);
            notifyDataSetChanged();
        }
    }
    
    SStringW GetColumnName(int iCol) const{
        return SStringW().Format(L"col%d",iCol+1);
    }
    
    struct SORTCTX
    {
        int iCol;
        SHDSORTFLAG stFlag;
    };
    
    bool OnSort(int iCol,SHDSORTFLAG * stFlags,int nCols)
    {
        if(iCol==5) //最后一列“操作”不支持排序
            return false;
        
        SHDSORTFLAG stFlag = stFlags[iCol];
        switch(stFlag)
        {
            case ST_NULL:stFlag = ST_UP;break;
            case ST_DOWN:stFlag = ST_UP;break;
            case ST_UP:stFlag = ST_DOWN;break;
        }
        for(int i=0;i<nCols;i++)
        {
            stFlags[i]=ST_NULL;
        }
        stFlags[iCol]=stFlag;
        
        SORTCTX ctx={iCol,stFlag};
        qsort_s(m_softInfo.GetData(),m_softInfo.GetCount(),sizeof(SOFTINFO),SortCmp,&ctx);
        return true;
    }
    
    static int __cdecl SortCmp(void *context,const void * p1,const void * p2)
    {
        SORTCTX *pctx = (SORTCTX*)context;
        const SOFTINFO *pSI1=(const SOFTINFO*)p1;
        const SOFTINFO *pSI2=(const SOFTINFO*)p2;
        int nRet =0;
        switch(pctx->iCol)
        {
            case 0://name
                nRet = wcscmp(pSI1->pszName,pSI2->pszName);
                break;
            case 1://score
                {
                    float fCmp = (pSI1->fScore - pSI2->fScore);
                    if(fabs(fCmp)<0.0000001) nRet = 0;
                    else if(fCmp>0.0f) nRet = 1;
                    else nRet = -1;
                }
                break;
            case 2://size
                nRet = (int)(pSI1->dwSize - pSI2->dwSize);
                break;
            case 3://install time
                nRet = wcscmp(pSI1->pszInstallTime,pSI2->pszInstallTime);
                break;
            case 4://user time
                nRet = wcscmp(pSI1->pszUseTime,pSI2->pszUseTime);
                break;

        }
        if(pctx->stFlag == ST_UP)
            nRet = -nRet;
        return nRet;
    }
};



SStringW skins[5] = {
L"skin_icon1",
L"skin_icon2",
L"skin_icon3",
L"skin_icon4",
L"skin_icon5"
};

class CTestTileAdapter : public SAdapterBase
{
public:
    CTestTileAdapter()
    {

    }
    virtual int getCount()
    {
        return 50000;
    }

    virtual void getView(int position, SWindow *pItem, pugi::xml_node xmlTemplate)
    {
        if(pItem->GetChildrenCount() == 0)
        {
            pItem->InitFromXml(xmlTemplate);
        }
        SImageWnd *pImg = pItem->FindChildByName2<SImageWnd>(L"img_file_icon");
        pImg->SetSkin(GETSKIN(skins[position % 5],pImg->GetScale()));
        SButton *pBtn = pItem->FindChildByName2<SButton>(L"btn_test");
        pBtn->SetWindowText(SStringT().Format(_T("btn %d"), position));
        pBtn->GetRoot()->SetUserData(position);
        pBtn->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CTestTileAdapter::OnButtonClick, this));
    }

    bool OnButtonClick(EventArgs *pEvt)
    {
        SButton *pBtn = sobj_cast<SButton>(pEvt->sender);
        int iItem = pBtn->GetRoot()->GetUserData();
        SMessageBox(NULL, SStringT().Format(_T("button of %d item was clicked"), iItem), _T("haha"), MB_OK);
        return true;
    }

};


struct TreeItemData
{
    SStringW strName;
    int      nAge;
    SStringT strTstLong;
};

class CTreeViewAdapter :public STreeAdapterBase<TreeItemData>
{
public:
    
	CTreeViewAdapter() {
	    TreeItemData data;
	    data.strName = L"name root";
	    data.nAge = 100;
	    
	    HSTREEITEM hRoot = InsertItem(data);
	    SetItemExpanded(hRoot,FALSE);
	    for(int i=0;i<100;i++)
	    {
	         data.strName.Format(L"branch_%d",i);
	         data.nAge ++;
	         data.strTstLong=_T("red text");
	         if(i==50) data.strTstLong = _T("Long Text Test. When this item is shown, the treeview size should be extended automatically.");
	         InsertItem(data,hRoot);
	    }
	    //ExpandItem(hRoot,ITvAdapter::TV_TOGGLE);
	}
	
	~CTreeViewAdapter() {}

	virtual void getView(SOUI::HTREEITEM loc, SWindow * pItem, pugi::xml_node xmlTemplate) {
		if(pItem->GetChildrenCount() == 0)
		{
			pItem->InitFromXml(xmlTemplate);
		}
		ItemInfo & ii = m_tree.GetItemRef((HSTREEITEM)loc);
		SWindow * pWnd = pItem->FindChildByID(R.id.btn_test);
		SASSERT(pWnd);
		pWnd->SetWindowText(S_CW2T(ii.data.strName));
		SWindow *pTxtRed = pItem->FindChildByID(R.id.txt_red);
		SASSERT(pTxtRed);
		pTxtRed->SetWindowText(ii.data.strTstLong);
		
		SToggle *pSwitch = pItem->FindChildByID2<SToggle>(R.id.tgl_switch);
		SASSERT(pSwitch);
		pSwitch->SetVisible(HasChildren(loc));
		pSwitch->SetToggle(IsItemExpanded(loc));
		pSwitch->GetEventSet()->subscribeEvent(EVT_CMD,Subscriber(&CTreeViewAdapter::OnSwitchClick,this));
	}
	
	bool OnSwitchClick(EventArgs *pEvt)
	{
	    SToggle *pToggle = sobj_cast<SToggle>(pEvt->sender);
	    SASSERT(pToggle);
	    SItemPanel *pItem = sobj_cast<SItemPanel>(pToggle->GetRoot());
	    SASSERT(pItem);
	    SOUI::HTREEITEM loc = (SOUI::HTREEITEM)pItem->GetItemIndex();
	    ExpandItem(loc,ITvAdapter::TVC_TOGGLE);
	    return true;
	}
};

SOUI_CLASS_NAME(EventThread, L"on_event_thread")
SOUI_CLASS_NAME(EventThreadStart, L"on_event_thread_start")
SOUI_CLASS_NAME(EventThreadStop, L"on_event_thread_stop")

EVENT_MAP_BEGIN(CMainDlg)
        EVENT_ID_COMMAND(1, OnClose)
        EVENT_ID_COMMAND(2, OnMaximize)
        EVENT_ID_COMMAND(3, OnRestore)
        EVENT_ID_COMMAND(5, OnMinimize)
        EVENT_ID_COMMAND(R.id.btn_tip,OnBtnTip)
        EVENT_NAME_CONTEXTMENU(L"edit_1140",OnEditMenu)
        EVENT_NAME_COMMAND(L"btn_msgbox",OnBtnMsgBox)
        
        //<--在新版本的uiresbuilder生成的resource.h中定义了R.id, R.name两个对象，可以使用如下方式来关联变量。
        EVENT_ID_COMMAND(R.id.btnSelectGif,OnBtnSelectGIF)
        EVENT_ID_RANGE_HANDLER(R.id.radio2_1,R.id.radio2_6,EventSwndStateChanged::EventID,OnTabPageRadioSwitch)    //10000-10005是XML中定义的radio2的ID
        EVENT_NAME_COMMAND(R.name.btn_menu,OnBtnMenu)
        EVENT_NAME_COMMAND(R.name.btn_webkit_go,OnBtnWebkitGo)
        EVENT_ID_COMMAND(R.id.btn_createchildren,OnBtnCreateChildren)
        EVENT_ID_COMMAND(R.id.btn_clock,OnBtnClock)
        EVENT_ID_COMMAND(R.id.btn_init_listbox,OnInitListBox)
        EVENT_ID_COMMAND(R.id.btn_skin,OnBtnSkin)
        EVENT_ID_COMMAND(R.id.btn_start_notify_thread,OnBtnStartNotifyThread)
        EVENT_ID_COMMAND(R.id.btn_stop_notify_thread,OnBtnStopNotifyThread)
        EVENT_ID_COMMAND(R.id.btn_open_wrap_content,OnBtnOpenWrapContent)
        EVENT_ID_HANDLER(R.id.cbx_interpolator,EventCBSelChange::EventID,OnCbxInterpolotorChange)
	    EVENT_HANDLER(EventPath::EventID, OnEventPath)
        //-->
        //<--通知中心事件
        EVENT_ID_HANDLER(SENDER_ID,EventThreadStart::EventID,OnEventThreadStart)
        EVENT_ID_HANDLER(SENDER_ID,EventThreadStop::EventID,OnEventThreadStop)
        EVENT_ID_HANDLER(SENDER_ID,EventThread::EventID,OnEventThread)
        //-->

        EVENT_NAME_COMMAND(L"btn_webkit_back",OnBtnWebkitBackward)
        EVENT_NAME_COMMAND(L"btn_webkit_fore",OnBtnWebkitForeward)
        EVENT_NAME_COMMAND(L"btn_webkit_refresh",OnBtnWebkitRefresh)
        EVENT_NAME_COMMAND(L"btn_hidetst",OnBtnHideTest)
        EVENT_NAME_COMMAND(L"btn_insert_gif",OnBtnInsertGif2RE)
        EVENT_NAME_COMMAND(L"btn_append_msg",OnBtnAppendMsg)
        EVENT_NAME_COMMAND(L"btn_richedit_save",OnBtnRtfSave)
        EVENT_NAME_COMMAND(L"btn_richedit_open",OnBtnRtfOpen)
        EVENT_NAME_COMMAND(L"btn_lrc",OnBtnLRC)
        EVENT_NAME_HANDLER(L"chromeTab",EVT_CHROMETAB_NEW,OnChromeTabNew)
        EVENT_NAME_COMMAND(L"btn_filewnd",OnBtnFileWnd)
        EVENT_NAME_HANDLER(L"edit_url",EVT_RE_NOTIFY,OnUrlReNotify)
        EVENT_NAME_HANDLER(L"mclv_test",EVT_CTXMENU,OnMclvCtxMenu)
        EVENT_NAME_HANDLER(L"edit_rotate",EVT_RE_NOTIFY,OnMatrixWindowReNotify)
        EVENT_NAME_HANDLER(L"edit_scale",EVT_RE_NOTIFY,OnMatrixWindowReNotify)
        EVENT_NAME_HANDLER(L"edit_skew",EVT_RE_NOTIFY,OnMatrixWindowReNotify)
        EVENT_NAME_HANDLER(L"edit_translate",EVT_RE_NOTIFY,OnMatrixWindowReNotify)
        
        EVENT_NAME_HANDLER(L"menu_slider",EventSliderPos::EventID,OnMenuSliderPos)
    EVENT_MAP_END() 

    //HOST消息及响应函数映射表
    BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY(OnDestory)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_SIZE(OnSize)
        MSG_WM_COMMAND(OnCommand)
        MSG_WM_TIMER(OnTimer)
        MESSAGE_HANDLER(g_dwSkinChangeMessage, OnSkinChangeMessage)
        CHAIN_MSG_MAP(SHostWnd)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

bool SMusicListAdapter::IsColumnVisible(int iCol) const
{
	return true;
}
int CMainDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef SHOW_AERO
    MARGINS mar = {5,5,30,5};
    DwmExtendFrameIntoClientArea ( m_hWnd, &mar );//打开这里可以启用Aero效果
#endif
	SetMsgHandled(FALSE);
	return 0;
}


struct student{
    TCHAR szName[100];
    TCHAR szSex[10];
    int age;
    int score;
};

//init listctrl
void CMainDlg::InitListCtrl()
{
    //找到列表控件
    SListCtrl *pList=FindChildByName2<SListCtrl>(L"lc_test");
    if(pList)
    {
        //列表控件的唯一子控件即为表头控件
        SWindow *pHeader=pList->GetWindow(GSW_FIRSTCHILD);
        //向表头控件订阅表明点击事件，并把它和OnListHeaderClick函数相连。
        pHeader->GetEventSet()->subscribeEvent(EVT_HEADER_CLICK,Subscriber(&CMainDlg::OnListHeaderClick,this));

        TCHAR szSex[][5]={_T("男"),_T("女"),_T("人妖")};
        for(int i=0;i<100;i++)
        {
            student *pst=new student;
            _stprintf(pst->szName,_T("学生_%d"),i+1);
            _tcscpy(pst->szSex,szSex[rand()%3]);
            pst->age=rand()%30;
            pst->score=rand()%60+40;

            int iItem=pList->InsertItem(i,pst->szName);
            pList->SetItemData(iItem,(DWORD)pst);
            pList->SetSubItemText(iItem,1,pst->szSex);
            TCHAR szBuf[10];
            _stprintf(szBuf,_T("%d"),pst->age);
            pList->SetSubItemText(iItem,2,szBuf);
            _stprintf(szBuf,_T("%d"),pst->score);
            pList->SetSubItemText(iItem,3,szBuf);
        }
    }
	SMCListViewEx *musiclist = FindChildByName2<SMCListViewEx>(_T("musiclist"));
	if (musiclist)
	{
		SMusicListAdapter* musicadapter = new SMusicListAdapter(1, m_hWnd);
		musiclist->SetAdapter(musicadapter);
	}
}

void SaveSkinInf2File(SkinType skinType, SkinSaveInf &skinSaveInf)
{
	pugi::xml_document docSave;
	pugi::xml_node rootNode = docSave.append_child(L"DEMO_SKIN_CONFIG");
	pugi::xml_node childSkinType = rootNode.append_child(L"skinInf");
	childSkinType.append_attribute(L"type") = skinType;
	SStringT strSkinConfigPath = SApplication::getSingleton().GetAppDir() + _T("\\themes\\skin_config.xml");
	switch (skinType)
	{
		//纯色只有SkinSaveInf的color有效
	case color:
		childSkinType.append_attribute(L"color") = (int)skinSaveInf.color;
		break;		
		//此处为系统皮肤，只需要给文件路径和margin
	case sys:
		childSkinType.append_attribute(L"skin_path") = skinSaveInf.filepath;
		SStringW margin;
		margin.Format(L"%d,%d,%d,%d", skinSaveInf.margin.left, skinSaveInf.margin.top, skinSaveInf.margin.right, skinSaveInf.margin.bottom);
		childSkinType.append_attribute(L"skin_margin") = margin;
		break;
	}
	docSave.save_file(strSkinConfigPath);
}

bool CMainDlg::SaveSkin(SkinType skinType, SkinSaveInf &skinSaveInf)
{
	HRESULT hr = S_OK;
	SaveSkinInf2File(skinType, skinSaveInf);
	return hr == S_OK;
}

void LoadSkinFormXml(SDemoSkin *skin, SkinType *skinType, SkinLoadInf *skininf)
{
	SStringT strSkinConfigPath = SApplication::getSingleton().GetAppDir() + _T("\\themes\\skin_config.xml");

	pugi::xml_document docLoad;
	pugi::xml_parse_result result = docLoad.load_file(strSkinConfigPath);
	if (result)
	{
		pugi::xml_node skinInf = docLoad.child(L"DEMO_SKIN_CONFIG").child(L"skinInf");
		*skinType = (SkinType)skinInf.attribute(L"type").as_int();
		switch (*skinType)
		{
			//纯色只有SkinSaveInf的color有效
		case color:
			skininf->color = skinInf.attribute(L"color").as_int();
			break;			
			//此处为系统皮肤，只需要给文件路径和margin
		case sys:
			skininf->filepath = skinInf.attribute(L"skin_path").as_string();
			int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
			swscanf(skinInf.attribute(L"skin_margin").as_string(), L"%d,%d,%d,%d", &v1, &v2, &v3, &v4);
			skininf->margin.left = v1;
			skininf->margin.top = v2;
			skininf->margin.right = v3;
			skininf->margin.bottom = v4;
			break;
		}
	}
	else
	{
		//在这里恢复默认皮肤
		SMessageBox(NULL, _T("找不到系统主题配置文件。"), _T("警告"), NULL);
	}
}
bool CMainDlg::LoadSkin()
{
	SDemoSkin *skin = (SDemoSkin *)GETSKIN(L"demoskinbk",GetScale());
	if (skin)
	{
		SkinLoadInf loadInf;
		SkinType type;
		LoadSkinFormXml(skin, &type, &loadInf);
		skin->SetHander(this);
		return skin->LoadSkin(type, loadInf);
	}
	return false;
}


int funCmpare(void* pCtx,const void *p1,const void *p2)
{
    int iCol=*(int*)pCtx;

    const DXLVITEM *plv1=(const DXLVITEM*)p1;
    const DXLVITEM *plv2=(const DXLVITEM*)p2;

    const student *pst1=(const student *)plv1->dwData;
    const student *pst2=(const student *)plv2->dwData;

    switch(iCol)
    {
    case 0://name
        return _tcscmp(pst1->szName,pst2->szName);
    case 1://sex
        return _tcscmp(pst1->szSex,pst2->szSex);
    case 2://age
        return pst1->age-pst2->age;
    case 3://score
        return pst1->score-pst2->score;
    default:
        return 0;
    }
}

//表头点击事件处理函数
bool CMainDlg::OnListHeaderClick(EventArgs *pEvtBase)
{
    //事件对象强制转换
    EventHeaderClick *pEvt =(EventHeaderClick*)pEvtBase;
    SHeaderCtrl *pHeader=(SHeaderCtrl*)pEvt->sender;
    //从表头控件获得列表控件对象
    SListCtrl *pList= (SListCtrl*)pHeader->GetParent();
    //列表数据排序
    SHDITEM hditem;
    hditem.mask=SHDI_ORDER;
    pHeader->GetItem(pEvt->iItem,&hditem);
    pList->SortItems(funCmpare,&hditem.iOrder);
    return true;
}

void CMainDlg::OnDestory()
{
    SListCtrl *pList=FindChildByName2<SListCtrl>(L"lc_test");
    if(pList)
    {
        for(int i=0;i<pList->GetItemCount();i++)
        {
            student *pst=(student*) pList->GetItemData(i);
            delete pst;
        }
    }
	SDemoSkin *skin = (SDemoSkin *)GETSKIN(L"demoskinbk",GetScale());
	if (skin)
	{
		skin->SaveSkin();
	}
    SetMsgHandled(FALSE);
	if (m_pTabBinder)
		delete m_pTabBinder;
	if (m_pTabBinder2)
		delete m_pTabBinder2;
}


class CSmileySource2 : public CSmileySource
{
public:
    CSmileySource2(){}

protected:
    //获对ID对应的图片路径
    virtual SStringW ImageID2Path(UINT nID)
    {
        return SStringW().Format(L"./gif/%d.gif",nID);
    }
};

//Richedit中插入表情使用的回调函数。
ISmileySource * CreateSource2()
{
    return  new CSmileySource2;
}

HRESULT CMainDlg::OnSkinChangeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled)
{
	FindChildByID(9527)->Invalidate();
	return S_OK;
}

LRESULT CMainDlg::OnInitDialog( HWND hWnd, LPARAM lParam )
{
    m_bLayoutInited=TRUE;
	STabCtrl *pTabCtrl = FindChildByName2<STabCtrl>(L"tab_radio2");
	{
		m_pTabBinder = new STabCtrlHeaderBinder(pTabCtrl);
		m_pTabBinder->Bind(FindChildByName(L"radio3_1"), 0);
		m_pTabBinder->Bind(FindChildByName(L"radio3_2"), 1);
		m_pTabBinder->Bind(FindChildByName(L"radio3_3"), 2);
		m_pTabBinder->Bind(FindChildByName(L"radio3_4"), 3);
		m_pTabBinder->Bind(FindChildByName(L"radio3_5"), 4);
		m_pTabBinder->Bind(FindChildByName(L"radio3_6"), 5);
		m_pTabBinder2 = new STabCtrlHeaderBinder(pTabCtrl);
		m_pTabBinder->Bind(FindChildByName(L"btn4_1"), 0);
		m_pTabBinder->Bind(FindChildByName(L"btn4_2"), 1);
		m_pTabBinder->Bind(FindChildByName(L"btn4_3"), 2);
		m_pTabBinder->Bind(FindChildByName(L"btn4_4"), 3);
		m_pTabBinder->Bind(FindChildByName(L"btn4_5"), 4);
		m_pTabBinder->Bind(FindChildByName(L"btn4_6"), 5);
	}
	LoadSkin();
    //设置为磁吸主窗口
    SetMainWnd(m_hWnd);
    
    InitListCtrl();
	SWindow *pEdit1 = FindChildByName(L"edit_drop_top");

    //设置标题
    SStringW strTitle = SStringW().Format(GETSTRING(R.string.title),SOUI_VER1,SOUI_VER2,SOUI_VER3,SOUI_VER4);
    FindChildByID(R.id.txt_title)->SetWindowText(S_CW2T(tr(strTitle)));
    
    //演示在SOUI中的拖放
    SWindow *pEdit2 = FindChildByName(L"edit_drop_bottom");
    if(pEdit1 && pEdit2)
    {
        HRESULT hr=::RegisterDragDrop(m_hWnd,GetDropTarget());
        RegisterDragDrop(pEdit1->GetSwnd(),new CTestDropTarget1(pEdit1));
        RegisterDragDrop(pEdit2->GetSwnd(),new CTestDropTarget1(pEdit2));
    }
    
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        SetSRicheditOleCallback(pEdit,CreateSource2);
        pEdit->SetAttribute(L"rtf",L"rtf:rtf_test");
    }

    //演示如何响应Edit的EN_CHANGE事件
    SEdit *pEditUrl = FindChildByName2<SEdit>(L"edit_url");
    if(pEditUrl)
    {
        pEditUrl->SSendMessage(EM_SETEVENTMASK,0,ENM_CHANGE);
    }
    
    //演示SetWindowRgn用法
    SWindow *pWndRgn = FindChildByName(L"wnd_rgn");
    if(pWndRgn)
    {
        CRect rc=pWndRgn->GetWindowRect();
        rc.MoveToXY(0,0);//注意：SWindow将窗口的左上角定义为Rgn的原点。
        HRGN hRgn =::CreateEllipticRgnIndirect(&rc);

        CAutoRefPtr<IRegion> pRgn;
        GETRENDERFACTORY->CreateRegion(&pRgn);
        pRgn->SetRgn(hRgn);
        pWndRgn->SetWindowRgn(pRgn,TRUE);

        DeleteObject(hRgn);
    }
    
    //行高固定的列表
    SListView *pLstViewFix = FindChildByName2<SListView>("lv_test_fix");
    if(pLstViewFix)
    {
        ILvAdapter *pAdapter = new CTestAdapterFix;
        pLstViewFix->SetAdapter(pAdapter);
        pAdapter->Release();
    }

    //行高可变的列表
    SListView *pLstViewFlex = FindChildByName2<SListView>("lv_test_flex");
    if(pLstViewFlex)
    {
        ILvAdapter *pAdapter = new CTestAdapterFlex;
        pLstViewFlex->SetAdapter(pAdapter);
        pAdapter->Release();
    }

    //多列listview
    SMCListView * pMcListView = FindChildByName2<SMCListView>("mclv_test");
    if(pMcListView)
    {
        IMcAdapter *pAdapter = new CTestMcAdapterFix;
        pMcListView->SetAdapter(pAdapter);
        pAdapter->Release();
    }
    
    //tileView
    STileView *pTileView = FindChildByName2<STileView>("lv_test_tile");
    if(pTileView)
    {
        CTestTileAdapter *pAdapter = new CTestTileAdapter;
        pTileView->SetAdapter(pAdapter);
        pAdapter->Release();
    }

	//treeview
	STreeView * pTreeView = FindChildByName2<STreeView>("tree_view_00");
	if (pTreeView)
	{
		CTreeViewAdapter * pTreeViewAdapter = new CTreeViewAdapter;
		pTreeView->SetAdapter(pTreeViewAdapter);
		pTreeViewAdapter->Release();
	}

	SShellNotifyIcon *pSShellNotifyIcon = FindChildByID2<SShellNotifyIcon>(8);
	if(pSShellNotifyIcon)
		pSShellNotifyIcon->StartAni();

    return 0;
}

void CMainDlg::OnBtnWebkitGo()
{
    SWkeWebkit *pWebkit= FindChildByName2<SWkeWebkit>(L"wke_test");
    if(pWebkit)
    {
        SEdit *pEdit=FindChildByName2<SEdit>(L"edit_url");
        SStringT strUrl=pEdit->GetWindowText();
        pWebkit->SetAttribute(L"url",S_CT2W(strUrl),FALSE);
    }
}

void CMainDlg::OnBtnWebkitBackward()
{
    SWkeWebkit *pWebkit= FindChildByName2<SWkeWebkit>(L"wke_test");
    if(pWebkit)
    {
        pWebkit->GetWebView()->goBack();
    }
}

void CMainDlg::OnBtnWebkitForeward()
{
    SWkeWebkit *pWebkit= FindChildByName2<SWkeWebkit>(L"wke_test");
    if(pWebkit)
    {
        pWebkit->GetWebView()->goForward();
    }
}

void CMainDlg::OnBtnWebkitRefresh()
{
    SWkeWebkit *pWebkit= FindChildByName2<SWkeWebkit>(L"wke_test");
    if(pWebkit)
    {
        pWebkit->GetWebView()->reload();
    }
}

void CMainDlg::OnBtnSelectGIF()
{
    SGifPlayer *pGifPlayer = FindChildByName2<SGifPlayer>(L"giftest");
    if(pGifPlayer)
    {
        CFileDialogEx openDlg(TRUE,_T("gif"),0,6,_T("gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
            pGifPlayer->PlayGifFile(openDlg.m_szFileName);
    }
}

void CMainDlg::OnBtnMenu()
{
    CPoint pt;
    GetCursorPos(&pt);
    //使用模拟菜单
//     SMenuEx menu;
//     menu.LoadMenu(_T("smenuex:menuex_test"));
//     menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);

    //使用自绘菜单
    SMenu menu;
    menu.LoadMenu(_T("menu_test"),_T("SMENU"));
    menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
}

//演示如何响应菜单事件
void CMainDlg::OnCommand( UINT uNotifyCode, int nID, HWND wndCtl )
{
    if(uNotifyCode==0)
    {
        if(nID == 7)
        {
            if(GetColorizeColor()==0) DoColorize(RGB(255,255,0));//将图片调整为粉红
            else DoColorize(0);//恢复
        }
        else if(nID==6)
        {//nID==6对应menu_test定义的菜单的exit项。
            PostMessage(WM_CLOSE);
        }else if(nID==54)
        {//about SOUI
            STabCtrl *pTabCtrl = FindChildByName2<STabCtrl>(L"tab_main");
            if(pTabCtrl) pTabCtrl->SetCurSel(_T("about"));
		}
		else if (nID == 51)
		{//skin1
			SSkinLoader::getSingleton().LoadSkin(_T("themes\\skin1"));
			SWindow::Invalidate();
		}
		else if (nID == 52)
		{//skin2
			SSkinLoader::getSingleton().LoadSkin(_T("themes\\skin2"));
			SWindow::Invalidate();
		}
		else if (nID == 53)
		{//skin3
			SSkinLoader::getSingleton().LoadSkin(_T("themes\\skin3"));
			SWindow::Invalidate();
		}
		
		else if(nID==100)
        {//delete item in mclistview
            SMCListView *pListView = FindChildByName2<SMCListView>(L"mclv_test");
            if(pListView)
            {
                int iItem = pListView->GetSel();
                if(iItem!=-1)
                {
                    CTestMcAdapterFix *pAdapter = (CTestMcAdapterFix*)pListView->GetAdapter();
                    pAdapter->DeleteItem(iItem);
                }
            }
        }
    }
}


void CMainDlg::OnBtnHideTest()
{
    SWindow * pBtn = FindChildByName(L"btn_display");
    if(pBtn) pBtn->SetVisible(!pBtn->IsVisible(TRUE),TRUE);
}


void CMainDlg::OnBtnInsertGif2RE()
{
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        CFileDialogEx openDlg(TRUE,_T("gif"),0,6,_T("gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
        {
            ISmileySource* pSource = new CSmileySource2;
            HRESULT hr=pSource->LoadFromFile(S_CT2W(openDlg.m_szFileName));
            if(SUCCEEDED(hr))
            {
                SComPtr<ISmileyCtrl> pSmiley;
                hr=::CoCreateInstance(CLSID_SSmileyCtrl,NULL,CLSCTX_INPROC,__uuidof(ISmileyCtrl),(LPVOID*)&pSmiley); 
                if(SUCCEEDED(hr))
                {
                    pSmiley->SetSource(pSource);
                    SComPtr<IRichEditOle> ole;
                    pEdit->SSendMessage(EM_GETOLEINTERFACE,0,(LPARAM)&ole);
                    pSmiley->Insert2Richedit(ole);
                }else
                {
                    UINT uRet = SMessageBox(m_hWnd,_T("可能是因为没有向系统注册表情COM模块。\\n现在注册吗?"),_T("创建表情OLE对象失败"),MB_YESNO|MB_ICONSTOP);
                    if(uRet == IDYES)
                    {
                        HMODULE hMod = LoadLibrary(_T("sosmiley.dll"));
                        if(hMod)
                        {
                            typedef HRESULT (STDAPICALLTYPE *DllRegisterServerPtr)();
                            DllRegisterServerPtr funRegDll = (DllRegisterServerPtr)GetProcAddress(hMod,"DllRegisterServer");
                            if(funRegDll)
                            {
                                HRESULT hr=funRegDll();
                                if(FAILED(hr))
                                {
                                    SMessageBox(m_hWnd,_T("请使用管理员权限运行模块注册程序"),_T("注册表情COM失败"),MB_OK|MB_ICONSTOP);
                                }else
                                {
                                    SMessageBox(m_hWnd,_T("请重试"),_T("注册成功"),MB_OK|MB_ICONINFORMATION);
                                }
                            }
                            FreeLibrary(hMod);
                        }else
                        {
                            SMessageBox(m_hWnd,_T("没有找到表情COM模块[sosmiley.dll]。\\n现在注册吗"),_T("错误"),MB_OK|MB_ICONSTOP);
                        }
                    }
                }
            }else
            {
                SMessageBox(m_hWnd,_T("加载表情失败"),_T("错误"),MB_OK|MB_ICONSTOP);
            }
            pSource->Release();
        }
    }
}

void CMainDlg::OnBtnAppendMsg()
{
    SChatEdit *pEdit = FindChildByName2<SChatEdit>(L"re_gifhost");
    if(pEdit)
    {
        CFormatMsgDlg formatMsgDlg;
        if(formatMsgDlg.DoModal()==IDOK)
        {
            for(int i=0;i<formatMsgDlg.m_nRepeat;i++)
                pEdit->AppendFormatText(S_CT2W(SStringT().Format(_T("line:%d "),i) + formatMsgDlg.m_strMsg));
        }
    }
}

void CMainDlg::OnBtnMsgBox()
{
    SMessageBox(NULL,_T("this is a message box"),_T("haha"),MB_OK|MB_ICONEXCLAMATION);
    SMessageBox(NULL,_T("this message box includes two buttons"),_T("haha"),MB_YESNO|MB_ICONQUESTION);

	CSimpleWnd::SetTimer(TIMER_QUIT,3000,NULL);//3S后退出APP
    SMessageBox(NULL,_T("this message box includes three buttons. \nthe app will quit after 3 seconds if you keep the msgbox open!"),L"Alarm",MB_ABORTRETRYIGNORE|MB_ICONSTOP);
	CSimpleWnd::KillTimer(TIMER_QUIT);
}

class SSkiaTestWnd : public SHostWnd
{
public:
	SSkiaTestWnd(LPCTSTR pszResName = NULL):SHostWnd(pszResName){}


protected:
	void OnLButtonDown(UINT nFlags,CPoint pt)
	{
		SMessageBox(m_hWnd,L"test",L"msgbox",MB_OK);
		SetMsgHandled(FALSE);
	}
BEGIN_MSG_MAP_EX(SSkiaTestWnd)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
	CHAIN_MSG_MAP(__super)
END_MSG_MAP()

	void OnFinalMessage(HWND hWnd){ 
	    //演示OnFinalMessage用法,下面new出来的不需要显示调用delete
	    __super::OnFinalMessage(hWnd);
	    delete this;
	}  
};

void CMainDlg::OnBtnLRC()
{	
    static int s_Count = 0;
    
    SSkiaTestWnd* pHostWnd = new SSkiaTestWnd(_T("layout:dlg_skiatext"));
    pHostWnd->Create(m_hWnd,WS_POPUP,0,0,0,0,0);
    
    //选择一种吸附模式
    CMagnetFrame::ATTACHMODE am=AM_NULL;
    CMagnetFrame::ATTACHALIGN aa=AA_NULL;
    switch(s_Count++ %4)
    {
    case 0:am = AM_TOP,aa = AA_LEFT;break;
    case 1:am = AM_BOTTOM, aa=AA_LEFT;break;
    case 2:am = AM_LEFT, aa=AA_TOP;break;
    case 3:am = AM_RIGHT,aa=AA_TOP;break;
    }
    AddSubWnd(pHostWnd->m_hWnd, am,aa);
    pHostWnd->ShowWindow(SW_SHOW);
}

void CMainDlg::OnTabPageRadioSwitch(EventArgs *pEvt)
{
    EventSwndStateChanged *pEvt2 = sobj_cast<EventSwndStateChanged>(pEvt);
    if(pEvt2->CheckState(WndState_Check) && (pEvt2->dwNewState & WndState_Check))
    {
        int id= pEvt->idFrom;
        STabCtrl *pTab =FindChildByName2<STabCtrl>(L"tab_radio2");
        if(pTab) pTab->SetCurSel(id-10000);
    }
}

void CMainDlg::OnBtnRtfSave()
{
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        CFileDialogEx openDlg(FALSE,_T("rtf"),_T("soui_richedit"),6,_T("rtf files(*.rtf)\0*.rtf\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
        {
            pEdit->SaveRtf(openDlg.m_szFileName);
        }
    }
}

void CMainDlg::OnBtnRtfOpen()
{
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        CFileDialogEx openDlg(TRUE,_T("rtf"),0,6,_T("rtf files(*.rtf)\0*.rtf\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
        {
            pEdit->LoadRtf(openDlg.m_szFileName);
        }
    }
}


void CMainDlg::OnChromeTabNew( EventArgs *pEvt )
{
    static int iPage = 0;
    EventChromeTabNew *pEvtTabNew = (EventChromeTabNew*)pEvt;

    SStringT strTitle = SStringT().Format(_T("新建窗口 %d"),++iPage);
    pEvtTabNew->pNewTab->SetWindowText(strTitle);
    pEvtTabNew->pNewTab->SetAttribute(L"tip",S_CT2W(strTitle));
}

//演示如何使用文件资源创建窗口
void CMainDlg::OnBtnFileWnd()
{
    //由于资源中使用了相对路径，需要将当前路径指定到资源所在位置
    SStringT strCurDir = SApplication::getSingleton().GetAppDir();
    strCurDir += _T("\\filewnd");
    SetCurrentDirectory(strCurDir);
    if(GetFileAttributes(_T("test.xml"))==INVALID_FILE_ATTRIBUTES)
    {
        SMessageBox(m_hWnd,_T("没有找到资源文件！"),_T("错误"),MB_OK|MB_ICONSTOP);
        return ;
    }
    SHostDialog fileDlg(_T("file:test.xml"));
    fileDlg.DoModal(m_hWnd);
}

//演示如何响应Edit的EN_CHANGE事件
void CMainDlg::OnUrlReNotify(EventArgs *pEvt)
{
    EventRENotify *pEvt2 = sobj_cast<EventRENotify>(pEvt);
    STRACE(_T("OnUrlReNotify,iNotify = %d"),pEvt2->iNotify);
    if(pEvt2->iNotify == EN_CHANGE)
    {
        STRACE(_T("OnUrlReNotify,iNotify = EN_CHANGE"));    
    }
}

void CMainDlg::OnMclvCtxMenu(EventArgs *pEvt)
{
    EventCtxMenu *pEvt2 = sobj_cast<EventCtxMenu>(pEvt);
    POINT pt = pEvt2->pt;
    
    {
        //选中鼠标点击行
        SMCListView *pListview = sobj_cast<SMCListView>(pEvt2->sender);
        CPoint pt2 = pt;
        SItemPanel *pItem = pListview->HitTest(pt2);
        if(pItem)
        {
            int iItem = pItem->GetItemIndex();
            pListview->SetSel(iItem);
            STRACE(_T("当前选中行:%d"),iItem);
        }
        
    }
    SMenu menu;
    menu.LoadMenu(_T("menu_lv"),_T("SMENU"));
    
    ClientToScreen(&pt);
        
    menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);

}

//处理模拟菜单中控件的事件
void CMainDlg::OnMenuSliderPos(EventArgs *pEvt)
{
    EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
    SASSERT(pEvt2);
    SSliderBar * pSlider = sobj_cast<SSliderBar>(pEvt->sender);
    SASSERT(pSlider);
    //注意此处不能调用this->FindChildByXXX，因为pEvt是菜单中的对象，和this不是一个host
    SWindow *pText = pSlider->GetParent()->FindChildByName(L"menu_text");
    SASSERT(pText);
    pText->SetWindowText(SStringT().Format(_T("%d"),pEvt2->nPos));
}


void CMainDlg::OnMatrixWindowReNotify(EventArgs *pEvt)
{
    EventRENotify *pEvt2 = sobj_cast<EventRENotify>(pEvt);
    SASSERT(pEvt2);
    if(pEvt2->iNotify != EN_CHANGE)
        return;
    SEdit *pEdit = sobj_cast<SEdit>(pEvt->sender);
    SASSERT(pEdit);
    
    SStringW strValue = S_CT2W(pEdit->GetWindowText());
    
    SWindow *pMatrixWnd = FindChildByName(L"matrix_test");
    SASSERT(pMatrixWnd);
    
    if(SStringW(L"edit_rotate") == pEvt->nameFrom)
    {
        pMatrixWnd->SetAttribute(L"rotate",strValue);
    }else if(SStringW(L"edit_skew") == pEvt->nameFrom)
    {
        pMatrixWnd->SetAttribute(L"skew",strValue);
    }else if(SStringW(L"edit_scale")==pEvt->nameFrom)
    {
        pMatrixWnd->SetAttribute(L"scale",strValue);
    }else if(SStringW(L"edit_translate") == pEvt->nameFrom)
    {
        pMatrixWnd->SetAttribute(L"translate",strValue);
    }
}

//演示从XML字符串动态创建子窗口，使用subscribeEvent来响应创建出来的控件的事件，这里不做演示
void CMainDlg::OnBtnCreateChildren()
{
    SRichEdit *pEdit = FindChildByID2<SRichEdit>(R.id.edit_xml);
    SASSERT(pEdit);
    SStringW strXml = S_CT2W(pEdit->GetWindowText());
    SWindow *pContainer = FindChildByID(R.id.wnd_container);
    SASSERT(pContainer);
    //remove all children at first.
    SWindow *pChild = pContainer->GetWindow(GSW_FIRSTCHILD);
    while(pChild)
    {
        SWindow *pNext = pChild->GetWindow(GSW_NEXTSIBLING);
        pChild->DestroyWindow();
        pChild = pNext;
    }
    //using SWindow::CreateChildren to Create Children described in the input xml string.
    pContainer->CreateChildren(strXml);
}

void CMainDlg::OnBtnClock()
{
    SHostDialog dlgClock(UIRES.LAYOUT.dlg_clock);
    dlgClock.DoModal();
}

void CMainDlg::OnInitListBox()
{
	SListBox *pLb = FindChildByID2<SListBox>(R.id.lb_test);
	if(pLb)
	{
		int nCount = pLb->GetCount();
		for(int i=0; i< 20; i++)
		{
			int iItem = pLb->AddString(SStringT().Format(_T("new item：%d"),nCount+i));
			pLb->EnsureVisible(iItem);
			pLb->UpdateWindow();
			Sleep(10);
		}
	}
}

// 
// void CMainDlg::OnSetSkin(int iSkin)
// {
//     SStringW strSkin = SStringW().Format(L"skin_bkimg_%d",iSkin);
//     SSkinImgList * pSkin = sobj_cast<SSkinImgList>(GETSKIN(strSkin));
//     SASSERT(pSkin);
//     COLORREF crAvg = SDIBHelper::CalcAvarageColor(pSkin->GetImage());
//     FindChildByID(R.id.img_skin_layer)->SetAttribute(L"skin",strSkin);
//     DoColorize(crAvg);
// }

void CMainDlg::OnBtnSkin()
{
	if (!::IsWindow(m_hSetSkinWnd))
	{
		CSetSkinWnd *pSetSkinWnd = new CSetSkinWnd();
		pSetSkinWnd->Create(NULL);
		pSetSkinWnd->CenterWindow(GetDesktopWindow());
		pSetSkinWnd->ShowWindow(SW_SHOWDEFAULT);
		m_hSetSkinWnd = pSetSkinWnd->m_hWnd;
	}
	else
	{
		SetForegroundWindow(m_hSetSkinWnd);
		FlashWindow(m_hSetSkinWnd, TRUE);
	}
}


UINT CMainDlg::Run()
{
	while(!IsStoped())
	{
		EventThread *pEvt = new EventThread(this);
		int nSleep = rand()%2000+500;
		pEvt->nData = nSleep;
		SNotifyCenter::getSingleton().FireEventAsync(pEvt);
		pEvt->Release();
		Sleep(nSleep);
	}
	return 0;
}

void CMainDlg::OnBtnStartNotifyThread()
{
	if(IsRunning()) return;
	SNotifyCenter::getSingleton().addEvent(EVENTID(EventThreadStart));
	SNotifyCenter::getSingleton().addEvent(EVENTID(EventThreadStop));
	SNotifyCenter::getSingleton().addEvent(EVENTID(EventThread));

	EventThreadStart evt(this);
	SNotifyCenter::getSingleton().FireEventSync(&evt);
	BeginThread();	
}

void CMainDlg::OnBtnStopNotifyThread()
{
	if(!IsRunning()) return;

	EndThread();
	EventThreadStop evt(this);
	SNotifyCenter::getSingleton().FireEventSync(&evt);

	SNotifyCenter::getSingleton().removeEvent(EventThreadStart::EventID);
	SNotifyCenter::getSingleton().removeEvent(EventThreadStop::EventID);
	SNotifyCenter::getSingleton().removeEvent(EventThread::EventID);
}

bool CMainDlg::OnEventThreadStart(EventArgs *e)
{
	SChatEdit *pOutput = FindChildByID2<SChatEdit>(R.id.re_notifycenter);
	pOutput->AppendFormatText(L"start Thread");
	return true;
}

bool CMainDlg::OnEventThreadStop(EventArgs *e)
{
	SChatEdit *pOutput = FindChildByID2<SChatEdit>(R.id.re_notifycenter);
	pOutput->AppendFormatText(L"stop Thread");
	return true;
}

bool CMainDlg::OnEventThread(EventArgs *e)
{
	EventThread *pEvt = sobj_cast<EventThread>(e);
	SStringW strMsg = SStringW().Format(L"event thread, sleep = %d",pEvt->nData);
	SChatEdit *pOutput = FindChildByID2<SChatEdit>(R.id.re_notifycenter);
	pOutput->AppendFormatText(strMsg);
	return true;
}

void CMainDlg::OnBtnTip()
{
	SWindow *pBtn = FindChildByID(R.id.btn_tip);
	if (pBtn)
	{
		CRect rc = pBtn->GetWindowRect();
		ClientToScreen(&rc);
		CTipWnd::ShowTip(rc.right, rc.top, CTipWnd::AT_LEFT_BOTTOM, _T("欢迎使用SOUI!\\n如果有好的demo欢迎发送截图给作者，SOUI基于MIT协议\\n启程软件"));
	}
}

void CMainDlg::OnBtnOpenWrapContent()
{
	SHostDialog dlgWrapContent(_T("layout:dlg_wrap_content"));
	dlgWrapContent.DoModal(m_hWnd);
}

void CMainDlg::OnTimer(UINT_PTR idEvent)
{
	SetMsgHandled(FALSE);
	if(idEvent==TIMER_QUIT)
	{
		CSimpleWnd::KillTimer(idEvent);
		PostQuitMessage(-3);
	}
}

void CMainDlg::OnCbxInterpolotorChange(EventArgs *e)
{
	EventCBSelChange *e2=sobj_cast<EventCBSelChange>(e);
	SComboBox *pCbx = sobj_cast<SComboBox>(e2->sender);
	if(e2->nCurSel!=-1)
	{
		SStringT str = pCbx->GetLBText(e2->nCurSel);
		str=str.Mid(1,str.GetLength()-1-strlen("Interpolator"));
		IInterpolator * pInterpolator = CREATEINTERPOLATOR(str);
		if(pInterpolator)
		{
			SInterpolatorView *pView = FindChildByID2<SInterpolatorView>(R.id.view_interpolator);
			pView->SetInterpolator(pInterpolator);
			pInterpolator->Release();
		}
	}
}


void CMainDlg::OnEventPath(EventArgs *e)
{
	EventPath * e2 = sobj_cast<EventPath>(e);
	SStringT strLen = SStringT().Format(_T("%.2f"), e2->fLength);
	FindChildByID(R.id.txt_path_length)->SetWindowText(strLen);
}
