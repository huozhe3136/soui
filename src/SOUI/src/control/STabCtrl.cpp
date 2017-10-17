﻿#include "souistd.h"
#include "control/Stabctrl.h"
#include "animator/SInterpolatorImpl.h"
#include <algorithm>

namespace SOUI
{

	class STabSlider : public SWindow, public ITimelineHandler
	{
		SOUI_CLASS_NAME_DECL(STabSlider, L"tabslider")

	public:
		STabSlider(STabCtrl *pTabCtrl, int iFrom, int iTo, int nSteps, int nType,IInterpolator *pInterpolator)
			: m_nSteps(nSteps)
			, m_iStep(0)
			, m_aniInterpoloator(pInterpolator)
			, m_pTabCtrl(pTabCtrl)
		{
			SASSERT(pTabCtrl);
			SASSERT(pInterpolator);

			CRect rcPage = pTabCtrl->GetChildrenLayoutRect();
			if (nType == 0)
			{				
				pTabCtrl->InsertChild(this);
				Move(rcPage);

				m_bVertical = pTabCtrl->m_nTabAlign == STabCtrl::AlignLeft || pTabCtrl->m_nTabAlign == STabCtrl::AlignRight;
				if (m_bVertical)
				{
					GETRENDERFACTORY->CreateRenderTarget(&m_memRT, rcPage.Width(), rcPage.Height() * 2);
				}
				else
				{
					GETRENDERFACTORY->CreateRenderTarget(&m_memRT, rcPage.Width() * 2, rcPage.Height());
				}

				CPoint pt;

				if (m_bVertical)
				{
					if (iFrom < iTo)
					{// move up 
						pt.x = pt.y = 0;
						m_nFrom = 0;
						m_nTo = rcPage.Height();
						//m_nAniRange = rcPage.Height();
					}
					else
					{// move down
						pt.x = 0, pt.y = rcPage.Height();
						m_ptOffset.y = rcPage.Height();
						m_nFrom = rcPage.Height();
						m_nTo = 0;
//						m_nAniRange = -rcPage.Height();
					}
				}
				else
				{
					if (iFrom < iTo)
					{// move left
						pt.x = pt.y = 0;
						m_nFrom = 0;
						m_nTo = rcPage.Width();
						//m_nAniRange = rcPage.Width();
					}
					else
					{
						pt.x = rcPage.Width(), pt.y = 0;
						m_ptOffset.x = rcPage.Width();
						m_nFrom = rcPage.Width();
						m_nTo = 0;
						//m_nAniRange = -rcPage.Width();
					}
				}
				pt -= rcPage.TopLeft();
				m_memRT->SetViewportOrg(pt);
				PaintBackground(m_memRT, &rcPage);

				pTabCtrl->GetItem(iFrom)->SetVisible(FALSE);

				if (m_bVertical)
				{
					if (iFrom < iTo)
					{// move up 
						pt.x = 0, pt.y = rcPage.Height();
					}
					else
					{// move down
						pt.x = pt.y = 0;
					}
				}
				else
				{
					if (iFrom < iTo)
					{// move left
						pt.x = rcPage.Width(), pt.y = 0;
					}
					else
					{
						pt.x = pt.y = 0;
					}
				}

				pt -= rcPage.TopLeft();
				m_memRT->SetViewportOrg(pt);

				pTabCtrl->GetItem(iTo)->SetVisible(TRUE);
				PaintBackground(m_memRT, &rcPage);

				m_memRT->SetViewportOrg(CPoint());

				GetContainer()->RegisterTimelineHandler(this);
				pTabCtrl->GetItem(iTo)->SetVisible(FALSE);
				SetVisible(TRUE, TRUE);
			}
			else
			{
				pTabCtrl->InsertChild(this, ICWND_FIRST);				
				Move(rcPage);

				m_bVertical = pTabCtrl->m_nTabAlign == STabCtrl::AlignLeft || pTabCtrl->m_nTabAlign == STabCtrl::AlignRight;
				if (m_bVertical)
				{
					GETRENDERFACTORY->CreateRenderTarget(&m_memRT, rcPage.Width(), rcPage.Height() * 2);
				}
				else
				{
					GETRENDERFACTORY->CreateRenderTarget(&m_memRT, rcPage.Width() * 2, rcPage.Height());
				}

				CPoint pt;

				if (m_bVertical)
				{
					if (iFrom < iTo)
					{// move up 
						pt.x = pt.y = 0;
						m_nFrom = 0;
						m_nTo = rcPage.Height();
						//m_nAniRange = rcPage.Height();
					}
					else
					{// move down
						pt.x = 0, pt.y = rcPage.Height();
						m_ptOffset.y = rcPage.Height();
						m_nFrom = rcPage.Height();
						m_nTo = 0;
						//m_nAniRange = -rcPage.Height();
					}
				}
				else
				{
					if (iFrom < iTo)
					{// move left
						pt.x = pt.y = 0;
						m_nFrom = 0;
						m_nTo = rcPage.Width();
						//m_nAniRange = rcPage.Width();
					}
					else
					{
						pt.x = rcPage.Width(), pt.y = 0;
						m_ptOffset.x = rcPage.Width();
						m_nFrom = rcPage.Width();
						m_nTo = 0;
						//m_nAniRange = -rcPage.Width();
					}
				}
				pt -= rcPage.TopLeft();
				m_memRT->SetViewportOrg(pt);
				PaintForeground2(m_memRT, &rcPage);

				pTabCtrl->GetItem(iFrom)->SetVisible(FALSE);

				if (m_bVertical)
				{
					if (iFrom < iTo)
					{// move up 
						pt.x = 0, pt.y = rcPage.Height();
					}
					else
					{// move down
						pt.x = pt.y = 0;
					}
				}
				else
				{
					if (iFrom < iTo)
					{// move left
						pt.x = rcPage.Width(), pt.y = 0;
					}
					else
					{
						pt.x = pt.y = 0;
					}
				}

				pt -= rcPage.TopLeft();
				m_memRT->SetViewportOrg(pt);

				pTabCtrl->GetItem(iTo)->SetVisible(TRUE);
				PaintForeground2(m_memRT, &rcPage);

				m_memRT->SetViewportOrg(CPoint());

				GetContainer()->RegisterTimelineHandler(this);
				pTabCtrl->GetItem(iTo)->SetVisible(FALSE);
				SetVisible(TRUE, TRUE);
			}
		}

