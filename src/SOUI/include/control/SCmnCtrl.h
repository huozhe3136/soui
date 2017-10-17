﻿/**
 * Copyright (C) 2014-2050 SOUI团队
 * All rights reserved.
 * 
 * @file       SCmnCtrl.h
 * @brief      通用控件
 * @version    v1.0      
 * @author     soui      
 * @date       2014-05-28
 * 
 * Describe    此文件中定义了很多通用控件:静态文本，超链接，按钮，单选按钮等
 */

#pragma once
#include "core/SWnd.h"
#include "core/Accelerator.h"

namespace SOUI
{

/**
 * @class      SStatic
 * @brief      静态文本控件类
 * 
 * Describe    静态文本控件可支持多行，有多行属性时，\n可以强制换行
 * Usage       <text>inner text example</text>
 */
class SOUI_EXP SStatic : public SWindow
{
    SOUI_CLASS_NAME_DECL(SStatic, L"text")
public:
    /**
     * SStatic::SStatic
     * @brief    构造函数
     *
     * Describe  构造函数
     */
	SStatic();
    /**
     * SStatic::SDrawText
     * @brief    绘制文本
     * @param    IRenderTarget *pRT -- 绘制设备句柄         
     * @param    LPCTSTR pszBuf -- 文本内容字符串         
     * @param    int cchText -- 字符串长度         
     * @param    LPRECT pRect -- 指向矩形结构RECT的指针         
     * @param    UINT uFormat --  正文的绘制选项         
     *
     * Describe  对DrawText封装
     */    
    void DrawText(IRenderTarget *pRT,LPCTSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat) override;

protected:

    void DrawMultiLine(IRenderTarget *pRT,LPCTSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat);
    
    int m_bMultiLines;  /**< 是否开启多行显示 */  
    int m_nLineInter;   /**< 行间距 */

    SOUI_ATTRS_DECL() 
};

/**
 * @class      SLink
 * @brief      超链接控件类
 * 
 * Describe    Only For Header Drag Test
 * Usage       <link>inner text example</link>
 */
class SOUI_EXP SLink : public SWindow
{
    SOUI_CLASS_NAME_DECL(SLink, L"link")

public:
    /**
     * SLink::SLink
     * @brief    构造函数
     *
     * Describe  构造函数
     */
	SLink();
	~SLink() override;
protected:
    /**
     * SLink::OnAttributeFinish
     * @brief    解析xml设置属性
     *
     * Describe  根据xml文件设置相关属性
     */
    void OnInitFinished(pugi::xml_node xmlNode) override;
    /**
     * SLink::SDrawText
     * @brief    绘制文本
     * @param    IRenderTarget *pRT -- 绘制设备句柄         
     * @param    LPCTSTR pszBuf -- 文本内容字符串         
     * @param    int cchText -- 字符串长度         
     * @param    LPRECT pRect -- 指向矩形结构RECT的指针         
     * @param    UINT uFormat --  正文的绘制选项         
     *
     * Describe  对DrawText封装
     */
    void DrawText(IRenderTarget *pRT,LPCTSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat) override;

    /**
     * SLink::OnSetCursor
     * @brief    设置光标样式和位置
     * @param    CPoint &pt -- 设置光标位置
     * @return   返回值BOOL 成功--TRUE 失败--FALSE
     *
     * Describe  函数内部会加载光标样式
     */
    BOOL OnSetCursor(const CPoint &pt) override;

    void OnLButtonDown(UINT nFlags,CPoint pt);
    void OnLButtonUp(UINT nFlags,CPoint pt);
    void OnMouseMove(UINT nFlags,CPoint pt);
    void OnMouseHover(WPARAM wParam, CPoint ptPos);

    SOUI_ATTRS_DECL() 
    
