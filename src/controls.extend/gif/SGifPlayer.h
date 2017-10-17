﻿#pragma once
#include <core/swnd.h>

#include "sskingif.h"
#include "SSkinAPNG.h"

namespace SOUI
{

    /**
    * @class     SGifPlayer
    * @brief     GIF图片显示控件
    * 
    * Describe
    */
    class SGifPlayer : public SWindow , public ITimelineHandler
    {
        SOUI_CLASS_NAME_DECL(SGifPlayer, L"gifplayer")   //定义GIF控件在XM加的标签
    public:
        SGifPlayer();
        ~SGifPlayer();

        /**
         * PlayGifFile
         * @brief    在控件中播放一个GIF图片文件
         * @param    LPCTSTR pszFileName --  文件名
         * @return   BOOL -- true:成功
         * Describe  
         */    
        BOOL PlayGifFile(LPCTSTR pszFileName);

        /**
        * PlayAPNGFile
        * @brief    在控件中播放一个APNG图片文件
        * @param    LPCTSTR pszFileName --  文件名
        * @return   BOOL -- true:成功
        * Describe  
        */    
        BOOL PlayAPNGFile(LPCTSTR pszFileName);

    protected://SWindow的虚函数
        virtual CSize GetDesiredSize(LPCRECT pRcContainer);
    protected://ITimerLineHander
        virtual void OnNextFrame();

    public://属性处理
        SOUI_ATTRS_DECL()
    protected:
        HRESULT OnAttrSkin(const SStringW & strValue, BOOL bLoading);
        
    protected:
        BOOL _PlayFile(LPCTSTR pszFileName, BOOL bGif);
    protected://消息处理，SOUI控件的消息处理和WTL，MFC很相似，采用相似的映射表，相同或者相似的消息映射宏
        
        /**
         * OnPaint
         * @brief    窗口绘制消息响应函数
         * @param    IRenderTarget * pRT --  绘制目标
         * @return   void
         * Describe  注意这里的参数是IRenderTarget *,而不是WTL中使用的HDC，同时消息映射宏也变为MSG_WM_PAINT_EX
         */    
        void OnPaint(IRenderTarget *pRT);

        /**
         * OnShowWindow
         * @brief    处理窗口显示消息
         * @param    BOOL bShow --  true:显示
         * @param    UINT nStatus --  显示原因
         * @return   void 
         * Describe  参考MSDN的WM_SHOWWINDOW消息
         */    
        void OnShowWindow(BOOL bShow, UINT nStatus);
    
        void OnDestroy();

        //SOUI控件消息映射表
        SOUI_MSG_MAP_DECL()	

    protected:
        SSkinAni *m_aniSkin;
        int	m_iCurFrame;
        int     m_nNextInterval;
    };

}