		~STabSlider() override
		{
		}

		void OnNextFrame() override
		{
			if (++m_iStep > m_nSteps)
			{
				Stop();
			}
			else
			{
				float fPos = m_aniInterpoloator->getInterpolation(m_iStep*1.0f/m_nSteps);
				int nOffset = m_nFrom + (int)(fPos * (m_nTo - m_nFrom));
				if (m_bVertical)
					m_ptOffset.y = nOffset;
				else
					m_ptOffset.x = nOffset;
				InvalidateRect(NULL);
			}
		}

		void Stop()
		{
			m_pTabCtrl->OnSliderFinish();
		}
	protected:

		void OnPaint(IRenderTarget *pRT)
		{
			CRect rcWnd = GetWindowRect();
			//pRT->BitBlt(rcWnd, m_memRT, m_ptOffset.x, m_ptOffset.y, SRCCOPY);
			CRect rcSrc(m_ptOffset.x, m_ptOffset.y, m_ptOffset.x + rcWnd.Width(), m_ptOffset.y + rcWnd.Height());
			pRT->AlphaBlend(rcWnd, m_memRT, rcSrc, 255);
		}

		void OnSize(UINT fType, CSize sz)
		{
			SWindow::OnSize(fType, sz);
			if (!m_memRT) return;
			//resize slidewnd as animitor running, just stop the animator
			Stop();
		}

		void OnDestroy()
		{
			GetContainer()->UnregisterTimelineHandler(this);
			SWindow::OnDestroy();
		}

		CAutoRefPtr<IRenderTarget> m_memRT;
		CPoint                     m_ptOffset;
		//int                        m_nAniRange;
		int						   m_nFrom,m_nTo;
		int                        m_nSteps;
		int                        m_iStep;
		bool                       m_bVertical;
		CAutoRefPtr<IInterpolator> m_aniInterpoloator;
		STabCtrl *                 m_pTabCtrl;
		SOUI_MSG_MAP_DECL() 
	};

	SOUI_MSG_MAP_BEGIN(STabSlider)
		MSG_WM_PAINT_EX(OnPaint)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DESTROY(OnDestroy)
	SOUI_MSG_MAP_END()

