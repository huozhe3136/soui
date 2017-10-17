/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SwndContainerImpl.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    SOUI窗口容器的实现
*/

#pragma  once

#include "SDropTargetDispatcher.h"
#include "FocusManager.h"

namespace SOUI
{

    class SOUI_EXP SwndContainerImpl : public ISwndContainer
                                     , public SWindow
    {
        SOUI_CLASS_NAME_DECL(SwndContainerImpl,L"SwndContainerImpl")
    public:
        SwndContainerImpl();
		~SwndContainerImpl() override;

        IDropTarget * GetDropTarget(){return &m_dropTarget;}

        CFocusManager * GetFocusManager() {return &m_focusMgr;}
    protected:
        //ISwndContainer
        BOOL RegisterDragDrop(SWND swnd,IDropTarget *pDropTarget) override;

        BOOL RevokeDragDrop(SWND swnd) override;

        virtual LRESULT DoFrameEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

        BOOL OnReleaseSwndCapture() override;

        SWND OnSetSwndCapture(SWND swnd) override;
        void OnSetSwndFocus(SWND swnd) override;
		void FrameToHost(RECT & rc) override;
        SWND OnGetSwndCapture() override;

        SWND GetFocus() override;

        SWND GetHover() override;

		IAcceleratorMgr* GetAcceleratorMgr() override; 

        BOOL RegisterTimelineHandler(ITimelineHandler *pHandler) override;

        BOOL UnregisterTimelineHandler(ITimelineHandler *pHandler) override;

        BOOL RegisterTrackMouseEvent(SWND swnd) override;

        BOOL UnregisterTrackMouseEvent(SWND swnd) override;

        //标记窗口树的zorder失效
        void MarkWndTreeZorderDirty() override;

        //重建窗口树的zorder
        void BuildWndTreeZorder() override;

    public://ITimelineHandler
        void OnNextFrame() override;
    protected:

        void OnFrameMouseMove(UINT uFlag,CPoint pt);

        void OnFrameMouseLeave();

        BOOL OnFrameSetCursor(const CPoint &pt);

        void OnFrameMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

        void OnFrameMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam);

        void OnFrameKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

        void OnFrameKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

        void OnActivate(UINT nState);

        void OnActivateApp(BOOL bActive, DWORD dwThreadID);

        void _BuildWndTreeZorder(SWindow *pWnd,UINT &iOrder);
        
        
    protected:
        SWND m_hCapture;
        SWND m_hHover;
        BOOL    m_bNcHover;

        CFocusManager m_focusMgr;

        SDropTargetDispatcher m_dropTarget;

        BOOL        m_bZorderDirty;

        SList<ITimelineHandler*>    m_lstTimelineHandler;
        SList<SWND>                 m_lstTrackMouseEvtWnd;
    };

}//namespace SOUI

