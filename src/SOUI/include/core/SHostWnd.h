//////////////////////////////////////////////////////////////////////////
//  Class Name: SHostWnd
//    Description: Real Container of SWindow
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "SwndContainerImpl.h"

#include "SimpleWnd.h"
#include "SDropTargetDispatcher.h"
#include "event/eventcrack.h"
#include "interface/stooltip-i.h"
#include "helper/swndspy.h"
#include "helper/SScriptTimer.h"
#include "core/SCaret.h"
#include "core/hostmsg.h"
#include "layout/slayoutsize.h"
#include "helper/SplitString.h"

namespace SOUI
{

    class SHostWnd;
    class SDummyWnd : public CSimpleWnd
    {
    public:
		SDummyWnd(SHostWnd* pOwner);

        void OnPaint(HDC dc);

		BEGIN_MSG_MAP_EX_DECL()

    private:
        SHostWnd *m_pOwner;
    };

	
    class SHostWndAttr : public SObject, public ITrCtxProvider
    {
        SOUI_CLASS_NAME_DECL(SHostWndAttr, L"SHostWndAttr")
        enum {WT_UNDEFINE=0,WT_APPMAIN=1,WT_NORMAL=2};
		friend class SHostWnd;
    public:
		SHostWndAttr(void);

		~SHostWndAttr() override;
        
		void Init();
        
		const SStringW & GetTrCtx() override;

		CRect GetMaxInset(int nScale) const;

		CRect GetMargin(int nScale) const;

		CSize GetMinSize(int nScale) const;

		void SetTranslucent(bool bTranslucent) {
			m_bTranslucent = bTranslucent;
		}

		void SetTrCtx(const SStringW & strTrCtx)
		{
			m_strTrCtx = strTrCtx;
		}

        SOUI_ATTRS_DECL() 

	protected:
        SLayoutSize m_rcMargin[4];       //窗口拉伸的边缘检测大小
		SLayoutSize m_szMin[2];          //窗口最小值
		SLayoutSize	m_width;			/* 窗口初始宽度 */
		SLayoutSize	m_height;			/* 窗口初始高度 */
		SLayoutSize m_rcMaxInset[4];     //窗口最大化时超出屏幕的边缘大小。经测试，WS_OVERLAPPED style的窗口该属性无效

        DWORD m_byAlpha:8;          //透明度
        DWORD m_byWndType:8;         //主窗口标志,有该标志的窗口关闭时自动发送WM_QUIT
        DWORD m_bResizable:1;       //窗口大小可调节
        DWORD m_bAppWnd:1;          //APP窗口，在任务栏上显示按钮
        DWORD m_bToolWnd:1;         //设置WS_ES_TOOLWINDOW属性
        DWORD m_bTranslucent:1;     //窗口的半透明属性
        DWORD m_bAllowSpy:1;        //允许spy
        DWORD m_bSendWheel2Hover:1; //将滚轮消息发送到hover窗口

        DWORD m_dwStyle;
        DWORD m_dwExStyle;

        SStringW m_strTrCtx;     //在语言翻译时作为context使用
        STrText  m_strTitle;
        HICON   m_hAppIconSmall;
        HICON   m_hAppIconBig;
    };

class SOUI_EXP SHostWnd
    : public SwndContainerImpl
    , public CSimpleWnd
{
    SOUI_CLASS_NAME_DECL(SHostWnd,L"hostwnd")
    friend class SDummyWnd;
protected:    
    SDummyWnd            m_dummyWnd;            /**<半透明窗口使用的一个响应WM_PAINT消息的窗口*/
    SHostWndAttr         m_hostAttr;            /**<host属性，对应XML中的SOUI结点 */
    SStringT             m_strXmlLayout;        /**<布局资源名称,如xml:main_wnd */

    BOOL                 m_bTrackFlag;          /**< 跟踪鼠标标志 */

    CAutoRefPtr<SCaret>     m_caret;            /**< 光标对象 */
    BOOL                    m_bCaretShowing;    /**<当前有插入符正在显示*/
    BOOL                    m_bCaretActive;     /**<模拟插入符正在显示标志*/
    CPoint                  m_ptCaret;          /**<插入符位置*/

    BOOL                    m_bNeedRepaint;     /**<缓存脏标志*/
    BOOL                    m_bNeedAllRepaint;  /**<缓存全部更新标志*/

    IToolTip *              m_pTipCtrl;         /**<tip接口*/

    CAutoRefPtr<IRegion>    m_rgnInvalidate;    /**<脏区域*/
    CAutoRefPtr<IRenderTarget> m_memRT;         /**<绘制缓存*/
    BOOL                    m_bResizing;        /**<执行WM_SIZE*/
    CAutoRefPtr<SStylePool> m_privateStylePool; /**<局部style pool*/
    CAutoRefPtr<SSkinPool>  m_privateSkinPool;  /**<局部skin pool*/

    SList<SWND>             m_lstUpdateSwnd;    /**<等待刷新的非背景混合窗口列表*/
    SList<RECT>             m_lstUpdatedRect;   /**<更新的脏矩形列表*/
    BOOL                    m_bRending;         /**<正在渲染过程中*/
    
    MSG                     m_msgMouse;         /**<上一次鼠标按下消息*/
    
    CAutoRefPtr<IScriptModule> m_pScriptModule; /**<脚本模块*/   

	int						m_nScale;			/**<缩放比例 */

public:
    SHostWnd(LPCTSTR pszResName = NULL);
    ~SHostWnd() override;

public:
    SWindow * GetRoot() const {return (SWindow*)this;}

	CSimpleWnd * GetNative(){return this;}

    HWND Create(HWND hWndParent,int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);
    HWND Create(HWND hWndParent,DWORD dwStyle,DWORD dwExStyle, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);

    BOOL InitFromXml(pugi::xml_node xmlNode) override;
    
    BOOL AnimateHostWindow(DWORD dwTime,DWORD dwFlags);

    //实现3个供脚本使用的定时器函数
    UINT setTimeout(LPCSTR pszScriptFunc,UINT uElapse);
    UINT setInterval(LPCSTR pszScriptFunc,UINT uElapse);
    void clearTimer(UINT uID);

    //实现几个常用接口,防止派生类调用时产生歧义
    BOOL DestroyWindow();
    UINT_PTR SetTimer(UINT_PTR nIDEvent,UINT nElapse);
    BOOL KillTimer(UINT_PTR id);
    CRect GetWindowRect() const;
    CRect GetClientRect() const override;

	SHostWndAttr & GetHostAttr() {
		return m_hostAttr;
	}

	IToolTip * GetToolTip() const {
		return m_pTipCtrl;
	}
protected://辅助函数
    BOOL _InitFromXml(pugi::xml_node xmlNode,int nWidth,int nHeight);
    void _Redraw();
    void _UpdateNonBkgndBlendSwnd();
    void _DrawCaret(CPoint pt,BOOL bErase);
    void _RestoreClickState();
	bool _IsRootWrapContent();
protected:
    //////////////////////////////////////////////////////////////////////////
    // Message handler

    void OnPrint(HDC dc, UINT uFlags);

    void OnPaint(HDC dc);

    BOOL OnEraseBkgnd(HDC dc);

    int OnCreate(LPCREATESTRUCT lpCreateStruct);

    void OnDestroy();

    void OnSize(UINT nType, CSize size);

    void OnMouseMove(UINT nFlags, CPoint point);

    void OnMouseLeave();

    BOOL OnWndSetCursor(HWND hWnd, UINT nHitTest, UINT message) ;

    void OnTimer(UINT_PTR idEvent);

    void OnSwndTimer(char cTimerID);


    LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT OnHostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
 
    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);