	SOUI_CLASS_NAME(STabSlider, L"tabslider")
//////////////////////////////////////////////////////////////////////////
// STabCtrl
	 SOUI_ATTRS_BEGIN(STabCtrl)
            ATTR_INT(L"curSel", m_nCurrentPage, FALSE)
            ATTR_SIZE(L"tabSize",m_szTab,TRUE)
            ATTR_INT(L"tabWidth", m_szTab.cx, FALSE)
            ATTR_INT(L"tabHeight", m_szTab.cy, FALSE)
            ATTR_INT(L"tabPos", m_nTabPos, FALSE)
            ATTR_INT(L"tabInterSize", m_nTabInterSize, FALSE)
            ATTR_SKIN(L"tabInterSkin", m_pSkinTabInter, FALSE)
            ATTR_SKIN(L"tabSkin", m_pSkinTab, FALSE)
            ATTR_SKIN(L"iconSkin", m_pSkinIcon, FALSE)
            ATTR_SKIN(L"frameSkin", m_pSkinFrame, FALSE)
            ATTR_INT(L"icon-x", m_ptIcon.x, FALSE)
            ATTR_INT(L"icon-y", m_ptIcon.y, FALSE)
            ATTR_INT(L"text-x", m_ptText.x, FALSE)
            ATTR_INT(L"text-y", m_ptText.y, FALSE)
            ATTR_ENUM_BEGIN(L"tabAlign", int, TRUE)
                ATTR_ENUM_VALUE(L"top", AlignTop)
                ATTR_ENUM_VALUE(L"left", AlignLeft)
                ATTR_ENUM_VALUE(L"right", AlignRight)
                ATTR_ENUM_VALUE(L"bottom", AlignBottom)
            ATTR_ENUM_END(m_nTabAlign)
            ATTR_ENUM_BEGIN(L"textDir", TEXTDIR, TRUE)
                ATTR_ENUM_VALUE(L"hori", Text_Horz)
                ATTR_ENUM_VALUE(L"horizontal", Text_Horz)
                ATTR_ENUM_VALUE(L"vert", Text_Vert)
                ATTR_ENUM_VALUE(L"vertical", Text_Vert)
            ATTR_ENUM_END(m_txtDir)
            ATTR_INT(L"animateSteps",m_nAnimateSteps,FALSE)
            ATTR_INT(L"animateType", m_nAniamteType, FALSE)/*动画样式0：背景跟着动，1：背景不动*/
            ATTR_INTERPOLATOR(L"interpolator",m_aniInterpolator,FALSE)
            ATTR_CHAIN_PTR(m_aniInterpolator,0)//chain attributes to interpolator
   SOUI_ATTRS_END()


	SOUI_MSG_MAP_BEGIN(STabCtrl)
		MSG_WM_PAINT_EX(OnPaint)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_KEYDOWN(OnKeyDown)
	SOUI_MSG_MAP_END()

