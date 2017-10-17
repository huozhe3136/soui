//////////////////////////////////////////////////////////////////////////
//  Class Name: SItemPanel
// Description: A Framework wrapping frame to be used in a swindow.
//     Creator: Huang Jianxiong
//     Version: 2011.10.20 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma  once

#include "SwndContainerImpl.h"

namespace SOUI
{

class SItemPanel;

struct IItemContainer
{
    virtual void OnItemSetCapture(SItemPanel *pItem,BOOL bCapture)=0; //设置or释放鼠标捕获
    virtual BOOL OnItemGetRect(SItemPanel *pItem,CRect &rcItem)=0;    //获得表项的显示位置
    virtual BOOL IsItemRedrawDelay()=0;                               //指示表项的更新方式
    virtual void OnItemRequestRelayout(SItemPanel *pItem)=0;          //列表项请求重新布局
};

class SOUI_EXP SItemPanel : public SwndContainerImpl
{
public:
    static SItemPanel * Create(SWindow *pFrameHost,pugi::xml_node xmlNode,IItemContainer *pItemContainer);
    
    SWindow * GetHostWindow() { return m_pFrmHost;}

protected:
    SItemPanel(SWindow *pFrameHost,pugi::xml_node xmlNode,IItemContainer *pItemContainer);
	~SItemPanel()  override;

    void OnFinalRelease() override;

public://SwndContainerImpl
    LRESULT DoFrameEvent(UINT uMsg,WPARAM wParam,LPARAM lParam) override;

    BOOL OnFireEvent(EventArgs &evt) override;

    CRect GetContainerRect() override;

    IRenderTarget * OnGetRenderTarget(const CRect & rc ,DWORD gdcFlags) override;

    void OnReleaseRenderTarget(IRenderTarget *pRT,const CRect &rc,DWORD gdcFlags) override;

    void OnRedraw(const CRect &rc) override;

    BOOL OnReleaseSwndCapture() override;

    SWND OnSetSwndCapture(SWND swnd) override;
    HWND GetHostHwnd() override;
    const SStringW & GetTranslatorContext() override;
    void FrameToHost(RECT & rc) override;
    BOOL IsTranslucent() const override;
    BOOL IsSendWheel2Hover() const override;
    
    BOOL OnCreateCaret(SWND swnd,HBITMAP hBmp,int nWidth,int nHeight) override;

    BOOL OnShowCaret(BOOL bShow) override;

    BOOL OnSetCaretPos(int x,int y) override;

    BOOL UpdateWindow() override;

	void UpdateTooltip() override;

    SMessageLoop *GetMsgLoop() override;
    
    IScriptModule * GetScriptModule() override;

	int GetScale() const override;
public://SWindow
    virtual void ModifyItemState(DWORD dwStateAdd, DWORD dwStateRemove);

    SWND SwndFromPoint(CPoint ptHitTest, BOOL bOnlyText) override;

    virtual void Draw(IRenderTarget *pRT,const CRect & rc);

    virtual void SetSkin(ISkinObj *pSkin);
    virtual void SetColor(COLORREF crBk,COLORREF crSelBk);
    void BeforePaint(IRenderTarget *pRT, SPainter &painter) override;

    BOOL NeedRedrawWhenStateChange() override;
    BOOL OnUpdateToolTip(CPoint pt, SwndToolTipInfo &tipInfo) override;
	BOOL IsLayeredWindow() const override;
    void RequestRelayout(SWindow *pSource ,BOOL bSourceResizable) override;
    
    CRect GetItemRect();
    void SetItemCapture(BOOL bCapture);
    void SetItemData(LPARAM dwData);
    LPARAM GetItemData();

    LPARAM GetItemIndex(){return m_lpItemIndex;}
    void SetItemIndex(LPARAM lp){m_lpItemIndex=lp;}

    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnDestroy();

    SOUI_MSG_MAP_DECL()

protected:
    SOUI_ATTRS_DECL() 
protected:
    SWindow * m_pFrmHost;
    IItemContainer * m_pItemContainer;
    COLORREF m_crBk, m_crSelBk,m_crHover;
    LPARAM        m_dwData;
    LPARAM        m_lpItemIndex;
};


}//namespace SOUI