    SOUI_MSG_MAP_DECL() 

protected:
    CRect m_rcText;  /**< 文本显示所在位置 */
    SStringT m_strLinkUrl;  /**< 窗口URL */
};

/**
 * @class      SButton
 * @brief      按钮控件类
 * 
 * Describe    通过属性ID绑定click事件 Use id attribute to process click event
 * Usage       <button id=xx>inner text example</button>
 */
class SOUI_EXP SButton : public SWindow
    , public IAcceleratorTarget
    , public ITimelineHandler
{
    SOUI_CLASS_NAME_DECL(SButton, L"button")
public:
    /**
     * SButton::SButton
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SButton();

public:
        /**
     * SButton::GetDesiredSize
     * @brief    获得期望的大小值
     * @param    LPRECT pRcContainer -- 内容窗体矩形
     *
     * Describe  根据内容窗体矩形大小，计算出适合的大小
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;

protected:
    /**
     * SButton::NeedRedrawWhenStateChange
     * @brief    状态变化需要重画
     * @return   返回值BOOL 成功--TRUE 失败--FALSE
     *
     * Describe  当按钮状态发生变化时候需要重新绘制 默认返回TRUE
     */
	BOOL NeedRedrawWhenStateChange() override;
    /**
     * SButton::OnGetDlgCode
     * @brief    获得编码
     *
     * Describe  返回宏定义SC_WANTCHARS代表需要WM_CHAR消息
     */
	UINT OnGetDlgCode()  override;

    /**
     * SButton::OnAcceleratorPressed
     * @brief    加速键按下
     * @param    CAccelerator& accelerator -- 加速键相关结构体
     * @return   返回值BOOL 成功--TRUE 失败--FALSE
     *
     * Describe  处理加速键响应消息
     */
    bool OnAcceleratorPressed(const CAccelerator& accelerator) override;

protected:
    
    /**
     * SButton::OnStateChanged
     * @brief    状态改变处理函数
     * @param    DWORD dwOldState -- 旧状态
     * @param    DWORD dwNewState -- 新状态
     *
     * Describe  状态改变处理函数
     */
    void OnStateChanged(DWORD dwOldState,DWORD dwNewState) override;
    
    void OnPaint(IRenderTarget *pRT);

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    void OnDestroy();

    void OnSize(UINT nType, CSize size);

    BOOL OnEraseBkgnd(IRenderTarget * pRT){return TRUE;}

    HRESULT OnAttrAccel(SStringW strAccel,BOOL bLoading);

protected:
    void OnNextFrame() override;
    
    /**
     * SLink::StopCurAnimate
     * @brief    停止动画
     *
     * Describe  停止动画
     */
    void StopCurAnimate();

    DWORD  m_accel;
    BOOL   m_bAnimate;    /**< 动画标志 */
    BYTE   m_byAlphaAni;  /**< 动画状态 */
public:
    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
};

/**
 * @class      SImageButton
 * @brief      图片按钮类
 * 
 * Describe    图片按钮类，继承SButton
 */
class SOUI_EXP SImageButton : public SButton
{
    SOUI_CLASS_NAME_DECL(SImageButton, L"imgbtn")
public:
	SImageButton();
};

/**
 * @class      SImageWnd
 * @brief      图片控件类
 * 
 * Describe    Image Control 图片控件类
 * Usage       Usage: <img skin="skin" sub="0"/>
 */
class SOUI_EXP SImageWnd : public SWindow
{
    SOUI_CLASS_NAME_DECL(SImageWnd, L"img")
public:
    /**
     * SImageWnd::SImageWnd
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SImageWnd();
    
    /**
     * SImageWnd::~SImageWnd
     * @brief    析构函数
     *
     * Describe  析构函数
     */
    ~SImageWnd() override;

    void OnPaint(IRenderTarget *pRT);
    
    /**
     * SImageWnd::SetSkin
     * @param    ISkinObj *pSkin -- 资源
     * @param    int iFrame -- 皮肤中的帧号
     * @param    BOOL bAutoFree -- 资源ID
     * @brief    设置皮肤
     * @return   返回值BOOL 成功--TRUE 失败--FALSE
     *
     * Describe  设置皮肤
     */
    BOOL SetSkin(ISkinObj *pSkin,int iFrame=0,BOOL bAutoFree=TRUE);
    
     /**
     * SImageWnd::SetImage
     * @param    IBitmap * pBitmap -- 图片对象
     * @param    FilterLevel fl -- FilterLevel
     * @return   void
     *
     * Describe  设置绘制图片
     */
    void SetImage(IBitmap * pBitmap,FilterLevel fl=kNone_FilterLevel);
    