	SOUI_CLASS_NAME(STabCtrl, L"tabctrl")
STabCtrl::STabCtrl() : 
		  m_nHoverTabItem(-1)
		, m_nCurrentPage(0)
		, m_nTabInterSize(0)
		, m_nTabPos(0)
		, m_pSkinTab(GETBUILTINSKIN(SKIN_SYS_TAB_PAGE))
		, m_pSkinIcon(NULL)
		, m_pSkinTabInter(NULL)
		, m_pSkinFrame(NULL)
		, m_ptText(-1, -1)
		, m_nTabAlign(AlignTop)
		, m_tabSlider(NULL)
		, m_txtDir(Text_Horz)
		, m_nAnimateSteps(0)
		,m_nAniamteType(0)
{
    m_szTab.cx = m_szTab.cy = -1;
    m_bFocusable=TRUE;
	//create a linear animator interpolator
	m_aniInterpolator.Attach(SApplication::getSingleton().CreateInterpolatorByName(SLinearInterpolator::GetClassName()));

    m_evtSet.addEvent(EVENTID(EventTabSelChanging));
    m_evtSet.addEvent(EVENTID(EventTabSelChanged));
    m_evtSet.addEvent(EVENTID(EventTabItemHover));
    m_evtSet.addEvent(EVENTID(EventTabItemLeave));
}

STabCtrl::~STabCtrl() 
{

}

UINT STabCtrl::OnGetDlgCode()
{
	return SC_WANTARROWS;
}

void STabCtrl::OnPaint(IRenderTarget *pRT)
{
    SPainter painter;
    BeforePaint(pRT,painter);
    
    CRect rcItem,rcItemPrev;
    CRect rcSplit;
    DWORD dwState;
    CRect rcTitle=GetTitleRect();
    

    pRT->PushClipRect(&rcTitle,RGN_AND);

    for(int i=0; i<(int)GetItemCount(); i++)
    {
        dwState=WndState_Normal;
        if(i == m_nCurrentPage) dwState=WndState_PushDown;
        else if(i== m_nHoverTabItem) dwState=WndState_Hover;

        GetItemRect(i,rcItem);
		if(rcItem.IsRectEmpty()) continue;

        //画分隔线
        if(i>0 && m_pSkinTabInter)
        {
            rcSplit=rcItem;
            if(m_nTabAlign==AlignLeft)
            {
                rcSplit.top=rcItemPrev.bottom;
                rcSplit.bottom = rcSplit.top + m_nTabInterSize;
            }
            else
            {
                rcSplit.left=rcItemPrev.right;
                rcSplit.right=rcSplit.left + m_nTabInterSize;
            }
            m_pSkinTabInter->Draw(pRT,rcSplit,0);
        }

        DrawItem(pRT,rcItem,i,dwState);
        rcItemPrev=rcItem;
    }
    pRT->PopClip();
    
    if (m_pSkinFrame)
    {
        CRect rcPage = GetChildrenLayoutRect();
        m_pSkinFrame->Draw(pRT, rcPage, WndState_Normal);
    }

    if(IsFocused() && IsFocusable() && m_bDrawFocusRect)
    {
        CRect rc;
        GetItemRect(m_nCurrentPage,rc);
        rc.DeflateRect(2,2);
        DrawDefFocusRect(pRT,&rc);
    }
    AfterPaint(pRT,painter);
}

CRect STabCtrl::GetChildrenLayoutRect()
{
    CRect rcRet;
    GetClientRect(rcRet);

    switch(m_nTabAlign)
    {
    case AlignLeft:
        rcRet.left+= m_szTab.cx;
        break;
    case AlignRight:
        rcRet.right-=m_szTab.cx;
        break;
    case AlignTop:
        rcRet.top += m_szTab.cy;
        break;
    case AlignBottom:
        rcRet.bottom -= m_szTab.cy;
        break;
    }
    return rcRet;
}

void STabCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
    SWindow::OnLButtonDown(nFlags,point);
    int iClickItem = HitTest(point);
    if(iClickItem != m_nCurrentPage)
    {
        SetCurSel(iClickItem);
    }
}

BOOL STabCtrl::RemoveItem( int nIndex , int nSelPage/*=0*/)
{
    STabPage * pTab = GetItem(nIndex);
	OnItemRemoved(pTab);

    DestroyChild(pTab);
    m_lstPages.RemoveAt(nIndex);

    if (m_nCurrentPage == nIndex)
    {
        if(nSelPage<0) nSelPage=0;
        if(nSelPage>= GetItemCount()) nSelPage=GetItemCount()-1;
        m_nCurrentPage=-1;
        SetCurSel(nSelPage);
    }else
    {
        if(m_nCurrentPage>nIndex) m_nCurrentPage--;
    }
	CRect rcTitle = GetTitleRect();
	InvalidateRect(rcTitle);
    return TRUE;
}

void STabCtrl::RemoveAllItems( void )
{
    for (int i = GetItemCount()-1; i >= 0; i--)
    {
        STabPage * pTab = GetItem(i);
		OnItemRemoved(pTab);
        DestroyChild(pTab);
        m_lstPages.RemoveAt(i);
    }
    m_nCurrentPage = -1;
    Invalidate();
}

