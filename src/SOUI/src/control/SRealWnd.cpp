#include "souistd.h"
#include "control/Srealwnd.h"
#include "control/RealWndHandler-i.h"

namespace SOUI
{


SRealWndParam::SRealWndParam()
    :m_dwStyle(WS_CHILD)
    ,m_dwExStyle(0)
{

}

SRealWndParam::~SRealWndParam()
{
}


SOUI_ATTRS_BEGIN(SRealWnd)
	ATTR_STRINGT(L"wndclass", m_realwndParam.m_strClassName, FALSE)
	ATTR_STRINGT(L"wndname", m_realwndParam.m_strWindowName, FALSE)
	ATTR_HEX(L"style", m_realwndParam.m_dwStyle, FALSE)
	ATTR_HEX(L"exstyle", m_realwndParam.m_dwExStyle, FALSE)
	ATTR_INT(L"init", m_bInit, FALSE)
SOUI_ATTRS_END()

SOUI_MSG_MAP_BEGIN(SRealWnd)
	MSG_WM_PAINT_EX(OnPaint)
	MSG_WM_DESTROY(OnDestroy)
	MSG_WM_SHOWWINDOW(OnShowWindow)
SOUI_MSG_MAP_END()

SOUI_CLASS_NAME(SRealWnd, L"realwnd")

SRealWnd::SRealWnd()
    :m_bInit(FALSE)
    ,m_hRealWnd(0)
	, m_lpData(0)
{
}

SRealWnd::~SRealWnd()
{
}

BOOL SRealWnd::NeedRedrawWhenStateChange()
{
    return FALSE;
}

void SRealWnd::ShowRealWindow()
{
    if(IsVisible(TRUE) && !IsWindow(m_hRealWnd))
    {
        InitRealWnd();
    }
    if(IsWindow(m_hRealWnd))
    {
        ShowWindow(m_hRealWnd,IsVisible(TRUE) ? SW_SHOW : SW_HIDE);
    }
}

void SRealWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    __super::OnShowWindow(bShow, nStatus);
    ShowRealWindow();
}

void SRealWnd::OnDestroy()
{
    if (IsWindow(m_hRealWnd))
    {
        IRealWndHandler *pRealWndHandler=GETREALWNDHANDLER;
        if(pRealWndHandler) pRealWndHandler->OnRealWndDestroy(this);
    }
}

BOOL SRealWnd::InitFromXml(pugi::xml_node xmlNode)
{
    BOOL bRet=__super::InitFromXml(xmlNode);
    if(bRet)
    {
        m_realwndParam.m_xmlParams.append_copy(xmlNode.child(L"params"));
        if(m_bInit) InitRealWnd();
    }
    return bRet;
}

HWND SRealWnd::GetRealHwnd(BOOL bAutoCreate/*=TRUE*/)
{
    if(!bAutoCreate) return m_hRealWnd;

    if(!m_bInit && !IsWindow(m_hRealWnd))
    {
        InitRealWnd();
    }

    return m_hRealWnd;
}

BOOL SRealWnd::InitRealWnd()
{
    m_realwndParam.m_dwStyle|= WS_CHILD;

    IRealWndHandler *pRealWndHandler=GETREALWNDHANDLER;

    if(pRealWndHandler) m_hRealWnd = pRealWndHandler->OnRealWndCreate(this);

    if(::IsWindow(m_hRealWnd))
    {
        if(!m_bInit && !pRealWndHandler->OnRealWndSize(this))
        {
            //如果不是在加载的时候创建窗口，则需要自动调整窗口位置
            CRect rcClient;
            GetClientRect(&rcClient);
            SetWindowPos(m_hRealWnd,0,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),SWP_NOZORDER);
        }

        if(pRealWndHandler) pRealWndHandler->OnRealWndInit(this);

        return TRUE;
    }
    return FALSE;
}

BOOL SRealWnd::OnRelayout(const CRect &rcWnd)
{
	if (!__super::OnRelayout(rcWnd)) 
		return FALSE;

	IRealWndHandler *pRealWndHandler = GETREALWNDHANDLER;
	HWND hRealWnd = GetRealHwnd(FALSE);
	if (pRealWndHandler && ::IsWindow(hRealWnd))
	{
		if (!pRealWndHandler->OnRealWndPosition(this,rcWnd))
		{
			CPoint pt = rcWnd.TopLeft();
			if(GetWindowLong(hRealWnd,GWL_STYLE)&WS_POPUP)
			{
				::ClientToScreen(GetContainer()->GetHostHwnd(),&pt);
			}
			::SetWindowPos(hRealWnd, 0, pt.x, pt.y, rcWnd.Width(), rcWnd.Height(), SWP_NOZORDER);
		}
	}
	return TRUE;
}

BOOL IRealWndHandler::OnRealWndInit(SRealWnd *pRealWnd) 
{
	return FALSE; 
}

BOOL IRealWndHandler::OnRealWndSize(SRealWnd *pRealWnd) 
{
	return FALSE; 
}

BOOL IRealWndHandler::OnRealWndPosition(SRealWnd *pRealWnd, const CRect &rcWnd)
{
	return FALSE; 
}
}//namespace SOUI