    /**
     * SImageWnd::SetIcon
     * @param    int nSubID -- 资源ID
     * @brief    设置图标
     * @return   返回值BOOL 成功--TRUE 失败--FALSE
     *
     * Describe  设置图标
     */
    BOOL SetIcon(int nSubID);

    /**
     * SImageWnd::GetSkin
     * @brief    获取资源
     * @return   返回值ISkinObj指针
     *
     * Describe  获取资源
     */
    ISkinObj * GetSkin(){return m_pSkin;}
protected:
    void OnColorize(COLORREF cr) override;
    
	void OnScaleChanged(int scale) override;
    /**
     * SImageWnd::GetDesiredSize
     * @brief    获取预期大小
     * @param    LPRECT pRcContainer  --  内容矩形框 
     * @return   返回值 CSize对象 
     *
     * Describe  根据矩形的大小，获取预期大小(解释有点不对)
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;

	int m_iTile;	//绘制是否平铺,0--位伸（默认），1--不变常规绘制, 2--平铺
    BOOL m_bManaged;  /**< 是否要自动释放当前的m_pSkin对象 */
    int m_iFrame;  /**< 资源图片ID */
    ISkinObj *m_pSkin;  /**< ISkinObj对象 */
    CAutoRefPtr<IBitmap>    m_pImg;//使用代码设定的图片
    FilterLevel             m_fl;

	SOUI_ATTRS_DECL()


    SOUI_MSG_MAP_DECL() 
};

/**
 * @class      SAnimateImgWnd
 * @brief      动画图片窗口
 * 
 * Describe    此窗口支持动画效果
 */
class SOUI_EXP SAnimateImgWnd : public SWindow, public ITimelineHandler
{
    SOUI_CLASS_NAME_DECL(SAnimateImgWnd, L"animateimg")
public:    
    /**
     * SAnimateImgWnd::SAnimateImgWnd
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SAnimateImgWnd();

    /**
     * SAnimateImgWnd::~SAnimateImgWnd
     * @brief    析构函数
     *
     * Describe  析构函数
     */    
	~SAnimateImgWnd()  override;

    /**
     * SAnimateImgWnd::Start
     * @brief    启动动画
     *
     * Describe  启动动画
     */  
    void Start();
    /**
     * SAnimateImgWnd::Stop
     * @brief    停止动画
     *
     * Describe  停止动画
     */  
    void Stop();

    /**
     * SAnimateImgWnd::IsPlaying
     * @brief    判断动画运行状态
     * @return   返回值是动画状态 TRUE -- 运行中
     *
     * Describe  判断动画运行状态
     */  
    BOOL IsPlaying(){return m_bPlaying;}
protected:
    /**
     * SAnimateImgWnd::GetDesiredSize
     * @brief    获取预期大小
     * @param    LPRECT pRcContainer  --  内容矩形框 
     * @return   返回值 CSize对象 
     *
     * Describe  根据矩形的大小，获取预期大小(解释有点不对)
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
    void OnNextFrame() override;
    void OnColorize(COLORREF cr) override;
    
    void OnPaint(IRenderTarget *pRT);

    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnDestroy();

    SOUI_MSG_MAP_DECL() 

    SOUI_ATTRS_DECL() 

protected:
    ISkinObj     *m_pSkin;        /**< 动画图片 */
    int           m_nSpeed;       /**< 速度 */
    int           m_iCurFrame;    /**< 当前帧 */
    BOOL          m_bAutoStart;   /**< 是否自动启动 */
    BOOL          m_bPlaying;     /**< 是否运行中 */
    int           m_iTimeFrame;   /**< OnNextFrame的执行次数 */
	int			  m_nRepeat;	  /**< 播放循环次数,-1代表无限循环 */
	int			  m_iRepeat;	  /**< 当前播放循环轮次 */
};

/**
 * @class      SProgress
 * @brief      进度条类
 * 
 * Describe    进度条类
 * Usage: <progress bgskin=xx posskin=xx min=0 max=100 value=10,showpercent=0/>
 */
class SOUI_EXP SProgress : public SWindow
{
    SOUI_CLASS_NAME_DECL(SProgress, L"progress")
public:
    /**
     * SProgress::SProgress
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SProgress();

    /**
     * SProgress::SetValue
     * @brief    设置进度条进度值
     * @param    int nValue  --  进度值 
     * @return   返回值是 TRUE -- 设置成功
     *
     * Describe  设置进度条进度值
     */  
    BOOL SetValue(int nValue);
    /**
     * SProgress::GetValue
     * @brief    获取进度值
     * @return   返回值是int 
     *
     * Describe  获取进度值
     */  
    int GetValue(){return m_nValue;}