void STabCtrl::OnMouseMove( UINT nFlags, CPoint point )
{
    CRect rcItem;
    int nOldHover=m_nHoverTabItem;
    m_nHoverTabItem=-1;
    int nTabCount=GetItemCount();
    for (int i = 0; i < nTabCount; i ++)
    {
        GetItemRect(i, rcItem);

        if (rcItem.PtInRect(point))
        {
            m_nHoverTabItem=i;
            break;
        }
    }
    if (m_nHoverTabItem != nOldHover)
    {
        if(nOldHover!=-1)
        {
            if(nOldHover!=m_nCurrentPage)
            {
                GetItemRect(nOldHover, rcItem);
                InvalidateRect(rcItem);
            }
            EventTabItemLeave evt(this);
            evt.iLeave = nOldHover;
            FireEvent(evt);
        }
        if(m_nHoverTabItem!=-1)
        {
            if(m_nHoverTabItem != m_nCurrentPage)
            {
                GetItemRect(m_nHoverTabItem, rcItem);
                InvalidateRect(rcItem);
            }
            EventTabItemHover evt(this);
            evt.iHover = m_nHoverTabItem;
            FireEvent(evt);
        }
    }
}

void STabCtrl::OnDestroy()
{
    for(int i = GetItemCount()-1; i>=0; i--)
    {
        DestroyChild(m_lstPages[i]);
    }
    m_lstPages.RemoveAll();
}


SWindow * STabCtrl::GetPage( int iPage )
{
    if( iPage < 0 || iPage>= (int)GetItemCount() ) return NULL;
    return m_lstPages[iPage];
}


int STabCtrl::GetPageIndex(LPCTSTR pszName,BOOL bTitle)
{
    if(bTitle)
    {
        for(UINT i=0;i<m_lstPages.GetCount();i++)
        {
            if(_tcscmp(m_lstPages[i]->GetTitle(),pszName)==0)
                return i;
        }
    }else
    {
        SStringW strName = S_CT2W(pszName);
        for(UINT i=0;i<m_lstPages.GetCount();i++)
        {
            if(m_lstPages[i]->GetName()==strName)
                return i;
        }
    }
    return -1;
}

SWindow * STabCtrl::GetPage( LPCTSTR pszName,BOOL bTitle/*=TRUE*/ )
{
    int iPage = GetPageIndex(pszName,bTitle);
    if(iPage == -1) return NULL;
    return m_lstPages[iPage];
}

void STabCtrl::OnSliderFinish()
{
    SASSERT(m_tabSlider);
    DestroyChild(m_tabSlider);
    m_tabSlider = NULL;
    STabPage * pPage = m_lstPages[m_nCurrentPage];
    SASSERT(pPage);
    pPage->SetVisible(TRUE,TRUE);
}

BOOL STabCtrl::SetCurSel( int nIndex )
{
    if( nIndex < 0 || nIndex> (int)GetItemCount()-1 || (m_nCurrentPage == nIndex)) return FALSE;
    int nOldPage = m_nCurrentPage;
    
    EventTabSelChanging evt(this);
    evt.uOldSel=nOldPage;
    evt.uNewSel=nIndex;

    FireEvent(evt);

    if (evt.bCancel)
        return FALSE;

    CRect rcItem;

    GetItemRect(m_nCurrentPage, rcItem);
    InvalidateRect(rcItem);
    
    if(m_tabSlider)
    {
        m_tabSlider->Stop();
    }
    if(m_nAnimateSteps && IsVisible(TRUE) && nOldPage!=-1 && nIndex !=-1)
    {
		m_tabSlider = new STabSlider(this, nOldPage, nIndex, m_nAnimateSteps,m_nAniamteType,m_aniInterpolator);
    }else
    {
        if(nOldPage!=-1)
        {
            GetItem(nOldPage)->SetVisible(FALSE,TRUE);
        }
        if(nIndex!=-1)
        {
            GetItem(nIndex)->SetVisible(TRUE,TRUE);
        }
    }
    m_nCurrentPage = nIndex;
    GetItemRect(m_nCurrentPage, rcItem);
    InvalidateRect(rcItem);
        
    EventTabSelChanged evt2(this);
    evt2.uNewSel=nIndex;
    evt2.uOldSel=nOldPage;

    FireEvent(evt2);
    return TRUE;
}

BOOL STabCtrl::SetCurSel( LPCTSTR pszName,BOOL bTitle/*=TRUE */)
{
    int iPage = GetPageIndex(pszName,bTitle);
    if(iPage == -1) return FALSE;
    return SetCurSel(iPage);
}