    void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);

    BOOL OnNcActivate(BOOL bActive);
    
    UINT OnWndNcHitTest(CPoint point);

    void OnSetFocus(HWND wndOld);
    void OnKillFocus(HWND wndFocus);
        
    void UpdateHost(HDC dc,const CRect &rc);
    void UpdateLayerFromRenderTarget(IRenderTarget *pRT,BYTE byAlpha, LPCRECT prcDirty=NULL);

    LRESULT OnUpdateSwnd(UINT uMsg,WPARAM,LPARAM);
    
    void OnCaptureChanged(HWND wnd);

    LRESULT OnScriptTimer(UINT uMsg,WPARAM wParam,LPARAM lParam);
    
    LRESULT OnMenuExEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

	void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
        
	bool onRootResize(EventArgs *e);
#ifndef DISABLE_SWNDSPY
protected:
    LRESULT OnSpyMsgSetSpy(UINT uMsg,WPARAM wParam,LPARAM lParam);

    LRESULT OnSpyMsgSwndEnum(UINT uMsg,WPARAM wParam,LPARAM lParam);

    LRESULT OnSpyMsgSwndSpy(UINT uMsg,WPARAM wParam,LPARAM lParam);
    
    LRESULT OnSpyMsgHitTest(UINT uMsg,WPARAM wParam,LPARAM lParam);

    HWND    m_hSpyWnd;
#endif

protected:// IContainer

    BOOL OnFireEvent(EventArgs &evt) override;

    CRect GetContainerRect() override;

    HWND GetHostHwnd() override;

    const SStringW & GetTranslatorContext() override;

    IRenderTarget * OnGetRenderTarget(const CRect & rc,DWORD gdcFlags) override;

    void OnReleaseRenderTarget(IRenderTarget * pRT,const CRect &rc,DWORD gdcFlags) override;

    void OnRedraw(const CRect &rc) override;

    BOOL OnReleaseSwndCapture() override;

    SWND OnSetSwndCapture(SWND swnd) override;

    BOOL IsTranslucent() const override;
    
    BOOL IsSendWheel2Hover() const override;
    
    BOOL OnCreateCaret(SWND swnd,HBITMAP hBmp,int nWidth,int nHeight) override;

    BOOL OnShowCaret(BOOL bShow) override;

    BOOL OnSetCaretPos(int x,int y) override;

    BOOL UpdateWindow() override;

	void UpdateTooltip() override;

    BOOL RegisterTimelineHandler(ITimelineHandler *pHandler) override;

    BOOL UnregisterTimelineHandler(ITimelineHandler *pHandler) override;

    SMessageLoop * GetMsgLoop() override;

    IScriptModule * GetScriptModule() override;

	int GetScale() const override;
protected://Swindow 虚方法
    void BeforePaint(IRenderTarget *pRT, SPainter &painter) override;
    void AfterPaint(IRenderTarget *pRT, SPainter &painter) override;
	BOOL IsLayeredWindow() const  override;
	void UpdateLayout() override;
	HRESULT OnLanguageChanged() override;
	void OnScaleChanged(int scale) override;


public:
    void RequestRelayout(SWindow *pSource ,BOOL bSourceResizable ) override;
    
public://事件处理接口
	virtual BOOL _HandleEvent(EventArgs *pEvt);


    BEGIN_MSG_MAP_EX_DECL() 


};

}//namespace SOUI