    /**
     * SProgress::SetRange
     * @param    int nMin  --  进度最小值 
     * @param    int nMax  --  进度最大值      
     * @brief    设置进度值最小大值
     *
     * Describe  设置进度值
     */  
    void SetRange(int nMin,int nMax);
    /**
     * SProgress::GetRange
     * @param    int nMin  --  进度最小值 
     * @param    int nMax  --  进度最大值      
     * @brief    获取进度值最小大值
     *
     * Describe  获取进度值
     */  
    void GetRange(int *pMin,int *pMax);
    /**
     * SProgress::IsVertical
     * @brief    判断进度条是否为竖直状态
     * @return   返回值是 TRUE -- 竖直状态
     *
     * Describe  获取进度值
     */  
    BOOL IsVertical(){return m_bVertical;}
protected:
    /**
     * SProgress::GetDesiredSize
     * @brief    获取预期大小
     * @param    LPRECT pRcContainer  --  内容矩形框 
     * @return   返回值 CSize对象 
     *
     * Describe  根据矩形的大小，获取预期大小(解释有点不对)
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
    void OnColorize(COLORREF cr) override;
    
    void OnPaint(IRenderTarget *pRT);
    int  OnCreate(void*);

    int m_nMinValue; /**< 进度最小值 */
    int m_nMaxValue; /**< 进度最大值 */
    int m_nValue;    /**< 进度值 */

    BOOL m_bShowPercent; /**< 是否显示百分比 */
    BOOL m_bVertical;    /**< 是否竖直状态 */

    ISkinObj *m_pSkinBg;   /**< 背景资源 */
    ISkinObj *m_pSkinPos;  /**< 前景资源 */

    SOUI_MSG_MAP_DECL() 

    SOUI_ATTRS_DECL() 
};

/**
 * @class      SLine
 * @brief      线条控件
 * 
 * Describe    线条控件
 * Usage: <hr size=1 mode="vert" lineStyle="dash"/>
 */
class SOUI_EXP SLine : public SWindow
{
    SOUI_CLASS_NAME_DECL(SLine, L"hr")

public:
    /**
     * SLine::SLine
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SLine();

    void OnPaint(IRenderTarget *pRT);
protected:
    int m_nLineStyle;
    int m_nLineSize;
    
    COLORREF    m_crLine;
    
    enum HRMODE{
        HR_HORZ=0,
        HR_VERT,
        HR_TILT,
    }m_mode;

    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
};

/**
 * @class      SCheckBox
 * @brief      复选框控件
 * 
 * Describe    复选框控件
 * Usage: <check checked="1">This is a check-box</check>
 */
class SOUI_EXP SCheckBox : public SWindow
{
    SOUI_CLASS_NAME_DECL(SCheckBox, L"check")