BOOL STabCtrl::SetItemTitle( int nIndex, LPCTSTR lpszTitle )
{
    STabPage* pTab = GetItem(nIndex);
    if (pTab)
    {
        pTab->SetTitle(lpszTitle);

        CRect rcTitle = GetTitleRect();
        InvalidateRect(rcTitle);
        return TRUE;
    }

    return FALSE;
}

BOOL STabCtrl::CreateChildren( pugi::xml_node xmlNode )
{
    for ( pugi::xml_node xmlChild = xmlNode.first_child(); xmlChild; xmlChild = xmlChild.next_sibling())
    {
        InsertItem(xmlChild,-1,TRUE);
    }
    
    if(m_nCurrentPage==-1 || m_nCurrentPage>=(int)m_lstPages.GetCount())
    {
        m_nCurrentPage=0;
    }
    if(m_lstPages.GetCount()==0)
    {
        m_nCurrentPage=-1;
    }
    
    if(m_nCurrentPage!=-1)
    {
        GetItem(m_nCurrentPage)->SetVisible(TRUE);
    }
    return TRUE;
}


STabPage * STabCtrl::CreatePageFromXml(pugi::xml_node xmlPage)
{
    if (wcscmp(xmlPage.name(),STabPage::GetClassName()) != 0) return NULL;
    return (STabPage *)SApplication::getSingleton().CreateWindowByName(STabPage::GetClassName());
}

int STabCtrl::InsertItem( LPCWSTR lpContent ,int iInsert/*=-1*/)
{
    pugi::xml_document xmlDoc;
    if(!xmlDoc.load_buffer(lpContent,wcslen(lpContent)*sizeof(wchar_t),pugi::parse_default,pugi::encoding_utf16)) return -1;
    return InsertItem(xmlDoc.first_child(),iInsert);
}

int STabCtrl::InsertItem( pugi::xml_node xmlNode,int iInsert/*=-1*/,BOOL bLoading/*=FALSE*/ )
{
    STabPage *pChild = CreatePageFromXml(xmlNode);
    if(!pChild) return -1;
    
    InsertChild(pChild);
    pChild->InitFromXml(xmlNode);
    pChild->GetLayoutParam()->SetMatchParent(Both);
    
    if(iInsert==-1) iInsert = (int)m_lstPages.GetCount();
    m_lstPages.InsertAt(iInsert,pChild);

    if(!bLoading )
    {
        CRect rcPage=GetChildrenLayoutRect();
        pChild->Move(&rcPage);
        pChild->SetVisible(FALSE,FALSE);
        if(m_nCurrentPage>=iInsert)  m_nCurrentPage++;
        InvalidateRect(GetTitleRect());
        if(m_nCurrentPage == -1) SetCurSel(iInsert);
    }
    OnItemInserted(pChild);
    return iInsert;
}


CRect STabCtrl::GetTitleRect()
{
    CRect rcTitle;
    GetClientRect(rcTitle);
    switch(m_nTabAlign)
    {
    case AlignTop:
        rcTitle.bottom = rcTitle.top+ m_szTab.cy;
        break;
    case AlignBottom:
        rcTitle.top = rcTitle.bottom- m_szTab.cy;
        break;
    case AlignLeft:
        rcTitle.right = rcTitle.left + m_szTab.cx;
        break;
    case AlignRight:
        rcTitle.left = rcTitle.right - m_szTab.cx;
        break;
    }
    return rcTitle;    
}

BOOL STabCtrl::GetItemRect( int nIndex, CRect &rcItem )
{
    if (nIndex < 0 || nIndex >= (int)GetItemCount())
        return FALSE;
    
    CRect rcTitle = GetTitleRect();
        
    rcItem = CRect(rcTitle.TopLeft(),m_szTab);

    switch (m_nTabAlign)
    {
    case AlignTop:
    case AlignBottom:
        rcItem.OffsetRect(m_nTabPos + nIndex * (rcItem.Width()+ m_nTabInterSize),0);
        break;
    case AlignLeft:
    case AlignRight:
        rcItem.OffsetRect(0, m_nTabPos + nIndex * (rcItem.Height()+ m_nTabInterSize));
        break;
    }
    rcItem.IntersectRect(rcItem,rcTitle);
    return TRUE;
}

STabPage* STabCtrl::GetItem( int nIndex )
{
    if(nIndex<0 || nIndex>= (int)GetItemCount()) return NULL;
    return m_lstPages[nIndex];
}

