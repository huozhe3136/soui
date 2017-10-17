/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SDropTargetDispatcher.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    dragdrop分发模块
*/

#pragma once
#include "swnd.h"

namespace SOUI{

    /**
    * @class      SDropTargetDispatcher
    * @brief      管理DropTarget在DUI窗口之间的分发。
    * 
    * Describe    
    */
    class SDropTargetDispatcher : public IDropTarget
    {
    public:
        SDropTargetDispatcher(SWindow * pFrame);
        ~SDropTargetDispatcher(void);

        BOOL RegisterDragDrop(SWND swnd,IDropTarget *pDropTarget);
        BOOL RevokeDragDrop(SWND swnd);


        //////////////////////////////////////////////////////////////////////////
        // IUnknown
        HRESULT STDMETHODCALLTYPE QueryInterface( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */  void __RPC_FAR *__RPC_FAR *ppvObject) override;

		ULONG STDMETHODCALLTYPE AddRef(void) override;

		ULONG STDMETHODCALLTYPE Release(void) override;

        //////////////////////////////////////////////////////////////////////////
        // IDropTarget

        HRESULT STDMETHODCALLTYPE DragEnter( 
            /* [unique][in] */ IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect)  override;

        HRESULT STDMETHODCALLTYPE DragOver( 
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect) override;

        HRESULT STDMETHODCALLTYPE DragLeave( void) override;

        HRESULT STDMETHODCALLTYPE Drop( 
            /* [unique][in] */  IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */  DWORD *pdwEffect) override;

    protected:
        POINT PointL2FrameClient(const POINTL & pt);


        typedef SMap<SWND,IDropTarget *> DTMAP;
        DTMAP m_mapDropTarget;
        IDataObject *m_pDataObj;
        SWND         m_hHover;
        SWindow     *m_pOwner;
    };

}
