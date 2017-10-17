//////////////////////////////////////////////////////////////////////////
//   File Name: SwndStyle.h
//////////////////////////////////////////////////////////////////////////

#include "souistd.h"
#include "core/Swndstyle.h"
#include "core/Swnd.h"
#include "helper/SplitString.h"
#include <algorithm>

namespace SOUI
{

SOUI_CLASS_NAME(SwndStyle, L"style")

SwndStyle::SwndStyle()
    : m_crBg(CR_INVALID)
	, m_crBorder(CR_INVALID)
	, m_strCursor(_T("arrow"))
	, m_byAlpha(0xFF)
	, m_bySepSpace(5)
	, m_bDotted(0)
	, m_bTrackMouseEvent(0)
	, m_bBkgndBlend(1)
	, m_nTextAlign(0)
	, m_uAlign(Align_Center)
	, m_uVAlign(VAlign_Middle)
	, m_nScale(100)
{
    for(int i=0; i<4; i++)
    {
        m_crText[i]=CR_INVALID;
    }
}

SwndStyle::~SwndStyle()
{

}

SOUI_ATTRS_BEGIN(SwndStyle)
	ATTR_HEX(L"textMode", m_nTextAlign, TRUE)

	ATTR_ENUM_BEGIN(L"align", UINT, TRUE)
	ATTR_ENUM_VALUE(L"left", Align_Left)
	ATTR_ENUM_VALUE(L"center", Align_Center)
	ATTR_ENUM_VALUE(L"right", Align_Right)
	ATTR_ENUM_END(m_uAlign)
	ATTR_ENUM_BEGIN(L"valign", UINT, TRUE)
	ATTR_ENUM_VALUE(L"top", VAlign_Top)
	ATTR_ENUM_VALUE(L"middle", VAlign_Middle)
	ATTR_ENUM_VALUE(L"bottom", VAlign_Bottom)
	ATTR_ENUM_END(m_uVAlign)

	ATTR_COLOR(L"colorBkgnd", m_crBg, TRUE)
	ATTR_COLOR(L"colorBorder", m_crBorder, TRUE)

	ATTR_FONT(L"font", m_ftText[0], TRUE)
	ATTR_FONT(L"fontHover", m_ftText[1], TRUE)
	ATTR_FONT(L"fontPush", m_ftText[2], TRUE)
	ATTR_FONT(L"fontDisable", m_ftText[3], TRUE)

	ATTR_COLOR(L"colorText", m_crText[0], TRUE)
	ATTR_COLOR(L"colorTextHover", m_crText[1], TRUE)
	ATTR_COLOR(L"colorTextPush", m_crText[2], TRUE)
	ATTR_COLOR(L"colorTextDisable", m_crText[3], TRUE)

