/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SwndStyle.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    SOUI窗口风格管理
*/
#pragma once

#include "res.mgr/SDpiAwareFont.h"

namespace SOUI
{

	
class SWindow;
class SOUI_EXP SwndStyle: public SObject
{
    SOUI_CLASS_NAME_DECL(SwndStyle, L"style")

    enum
    {
        Align_Left               = 0x000UL, // valign = top
        Align_Center             = 0x100UL, // valign = middle
        Align_Right              = 0x200UL, // valign = bottom

        VAlign_Top               = 0x0000UL, // valign = top
        VAlign_Middle            = 0x1000UL, // valign = middle
        VAlign_Bottom            = 0x2000UL, // valign = bottom
    };
public:
    SwndStyle();
	~SwndStyle() override;

    COLORREF m_crBg;                /**<背景颜色 */
    COLORREF m_crBorder;            /**<边框颜色 */
    
    SStringT m_strCursor;           /**<光标NAME */

    BYTE     m_byAlpha;             /**<窗口透明度 */
    BYTE     m_bySepSpace;          /**<子窗口水平间隔 */
    DWORD    m_bDotted:1;           /**<支持省略号显示文本 */
    DWORD    m_bTrackMouseEvent:1;  /**<监视鼠标进入及移出消息 */
    DWORD    m_bBkgndBlend:1;       /**<渲染窗口内容和背景混合标志 */

    UINT GetTextAlign();
    int GetStates();
    COLORREF GetTextColor(int iState);
    IFontPtr GetTextFont(int iState);
	CRect GetMargin() const;
	CRect GetPadding() const;
    void SetTextColor(int iState,COLORREF cr){m_crText[iState]=cr;}

	void SetScale(int nScale);
protected:
	SLayoutSize    m_rcMargin[4];   /**< 4周非客户区大小 */
	SLayoutSize    m_rcInset[4];    /**< 文字区4个方向的内边距 */

    UINT m_nTextAlign;      /**<文本对齐 */
    UINT m_uAlign,m_uVAlign;/**<水平及垂直对齐 */
    COLORREF m_crText[4];   /**<文字4种状态下的颜色 */
    SDpiAwareFont m_ftText[4];  /**<文字4种状态下的字体 */

	int GetScale() const {return m_nScale;}

	int		m_nScale;

	HRESULT OnAttrPadding(const SStringW & strValue,BOOL bLoading);
    HRESULT OnAttrMargin(const SStringW &strValue,BOOL bLoading);
    HRESULT OnAttrMarginX(const SStringW &strValue,BOOL bLoading);
    HRESULT OnAttrMarginY(const SStringW &strValue,BOOL bLoading);
    
	void _ParseLayoutSize4(const SStringW & strValue, SLayoutSize layoutSizes[]);
    
	SOUI_ATTRS_DECL()
};


}//namespace SOUI