    enum
    {
        CheckBoxSpacing = 4,
    };

public:
    /**
     * SCheckBox::SCheckBox
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SCheckBox();

protected:

    ISkinObj *m_pSkin;   /**< 状态图片资源 */
    ISkinObj *m_pFocusSkin; /**< 焦点状态资源 */
    /**
     * SCheckBox::_GetDrawState
     * @brief    获得复选框状态
     * @return   返回状态值
     *
     * Describe  获取复选框状态
     */
    UINT _GetDrawState();
    /**
     * SCheckBox::GetCheckRect
     * @brief    获得复选框矩形
     * @return   返回值CRect矩形框
     *
     * Describe  获取复选框矩形
     */
    CRect GetCheckRect();
    /**
     * SCheckBox::GetDesiredSize
     * @brief    获取预期大小
     * @param    LPRECT pRcContainer  --  内容矩形框 
     * @return   返回值 CSize对象 
     *
     * Describe  根据矩形的大小，获取预期大小(解释有点不对)
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
    /**
     * SCheckBox::GetTextRect
     * @brief    获取文本大小
     * @param    LPRECT pRect  --  内容矩形框 
     *
     * Describe  设置矩形的大小
     */
    void GetTextRect(LPRECT pRect) override;
    /**
     * SCheckBox::NeedRedrawWhenStateChange
     * @brief    判断状态改变是否需要重画
     * @return   返回值 BOOL 
     *
     * Describe  状态改变是否需要重画
     */
	BOOL NeedRedrawWhenStateChange() override;

    /**
     * SCheckBox::OnGetDlgCode
     * @brief    返回对应消息码
     * @return   返回值 UINT 
     *
     * Describe  返回对应消息码
     */
	UINT OnGetDlgCode() override;
    /**
     * SCheckBox::DrawFocus
     * @brief    绘制获取焦点
     * @param    IRenderTarget *pRT  --  设备句柄
     *
     * Describe  返回对应消息码
     */
    void DrawFocus(IRenderTarget *pRT) override;

    void OnColorize(COLORREF cr) override;
    
	void OnScaleChanged(int nScale) override;
	
    void OnLButtonUp(UINT nFlags, CPoint point);

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    
    void OnPaint(IRenderTarget *pRT);

    HRESULT OnAttrCheck(const SStringW& strValue, BOOL bLoading);
    
    SOUI_ATTRS_DECL()


    SOUI_MSG_MAP_DECL() 
};

/**
 * @class      SIconWnd
 * @brief      图标控件
 * 
 * Describe    图标控件
 * Usage: <icon src="icon:16" />
 */
class SOUI_EXP SIconWnd : public SWindow
{
    SOUI_CLASS_NAME_DECL(SIconWnd, L"icon")
public:    
    /**
     * SIconWnd::SIconWnd
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SIconWnd();
    ~SIconWnd() override;

    void OnPaint(IRenderTarget *pRT);
    /**
     * SIconWnd::AttachIcon
     * @brief    附加图标资源
     * @param    HICON hIcon -- 图标资源句柄
     * @return   返回值 HICON 
     *
     * Describe  附加图标资源
     */
    void SetIcon(HICON hIcon);
protected:
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
    HICON m_theIcon; /**< 图标资源句柄 */

    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
};


/**
 * @class      SRadioBox
 * @brief      单选框控件
 * 
 * Describe    单选框控件
 * Usage: <radio checked="1">This is a check-box</radio>
 */
class SOUI_EXP SRadioBox : public SWindow
{
    SOUI_CLASS_NAME_DECL(SRadioBox, L"radio")