void STabCtrl::DrawItem(IRenderTarget *pRT,const CRect &rcItem,int iItem,DWORD dwState )
{
    if(rcItem.IsRectEmpty()) return;
    int iState = IIF_STATE3(dwState,WndState_Normal,WndState_Hover,WndState_PushDown);
    if(m_pSkinTab)
        m_pSkinTab->Draw(pRT,rcItem,iState);

    //根据状态从style中获得字体，颜色
    IFontPtr font=m_style.GetTextFont(iState);
    COLORREF crTxt = m_style.GetTextColor(iState);
    CAutoRefPtr<IFont> oldFont;
    if(font) pRT->SelectObject(font,(IRenderObj**)&oldFont);
    COLORREF crOld = 0;
    if(crTxt != CR_INVALID) crOld = pRT->SetTextColor(crTxt);
    
    CRect rcIcon(m_ptIcon+rcItem.TopLeft(),CSize(0,0));
    if(m_pSkinIcon)
    {
        rcIcon.right=rcIcon.left+m_pSkinIcon->GetSkinSize().cx;
        rcIcon.bottom=rcIcon.top+m_pSkinIcon->GetSkinSize().cy;
        int iIcon=GetItem(iItem)->GetIconIndex();
        if(iIcon == -1) iIcon = iItem;
        m_pSkinIcon->Draw(pRT,rcIcon,iIcon);
    }

    if(m_ptText.x!=-1 && m_ptText.y!=-1)
    {//从指定位置开始绘制文字
        if(m_txtDir == Text_Horz)
            pRT->TextOut(rcItem.left+m_ptText.x,rcItem.top+m_ptText.y,GetItem(iItem)->GetTitle(),-1);
        else
            TextOutV(pRT,rcItem.left+m_ptText.x,rcItem.top+m_ptText.y,GetItem(iItem)->GetTitle());
    }
    else
    {
        CRect rcText=rcItem;
        UINT alignStyle=m_style.GetTextAlign();
        UINT align=alignStyle;
        if(m_ptText.x==-1 && m_ptText.y!=-1)
        {//指定了Y偏移，X居中
            rcText.top+=m_ptText.y;
            align=alignStyle&(DT_CENTER|DT_RIGHT|DT_SINGLELINE|DT_END_ELLIPSIS);
        }
        else if(m_ptText.x!=-1 && m_ptText.y==-1)
        {//指定了X偏移，Y居中
            rcText.left+=m_ptText.x;
            align=alignStyle&(DT_VCENTER|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);
        }
        
        if(m_txtDir == Text_Horz)
            pRT->DrawText(GetItem(iItem)->GetTitle(),-1,&rcText,align);
        else
            DrawTextV(pRT,rcText,GetItem(iItem)->GetTitle());
    }
    
    //恢复字体，颜色
    if(font) pRT->SelectObject(oldFont);
    if(crTxt!=CR_INVALID) pRT->SetTextColor(crOld);
}


BOOL STabCtrl::OnUpdateToolTip( CPoint pt, SwndToolTipInfo & tipInfo )
{
    int iItem = HitTest(pt);
    if(iItem == -1) return FALSE;
    if(GetItem(iItem)->GetToolTipText().IsEmpty()) return FALSE;
    tipInfo.swnd = m_swnd;
    tipInfo.dwCookie = iItem;
    GetItemRect(iItem,tipInfo.rcTarget);
    tipInfo.strTip = GetItem(iItem)->GetToolTipText();
    return TRUE;
}


void STabCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    if(nChar==VK_LEFT || nChar==VK_UP)
    {
        if(!SetCurSel(m_nCurrentPage-1))
            SetCurSel(GetItemCount()-1);
    }else if(nChar==VK_RIGHT || nChar==VK_DOWN)
    {
        if(!SetCurSel(m_nCurrentPage+1))
            SetCurSel(0);
    }else if(nChar==VK_HOME)
    {
        SetCurSel(0);
    }else if(nChar==VK_END)
    {
        SetCurSel(GetItemCount()-1);
    }
}

int STabCtrl::HitTest( CPoint pt )
{
    int nTabCount=GetItemCount();
    for (int i = 0; i < nTabCount; i ++)
    {
        CRect rcItem;
        GetItemRect(i, rcItem);

        if (rcItem.PtInRect(pt))
        {
            return i;
        }
    }
    return -1;
}