	ATTR_CUSTOM(L"margin-x", OnAttrMarginX)
	ATTR_CUSTOM(L"margin-y", OnAttrMarginY)
	ATTR_CUSTOM(L"margin", OnAttrMargin)
	ATTR_CUSTOM(L"inset", OnAttrPadding)
	ATTR_CUSTOM(L"padding", OnAttrPadding)
	ATTR_STRINGT(L"cursor", m_strCursor, FALSE)
	ATTR_INT(L"dotted", m_bDotted, FALSE)
	ATTR_INT(L"trackMouseEvent", m_bTrackMouseEvent, FALSE)
	ATTR_INT(L"alpha", m_byAlpha, TRUE)
	ATTR_INT(L"bkgndBlend", m_bBkgndBlend, TRUE)
	ATTR_INT(L"sepSpace", m_bySepSpace, TRUE)
SOUI_ATTRS_BREAK()      //属性不交给SObject处理

UINT SwndStyle::GetTextAlign()
{
    if(m_nTextAlign) return m_nTextAlign;
    UINT uRet=0;
    switch(m_uAlign)
    {
    case Align_Center: uRet |= DT_CENTER;break;
    case Align_Right: uRet|= DT_RIGHT;break;
    }
    switch(m_uVAlign)
    {
    case VAlign_Middle:uRet |= DT_VCENTER|DT_SINGLELINE;break;
    case VAlign_Bottom:uRet|= DT_BOTTOM|DT_SINGLELINE;break;
    }
    if(m_bDotted) uRet|=DT_END_ELLIPSIS;
    return uRet;
}

int SwndStyle::GetStates()
{
    int fonts=1,colors=1;
    for(int i=1;i<4;i++)
    {
        if(m_ftText[i].GetFontPtr()!=NULL) fonts++;
        if(m_crText[i]!=CR_INVALID) colors++;
    }
    return (std::max)(fonts,colors);
}

COLORREF SwndStyle::GetTextColor( int iState )
{
    if(m_crText[iState]==CR_INVALID) iState=0;
    return m_crText[iState];
}

IFontPtr SwndStyle::GetTextFont( int iState )
{
    if(!m_ftText[iState].GetFontPtr()) iState=0;
    return m_ftText[iState].GetFontPtr();
}

HRESULT SwndStyle::OnAttrMargin(const SStringW& strValue,BOOL bLoading)
{
	_ParseLayoutSize4(strValue, m_rcMargin);
    return !bLoading?S_OK:S_FALSE;
}

HRESULT SwndStyle::OnAttrMarginX(const SStringW& strValue,BOOL bLoading)
{
	m_rcMargin[0] = m_rcMargin[2] = SLayoutSize::fromString(strValue);
    return !bLoading?S_OK:S_FALSE;
}

HRESULT SwndStyle::OnAttrMarginY(const SStringW& strValue,BOOL bLoading)
{
	m_rcMargin[1] = m_rcMargin[3] = SLayoutSize::fromString(strValue);

	return !bLoading?S_OK:S_FALSE;
}

void SwndStyle::_ParseLayoutSize4(const SStringW & strValue, SLayoutSize layoutSizes[])
{
	SStringWList values;
	int nValues = SplitString(strValue, L',', values);
	if (nValues == 1)
	{
		layoutSizes[0] = layoutSizes[1] = layoutSizes[2] = layoutSizes[3] = SLayoutSize::fromString(values[0]);
	}
	else if (nValues == 2)
	{
		layoutSizes[0] = layoutSizes[2] = SLayoutSize::fromString(values[0]);
		layoutSizes[1] = layoutSizes[3] = SLayoutSize::fromString(values[1]);
	}
	else if (nValues == 4)
	{
		layoutSizes[0] = SLayoutSize::fromString(values[0]);
		layoutSizes[1] = SLayoutSize::fromString(values[1]);
		layoutSizes[2] = SLayoutSize::fromString(values[2]);
		layoutSizes[3] = SLayoutSize::fromString(values[3]);
	}
}

HRESULT SwndStyle::OnAttrPadding(const SStringW &strValue,BOOL bLoading)
{
	_ParseLayoutSize4(strValue, m_rcInset);
	return bLoading?S_FALSE:S_OK;
}


void SwndStyle::SetScale(int nScale)
{
	m_nScale = nScale;
	for(int i=0;i<4;i++)
	{
		m_ftText[i].SetScale(nScale);
	}
}

CRect SwndStyle::GetMargin() const
{
	CRect rcRet;
	rcRet.left = m_rcMargin[0].toPixelSize(GetScale());
	rcRet.top = m_rcMargin[1].toPixelSize(GetScale());
	rcRet.right = m_rcMargin[2].toPixelSize(GetScale());
	rcRet.bottom = m_rcMargin[3].toPixelSize(GetScale());
	return rcRet;
}

CRect SwndStyle::GetPadding() const
{
	CRect rcRet;
	rcRet.left = m_rcInset[0].toPixelSize(GetScale());
	rcRet.top = m_rcInset[1].toPixelSize(GetScale());
	rcRet.right = m_rcInset[2].toPixelSize(GetScale());
	rcRet.bottom = m_rcInset[3].toPixelSize(GetScale());
	return rcRet;

}


}//namespace SOUI