    enum
    {
        RadioBoxSpacing = 4,
    };

public:
    /**
     * SRadioBox::SRadioBox
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SRadioBox();


protected:

    ISkinObj *m_pSkin;  /**< 皮肤资源 */
    ISkinObj *m_pFocusSkin; /**< 焦点皮肤资源 */
    /**
     * SRadioBox::SRadioBox
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    UINT _GetDrawState(); 
    /**
     * SRadioBox::SRadioBox
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    CRect GetRadioRect();
    
    /**
     * SRadioBox::GetTextRect
     * @brief    获得文本大小
     * @param    LPRECT pRect -- 文本大小Rect
     *
     * Describe  构造函数
     */
    void GetTextRect(LPRECT pRect) override;
    /**
     * SRadioBox::GetDesiredSize
     * @brief    获取预期大小值
     * @param    LPRECT pRcContainer -- 内容窗口Rect
     *
     * Describe  获取预期大小值
     */
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
    /**
     * SRadioBox::NeedRedrawWhenStateChange
     * @brief    当状态改变时候是否需要重绘
     * @return   返回BOOL
     *
     * Describe  当状态改变时候是否需要重绘
     */
    BOOL NeedRedrawWhenStateChange() override;
    /**
     * SRadioBox::DrawFocus
     * @brief    绘制焦点样式
     * @param    IRenderTarget *pRT -- 绘制设备
     *
     * Describe  当获得焦点时候需要绘制
     */
    void DrawFocus(IRenderTarget *pRT) override;
    /**
     * SRadioBox::OnGetDlgCode
     * @brief    获取消息编码
     * @return   返回编码值
     *
     * Describe  获取消息编码
     */
	UINT OnGetDlgCode() override;
    /**
     * SRadioBox::IsSiblingsAutoGroupped
     * @brief    是否自动添加到同一组
     * @return   返回BOOL 
     *
     * Describe  相同名称的单选按钮是否自动添加到同一组中
     */
	BOOL IsSiblingsAutoGroupped() override;
    
    
    /**
     * SRadioBox::GetSelectedSiblingInGroup
     * @brief    获取一个group中有选中状态的兄弟
     * @return   返回选中状态的兄弟窗口指针
     *
     * Describe  没有选中状态兄弟时返回NULL
     */
    SWindow * GetSelectedSiblingInGroup() override;
    
    
    /**
     * OnStateChanging
     * @brief    状态发生变化
     * @param    DWORD dwOldState --  原状态
     * @param    DWORD dwNewState --  新状态
     * @return   void 
     *
     * Describe  
     */
    void OnStateChanging(DWORD dwOldState,DWORD dwNewState) override;

    void OnColorize(COLORREF cr) override;

	void OnScaleChanged(int nScale) override;
protected:
    void OnSetFocus(SWND wndOld);
    
    void OnLButtonUp(UINT nFlags,CPoint pt);

    void OnPaint(IRenderTarget *pRT);

    HRESULT OnAttrCheck(const SStringW& strValue, BOOL bLoading);

    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
};

/**
 * @class      SToggle
 * @brief      Toggle控件
 * 
 * Describe    Toggle控件
 */
class SOUI_EXP SToggle : public SWindow
{
    SOUI_CLASS_NAME_DECL(SToggle, L"toggle")
public:
    
    /**
     * SToggle::SToggle
     * @brief    构造函数
     *
     * Describe  构造函数
     */
    SToggle();
    /**
     * SToggle::SetToggle
     * @brief    设置Toggle属性
     * @param    BOOL bToggle -- 是否启用Toggle特效         
     * @param    BOOL bUpdate -- 是否更新 默认值TRUE
     *
     * Describe  设置Toggle属性
     */
    void SetToggle(BOOL bToggle,BOOL bUpdate=TRUE);
    /**
     * SToggle::GetToggle
     * @brief    获取Toggle属性
     * @return   返回值BOOL        
     *
     * Describe  获取Toggle属性 主要是获取是否Toggle
     */
    BOOL GetToggle();
protected:
    void OnPaint(IRenderTarget *pRT);
    void OnLButtonUp(UINT nFlags,CPoint pt);
    CSize GetDesiredSize(LPCRECT pRcContainer) override;
	BOOL NeedRedrawWhenStateChange() override;
    void OnColorize(COLORREF cr) override;
    
    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
protected:
    BOOL m_bToggled;
    ISkinObj *m_pSkin;
};

/**
 * @class      SGroup
 * @brief      组控件
 * 
 * Describe    组控件
 * Usage       <group colorLine1="#b8d5e2" colorLine2="#999999">group text</>
 */
class SOUI_EXP SGroup : public SWindow
{
    SOUI_CLASS_NAME_DECL(SGroup, L"group")
public:
    SGroup();

protected:
    CRect GetChildrenLayoutRect() override;
	CSize GetDesiredSize(int nParentWid, int nParentHei) override;

    void OnPaint(IRenderTarget *pRT);
    
    COLORREF m_crLine1,m_crLine2; /**< group 3D显示使用的两种颜色 */
	SLayoutSize m_nRound; /**< 圆角半径 */
    SLayoutSize m_nHeaderHeight; /**< 头部高度 */
public:
    SOUI_ATTRS_DECL() 

    SOUI_MSG_MAP_DECL() 
};

}//namespace SOUI