void STabCtrl::OnInitFinished( pugi::xml_node xmlNode )
{
    if(m_pSkinTab)
    {
        SIZE sz = m_pSkinTab->GetSkinSize();
        if(m_szTab.cx == -1) m_szTab.cx = sz.cx;
        if(m_szTab.cy == -1) m_szTab.cy = sz.cy;
    }
}

void STabCtrl::UpdateChildrenPosition()
{
    CRect rcPage = GetChildrenLayoutRect();
    for(size_t i =0 ;i<m_lstPages.GetCount() ;i++)
    {
        m_lstPages[i]->Move(rcPage);
    }
}

void STabCtrl::BeforePaint( IRenderTarget *pRT, SPainter &painter )
{
    IFontPtr pFont = m_style.GetTextFont(0);
    if(pFont) 
        pRT->SelectObject(pFont,(IRenderObj**)&painter.oldFont);

    COLORREF crTxt = m_style.GetTextColor(0);
    if(crTxt != CR_INVALID)
        painter.oldTextColor = pRT->SetTextColor(crTxt);

}

void STabCtrl::TextOutV(IRenderTarget *pRT,int x,int y , const SStringT & strText)
{
    SStringT strTmp = strText;
    LPTSTR p = strTmp.GetBuffer(0);
    while(*p)
    {
        LPTSTR p2 = SStringT::_tchar_traits::CharNext(p);
        SIZE szWord;
        pRT->MeasureText(p,(int)(p2-p),&szWord);
        pRT->TextOut(x,y,p,(int)(p2-p));
        p = p2;
        y += szWord.cy;
    }
    strTmp.ReleaseBuffer();
}

SIZE STabCtrl::MeasureTextV(IRenderTarget *pRT, const SStringT & strText)
{
    SIZE szRet={0,0};
    SStringT strTmp = strText;
    LPTSTR p = strTmp.GetBuffer(0);
    while(*p)
    {
        LPTSTR p2 = SStringT::_tchar_traits::CharNext(p);
        SIZE szWord;
        pRT->MeasureText(p,(int)(p2-p),&szWord);
        szRet.cx = (std::max)(szRet.cx,szWord.cx);
        szRet.cy += szWord.cy;
        p = p2;
    }
    strTmp.ReleaseBuffer();

    return szRet;
}

void STabCtrl::DrawTextV(IRenderTarget *pRT, CRect rcText, const SStringT & strText)
{//only support horizontal and vertical center
    SIZE szText =  MeasureTextV(pRT,strText);
    TextOutV(pRT,
        rcText.left + (rcText.Width()-szText.cx)/2,
        rcText.top + (rcText.Height()-szText.cy)/2,
        strText
        );
}

void STabCtrl::OnColorize(COLORREF cr)
{
    __super::OnColorize(cr);
    if(m_pSkinIcon) m_pSkinIcon->OnColorize(cr);
    if(m_pSkinTab) m_pSkinTab->OnColorize(cr);
    if(m_pSkinTabInter) m_pSkinTabInter->OnColorize(cr);
    if(m_pSkinFrame) m_pSkinFrame->OnColorize(cr);
}

HRESULT STabCtrl::OnLanguageChanged()
{
	__super::OnLanguageChanged();
	for(int i=0;i<(int)m_lstPages.GetCount();i++)
	{
		STabPage *pPage = m_lstPages.GetAt(i);
		pPage->m_strTitle.TranslateText();
	}
	InvalidateRect(GetTitleRect());
	return HRESULT(3);
}

SOUI_ATTRS_BEGIN(STabPage)
	ATTR_I18NSTRT(L"title", m_strTitle, FALSE)
	ATTR_INT(L"iconIndex", m_iIcon, FALSE)
SOUI_ATTRS_END()


SOUI_CLASS_NAME(STabPage, L"page")

STabPage::STabPage() :m_strTitle(this), m_iIcon(-1)
{
	m_bVisible = FALSE;
}
/**
* STabPage::~STabPage
* @brief    析构函数
*
* Describe  析构函数
*/
STabPage::~STabPage()
{
}

BOOL STabPage::OnUpdateToolTip(CPoint pt, SwndToolTipInfo &tipInfo) 
{
	return FALSE; 
}


}//namespace SOUI