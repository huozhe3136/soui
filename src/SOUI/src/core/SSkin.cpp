//////////////////////////////////////////////////////////////////////////
//   File Name: SSkinPool
//////////////////////////////////////////////////////////////////////////

#include "souistd.h"
#include "core/Sskin.h"
#include "helper/SDIBHelper.h"

namespace SOUI
{

//////////////////////////////////////////////////////////////////////////
SOUI_CLASS_NAME(SSkinImgList, L"imglist")
// SSkinImgList
SSkinImgList::SSkinImgList()
:m_nStates(1)
,m_bTile(FALSE)
,m_bAutoFit(TRUE)
, m_bVertical(FALSE)
, m_filterLevel(kNone_FilterLevel)
{

}

SSkinImgList::~SSkinImgList()
{
}

SOUI_ATTRS_BEGIN(SSkinImgList)
	ATTR_IMAGEAUTOREF(L"src", m_pImg, FALSE)    //skinObj引用的图片文件定义在uires.idx中的name属性。
	ATTR_INT(L"tile", m_bTile, FALSE)    //绘制是否平铺,0--位伸（默认），其它--平铺
	ATTR_INT(L"autoFit", m_bAutoFit, FALSE)//autoFit为0时不自动适应绘图区大小
	ATTR_INT(L"vertical", m_bVertical, FALSE)//子图是否垂直排列，0--水平排列(默认), 其它--垂直排列
	ATTR_INT(L"states", m_nStates, FALSE)  //子图数量,默认为1
	ATTR_ENUM_BEGIN(L"filterLevel", FilterLevel, FALSE)
	ATTR_ENUM_VALUE(L"none", kNone_FilterLevel)
	ATTR_ENUM_VALUE(L"low", kLow_FilterLevel)
	ATTR_ENUM_VALUE(L"medium", kMedium_FilterLevel)
	ATTR_ENUM_VALUE(L"high", kHigh_FilterLevel)
	ATTR_ENUM_END(m_filterLevel)
SOUI_ATTRS_END()

void SSkinImgList::SetVertical(BOOL bVertical) 
{
	m_bVertical = bVertical; 
}

BOOL SSkinImgList::IsVertical()
{
	return m_bVertical;
}

bool SSkinImgList::SetImage(IBitmap *pImg)
{
	m_pImg = pImg;
	return true;
}

IBitmap *SSkinImgList::GetImage()
{
	return m_pImg;
}

void SSkinImgList::SetTile(BOOL bTile)
{
	m_bTile = bTile; 
}

BOOL SSkinImgList::IsTile() 
{
	return m_bTile; 
}

SIZE SSkinImgList::GetSkinSize()
{
    SIZE ret = {0, 0};
    if(m_pImg) ret=m_pImg->Size();
    if(m_bVertical) ret.cy/=m_nStates;
    else ret.cx/=m_nStates;
    return ret;
}

void SSkinImgList::SetStates(int nStates) 
{
	m_nStates = nStates; 
}

BOOL SSkinImgList::IgnoreState()
{
    return GetStates()==1;
}

int SSkinImgList::GetStates()
{
    return m_nStates;
}

void SSkinImgList::_Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha)
{
    if(!m_pImg) return;
    SIZE sz=GetSkinSize();
    RECT rcSrc={0,0,sz.cx,sz.cy};
    if(m_bVertical) 
        OffsetRect(&rcSrc,0, dwState * sz.cy);
    else
        OffsetRect(&rcSrc, dwState * sz.cx, 0);
    pRT->DrawBitmapEx(rcDraw,m_pImg,&rcSrc,GetExpandMode(),byAlpha);
}

UINT SSkinImgList::GetExpandMode()
{
    if(m_bAutoFit)
        return MAKELONG(m_bTile?EM_TILE:EM_STRETCH,m_filterLevel);
    else
        return MAKELONG(EM_NULL,m_filterLevel);
}

void SSkinImgList::OnColorize(COLORREF cr)
{
    if(!m_bEnableColorize) return;
    if(cr == m_crColorize) return;
	m_crColorize = cr;

    if(m_imgBackup)
    {//restore
        LPCVOID pSrc = m_imgBackup->GetPixelBits();
        LPVOID pDst = m_pImg->LockPixelBits();
        memcpy(pDst,pSrc,m_pImg->Width()*m_pImg->Height()*4);
        m_pImg->UnlockPixelBits(pDst);
    }else
    {
        if(S_OK != m_pImg->Clone(&m_imgBackup)) return;
    }
    
	if(cr!=0)
		SDIBHelper::Colorize(m_pImg,cr);
	else
		m_imgBackup = NULL;//free backup
}

void SSkinImgList::_Scale(ISkinObj * skinObj, int nScale)
{
	__super::_Scale(skinObj,nScale);
	SSkinImgList *pRet = sobj_cast<SSkinImgList>(skinObj);
	pRet->m_nStates = m_nStates;
	pRet->m_bTile = m_bTile;
	pRet->m_bVertical = m_bVertical;
	pRet->m_filterLevel = m_filterLevel;
	pRet->m_bAutoFit = m_bAutoFit;
	
	CSize szSkin = GetSkinSize();
	szSkin.cx = MulDiv(szSkin.cx,nScale,100);
	szSkin.cy = MulDiv(szSkin.cy,nScale,100);
	if(m_bVertical)
	{
		szSkin.cy *= GetStates();
	}else
	{
		szSkin.cx *= GetStates();
	}

	if(m_imgBackup)
	{
		m_imgBackup->Scale(&pRet->m_imgBackup,szSkin.cx,szSkin.cy,kHigh_FilterLevel);
	}
	if(m_pImg)
	{
		m_pImg->Scale(&pRet->m_pImg,szSkin.cx,szSkin.cy,kHigh_FilterLevel);
	}
}

//////////////////////////////////////////////////////////////////////////
SOUI_CLASS_NAME(SSkinImgFrame, L"imgframe")

//  SSkinImgFrame

SOUI_ATTRS_BEGIN(SSkinImgFrame)
	ATTR_RECT(L"margin", m_rcMargin, FALSE)          //九宫格4周
	ATTR_INT(L"left", m_rcMargin.left, FALSE)        //九宫格左边距
	ATTR_INT(L"top", m_rcMargin.top, FALSE)          //九宫格上边距
	ATTR_INT(L"right", m_rcMargin.right, FALSE)      //九宫格右边距
	ATTR_INT(L"bottom", m_rcMargin.bottom, FALSE)    //九宫格下边距
	ATTR_INT(L"margin-x", m_rcMargin.left = m_rcMargin.right, FALSE)//九宫格左右边距
	ATTR_INT(L"margin-y", m_rcMargin.top = m_rcMargin.bottom, FALSE)//九宫格上下边距
SOUI_ATTRS_END()

SSkinImgFrame::SSkinImgFrame()
	: m_rcMargin(0, 0, 0, 0)
{
}


void SSkinImgFrame::_Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha)
{
    if(!m_pImg) return;
    SIZE sz=GetSkinSize();
    CPoint pt;
    if(IsVertical())
        pt.y=sz.cy*dwState;
    else
        pt.x=sz.cx*dwState;
    CRect rcSour(pt,sz);
    pRT->DrawBitmap9Patch(rcDraw,m_pImg,&rcSour,&m_rcMargin,GetExpandMode(),byAlpha);
}

UINT SSkinImgFrame::GetExpandMode()
{
    return MAKELONG(m_bTile?EM_TILE:EM_STRETCH,m_filterLevel);
}

void SSkinImgFrame::_Scale(ISkinObj *skinObj, int nScale)
{
	SSkinImgList::_Scale(skinObj,nScale);
	SSkinImgFrame * pClone = sobj_cast<SSkinImgFrame>(skinObj);
	pClone->m_rcMargin.left = MulDiv(m_rcMargin.left , nScale, 100);
	pClone->m_rcMargin.top = MulDiv(m_rcMargin.top , nScale, 100);
	pClone->m_rcMargin.right = MulDiv(m_rcMargin.right , nScale, 100);
	pClone->m_rcMargin.bottom = MulDiv(m_rcMargin.bottom , nScale, 100);
}


//////////////////////////////////////////////////////////////////////////
SOUI_ATTRS_BEGIN(SSkinButton)
	ATTR_COLOR(L"colorBorder", m_colors.m_crBorder, TRUE)                //边框颜色
	ATTR_COLOR(L"colorUp", m_colors.m_crUp[ST_NORMAL], TRUE)             //正常状态渐变起始颜色
	ATTR_COLOR(L"colorDown", m_colors.m_crDown[ST_NORMAL], TRUE)         //正常状态渐变终止颜色
	ATTR_COLOR(L"colorUpHover", m_colors.m_crUp[ST_HOVER], TRUE)         //浮动状态渐变起始颜色
	ATTR_COLOR(L"colorDownHover", m_colors.m_crDown[ST_HOVER], TRUE)     //浮动状态渐变终止颜色
	ATTR_COLOR(L"colorUpPush", m_colors.m_crUp[ST_PUSHDOWN], TRUE)       //下压状态渐变起始颜色
	ATTR_COLOR(L"colorDownPush", m_colors.m_crDown[ST_PUSHDOWN], TRUE)   //下压状态渐变终止颜色
	ATTR_COLOR(L"colorUpDisable", m_colors.m_crUp[ST_DISABLE], TRUE)     //禁用状态渐变起始颜色
	ATTR_COLOR(L"colorDownDisable", m_colors.m_crDown[ST_DISABLE], TRUE) //禁用状态渐变终止颜色
	ATTR_INT(L"cornerRadius", m_nCornerRadius, TRUE)              //圆角大小
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinButton, L"button")
// SSkinButton
SSkinButton::SSkinButton()
    : m_nCornerRadius(2)
{
    m_colors.m_crBorder = RGB(0x70, 0x70, 0x70);
    m_colors.m_crUp[0]=(RGB(0xEE, 0xEE, 0xEE));
    m_colors.m_crDown[0]=(RGB(0xD6, 0xD6, 0xD6));
    m_colors.m_crUp[1]=(RGB(0xEE, 0xEE, 0xEE));
    m_colors.m_crDown[1]=(RGB(0xE0, 0xE0, 0xE0));
    m_colors.m_crUp[2]=(RGB(0xCE, 0xCE, 0xCE));
    m_colors.m_crDown[2]=(RGB(0xC0, 0xC0, 0xC0));
    m_colors.m_crUp[3]=(RGB(0x8E, 0x8E, 0x8E));
    m_colors.m_crDown[3]=(RGB(0x80, 0x80, 0x80));
}

void SSkinButton::_Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha)
{
    CRect rcDraw = *prcDraw;
    
    rcDraw.DeflateRect(1, 1);
    CAutoRefPtr<IRegion> rgnClip;
    if(m_nCornerRadius>2)
    {
        GETRENDERFACTORY->CreateRegion(&rgnClip);
        //the last two params of CreateRoundRectRgn are width and height of ellipse, thus we should multiple corner radius by 2.
        HRGN hRgn = ::CreateRoundRectRgn(prcDraw->left,prcDraw->top,prcDraw->right+1,prcDraw->bottom+1,m_nCornerRadius*2,m_nCornerRadius*2);
        rgnClip->SetRgn(hRgn);
        DeleteObject(hRgn);
    }
    if(rgnClip)
        pRT->PushClipRegion(rgnClip);
    if(m_colors.m_crUp[dwState]!=m_colors.m_crDown[dwState])
        pRT->GradientFill(rcDraw,TRUE,m_colors.m_crUp[dwState],m_colors.m_crDown[dwState],byAlpha);
    else
    {
        SColor cr(m_colors.m_crDown[dwState]);
        cr.updateAlpha(byAlpha);
        pRT->FillSolidRect(prcDraw,cr.toCOLORREF());
    }

    CAutoRefPtr<IPen> pPen,pOldPen;
    pRT->CreatePen(PS_SOLID,m_colors.m_crBorder,1,&pPen);
    pRT->SelectObject(pPen,(IRenderObj**)&pOldPen);
    pRT->DrawRoundRect(prcDraw,CPoint(m_nCornerRadius,m_nCornerRadius));
    pRT->SelectObject(pOldPen);
    
    if(rgnClip)
        pRT->PopClip();
}

BOOL SSkinButton::IgnoreState()
{
    return FALSE;
}

int SSkinButton::GetStates()
{
    return 4;
}

void SSkinButton::SetColors( COLORREF crUp[4],COLORREF crDown[4],COLORREF crBorder )
{
    memcpy(m_colors.m_crUp,crUp,4*sizeof(COLORREF));
    memcpy(m_colors.m_crDown,crDown,4*sizeof(COLORREF));
    m_colors.m_crBorder=crBorder;
}

void SSkinButton::OnColorize(COLORREF cr)
{
    if(!m_bEnableColorize) return;
    if(m_crColorize == cr) return;
    if(cr == 0)
    {
        memcpy(&m_colors,&m_colorsBackup,sizeof(BTNCOLORS));
        m_crColorize = 0;
    }else
    {
        if(m_crColorize!=0)
        {//从备份里获取数据
            memcpy(&m_colors,&m_colorsBackup,sizeof(BTNCOLORS));
        }else
        {//将数据备份
            memcpy(&m_colorsBackup,&m_colors,sizeof(BTNCOLORS));
        }
        m_crColorize = cr;

        //调整颜色值
        SDIBHelper::Colorize(m_colors.m_crBorder,m_crColorize);
        for(int i=0;i<4;i++)
        {
            SDIBHelper::Colorize(m_colors.m_crDown[i],m_crColorize);
            SDIBHelper::Colorize(m_colors.m_crUp[i],m_crColorize);
        }
    }
}

ISkinObj * SSkinButton::Scale(int nScale)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// SSkinGradation


SOUI_ATTRS_BEGIN(SSkinGradation)
	ATTR_COLOR(L"colorFrom", m_crFrom, TRUE)    //渐变起始颜色
	ATTR_COLOR(L"colorTo", m_crTo, TRUE)        //渐变终止颜色
	ATTR_INT(L"vertical", m_bVert, TRUE)        //渐变方向,0--水平(默认), 1--垂直
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinGradation, L"gradation")

SSkinGradation::SSkinGradation()
    : m_crFrom(CR_INVALID)
	, m_crTo(CR_INVALID)
	, m_bVert(TRUE)
	, m_crColorize(0)
{
}

void SSkinGradation::_Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha)
{
    pRT->GradientFill(prcDraw,m_bVert,m_crFrom,m_crTo,byAlpha);
}

void SSkinGradation::OnColorize(COLORREF cr)
{
    if(!m_bEnableColorize) return;
    if(m_crColorize == cr) return;
    if(m_crColorize!=0)
    {
        m_crFrom = m_crFromBackup;
        m_crTo = m_crToBackup;
    }else
    {
        m_crFromBackup = m_crFrom;
        m_crToBackup = m_crTo;
    }
    m_crColorize = cr;
    SDIBHelper::Colorize(m_crFrom,cr);
    SDIBHelper::Colorize(m_crTo,cr);
}

ISkinObj * SSkinGradation::Scale(int nScale)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// SScrollbarSkin
SOUI_ATTRS_BEGIN(SSkinScrollbar)
	ATTR_INT(L"margin", m_nMargin, FALSE)             //边缘不拉伸大小
	ATTR_INT(L"hasGripper", m_bHasGripper, FALSE)     //滑块上是否有帮手(gripper)
	ATTR_INT(L"hasInactive", m_bHasInactive, FALSE)   //是否有禁用态
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinScrollbar, L"scrollbar")

SSkinScrollbar::SSkinScrollbar():m_nMargin(0),m_bHasGripper(FALSE),m_bHasInactive(FALSE)
{
    
}

BOOL SSkinScrollbar::HasArrow() 
{
	return TRUE; 
}

int SSkinScrollbar::GetIdealSize() 
{
	if (!m_pImg) return 0;
	return m_pImg->Width() / 9;
}

CRect SSkinScrollbar::GetPartRect(int nSbCode, int nState,BOOL bVertical)
{
    CSize sz=GetSkinSize();
    CSize szFrame(sz.cx/9,sz.cx/9);
    if(nSbCode==SB_CORNOR)
    {
        return CRect(CPoint(szFrame.cx*8,0),szFrame);
    }else if(nSbCode==SB_THUMBGRIPPER)
    {
        return CRect(CPoint(szFrame.cx*8,(1+(bVertical?0:1))*szFrame.cy),szFrame);
    }else
    {
        if(nState==SBST_INACTIVE && !m_bHasInactive)
        {
            nState=SBST_NORMAL;
        }
        CRect rcRet;
        int iPart=-1;
        switch(nSbCode)
        {
        case SB_LINEUP:
            iPart=0;
            break;
        case SB_LINEDOWN:
            iPart=1;
            break;
        case SB_THUMBTRACK:
            iPart=2;
            break;
        case SB_PAGEUP:
        case SB_PAGEDOWN:
            iPart=3;
            break;
        }
        if(!bVertical) iPart+=4;
        
        return CRect(CPoint(szFrame.cx*iPart,szFrame.cy*nState),szFrame);
    }
}

void SSkinScrollbar::_Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha)
{
    if(!m_pImg) return;
    int nSbCode=LOWORD(dwState);
    int nState=LOBYTE(HIWORD(dwState));
    BOOL bVertical=HIBYTE(HIWORD(dwState));
    CRect rcMargin(0,0,0,0);
    if(bVertical)
        rcMargin.top=m_nMargin,rcMargin.bottom=m_nMargin;
    else
        rcMargin.left=m_nMargin,rcMargin.right=m_nMargin;

    CRect rcSour=GetPartRect(nSbCode,nState,bVertical);
    
    pRT->DrawBitmap9Patch(prcDraw,m_pImg,&rcSour,&rcMargin,m_bTile?EM_TILE:EM_STRETCH,byAlpha);
    
    if(nSbCode==SB_THUMBTRACK && m_bHasGripper)
    {
        rcSour=GetPartRect(SB_THUMBGRIPPER,0,bVertical);
        CRect rcDraw=*prcDraw;
        
        if (bVertical)
            rcDraw.top+=(rcDraw.Height()-rcSour.Height())/2,rcDraw.bottom=rcDraw.top+rcSour.Height();
        else
            rcDraw.left+=(rcDraw.Width()-rcSour.Width())/2,rcDraw.right=rcDraw.left+rcSour.Width();
        pRT->DrawBitmap9Patch(&rcDraw,m_pImg,&rcSour,&rcMargin,m_bTile?EM_TILE:EM_STRETCH,byAlpha);
    }
}

void SSkinScrollbar::_Scale(ISkinObj *skinObj, int nScale)
{
	__super::_Scale(skinObj,nScale);

	SSkinScrollbar *pRet = sobj_cast<SSkinScrollbar>(skinObj);
	pRet->m_nMargin = MulDiv(m_nMargin,nScale,100);
	pRet->m_bHasInactive = m_bHasInactive;
	pRet->m_bHasGripper = m_bHasGripper;
}


//////////////////////////////////////////////////////////////////////////
// SSkinColor
SOUI_ATTRS_BEGIN(SSkinColorRect)
	ATTR_COLOR(L"normal", m_crStates[0], FALSE)
	ATTR_COLOR(L"hover", m_crStates[1], FALSE)
	ATTR_COLOR(L"pushdown", m_crStates[2], FALSE)
	ATTR_COLOR(L"disable", m_crStates[3], FALSE)
	ATTR_INT(L"cornerRadius", m_nRadius, FALSE)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinColorRect, L"colorrect")

SSkinColorRect::SSkinColorRect():m_nRadius(0)
{
    m_crStates[0]=RGBA(255,255,255,255);
    m_crStates[1]=CR_INVALID;
    m_crStates[2]=CR_INVALID;
    m_crStates[3]=CR_INVALID;
}

SSkinColorRect::~SSkinColorRect()
{
}

BOOL SSkinColorRect::IgnoreState()
{
	return GetStates()<2; 
}

void SSkinColorRect::_Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha)
{
    if(dwState>3) return;
    
    if(m_crStates[dwState]==CR_INVALID)
        dwState =0;
    SColor cr(m_crStates[dwState]);
    cr.updateAlpha(byAlpha);
    if(m_nRadius!=0)
        pRT->FillSolidRoundRect(prcDraw,CPoint(m_nRadius,m_nRadius),cr.toCOLORREF());
    else
        pRT->FillSolidRect(prcDraw,cr.toCOLORREF());
}

int SSkinColorRect::GetStates()
{
	int nStates = 4;
	for(int i=3;i>=0;i--)
	{
		if(m_crStates[i] == CR_INVALID) nStates--;
		else break;
	}
	return nStates;
}

ISkinObj * SSkinColorRect::Scale(int nScale)
{
	return NULL;
}


//////////////////////////////////////////////////////////////////////////


SOUI_ATTRS_BEGIN(SSkinShape)
	ATTR_ENUM_BEGIN(L"shape", Shape, TRUE)
	ATTR_ENUM_VALUE(L"rectangle", rectangle)
	ATTR_ENUM_VALUE(L"oval", oval)
	ATTR_ENUM_VALUE(L"ring", ring)
	ATTR_ENUM_END(m_shape)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinShape, L"shape")

SSkinShape::SSkinShape() :m_shape(rectangle), m_crSolid(CR_INVALID)
{

}

SSkinShape::~SSkinShape()
{

}

BOOL SSkinShape::IgnoreState()
{
	return TRUE; 
}

int SSkinShape::GetStates() 
{
	return 1; 
}

void SSkinShape::OnInitFinished(pugi::xml_node xmlNode)
{
	pugi::xml_node xmlSolid = xmlNode.child(L"solid");
	if(xmlSolid)
	{
		m_crSolid = GETCOLOR(xmlSolid.attribute(L"color").as_string());
	}
	pugi::xml_node xmlGrident = xmlNode.child(SGradient::GetClassName());
	if(xmlGrident)
	{
		if (!m_gradient) m_gradient.Attach(new SGradient());
		m_gradient->InitFromXml(xmlGrident);
	}
	pugi::xml_node xmlSize = xmlNode.child(L"size");
	if(xmlSize)
	{
		if (!m_shapeSize) m_shapeSize.Attach(new SShapeSize());
		m_shapeSize->InitFromXml(xmlSize);
	}
	pugi::xml_node xmlStoke = xmlNode.child(SStroke::GetClassName());
	if(xmlStoke)
	{
		if (!m_stroke) m_stroke.Attach(new SStroke());
		m_stroke->InitFromXml(xmlStoke);
	}
	pugi::xml_node xmlConner = xmlNode.child(SCornerSize::GetClassName());
	if(xmlConner)
	{
		if (!m_cornerSize) m_cornerSize.Attach(new SCornerSize());
		m_cornerSize->InitFromXml(xmlConner);
	}
	pugi::xml_node xmlRing = xmlNode.child(SShapeRing::GetClassName());
	if (xmlRing)
	{
		if (!m_ringParam) m_ringParam.Attach(new SShapeRing());
		m_ringParam->InitFromXml(xmlRing);
	}
}

void SSkinShape::_Scale(ISkinObj * pObj, int nScale)
{
	__super::_Scale(pObj, nScale);

	SSkinShape * pRet = sobj_cast<SSkinShape>(pObj);
	SASSERT(pRet);
	pRet->m_shape = m_shape;
	pRet->m_crSolid = m_crSolid;
	pRet->m_shapeSize = m_shapeSize;
	pRet->m_cornerSize = m_cornerSize;
	pRet->m_stroke = m_stroke;
	pRet->m_gradient = m_gradient;
	pRet->m_ringParam = m_ringParam;
}


SIZE SSkinShape::GetSkinSize()
{
	if (!m_shapeSize) return CSize();

	SIZE szRet = {m_shapeSize->m_width.toPixelSize(GetScale()),m_shapeSize->m_height.toPixelSize(GetScale())};
	return szRet;
}

void SSkinShape::_Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha)
{
	POINT ptConner = {0,0};
	if(m_cornerSize)
	{
		ptConner = m_cornerSize->GetConner(GetScale());
	}

	RECT rcDest = *rcDraw;
	::InflateRect(&rcDest,-1,-1);
	if(m_crSolid != CR_INVALID)
	{
		CAutoRefPtr<IBrush> brush,oldBrush;
		pRT->CreateSolidColorBrush(m_crSolid,&brush);
		pRT->SelectObject(brush,(IRenderObj**)&oldBrush);
		
		switch(m_shape)
		{
		case rectangle:
			if(m_cornerSize)
				pRT->FillSolidRoundRect(&rcDest,ptConner,m_crSolid);
			else
				pRT->FillSolidRect(&rcDest,m_crSolid);
			break;
		case oval:
			pRT->FillEllipse(&rcDest);
			break;
		case ring:
			break;
		}
		pRT->SelectObject(oldBrush);
	}

	if(m_gradient!=NULL)
	{
		CAutoRefPtr<IRegion> region;
		pRT->CreateRegion(&region);
		RECT rcGradient = rcDest;
		//set clip
		switch(m_shape)
		{
		case rectangle:
			if(m_cornerSize)
			{
				region->CombineRoundRect(&rcGradient,ptConner,RGN_COPY);
			}
			break;
		case oval:
			region->CombineEllipse(&rcGradient,RGN_COPY);
			break;
		case ring:
			break;
		}
		if(!region->IsEmpty())
			pRT->PushClipRegion(region);

		
		m_gradient->Draw(pRT,&rcGradient,byAlpha,GetScale());

		if(!region->IsEmpty())
			pRT->PopClip();
	}

	if(m_stroke != NULL)
	{
		CAutoRefPtr<IPen> pPen,oldPen;
		int nPenWidth = m_stroke->m_width.toPixelSize(GetScale());
		pRT->CreatePen(m_stroke->m_style,m_stroke->m_color,nPenWidth,&pPen);
		pRT->SelectObject(pPen,(IRenderObj**)&oldPen);
		switch(m_shape)
		{
		case rectangle:
			if(m_cornerSize)
				pRT->DrawRoundRect(&rcDest,ptConner);
			else
				pRT->DrawRectangle(&rcDest);
			break;
		case oval:
			pRT->DrawEllipse(&rcDest);
			break;
		case ring:
			if(m_ringParam){
				int nRadius = m_ringParam->m_innerRadio.toPixelSize(GetScale())+nPenWidth/2;
				POINT ptCenter = { (rcDest.left + rcDest.right) / 2,(rcDest.top + rcDest.bottom) / 2 };
				RECT rcRing = { ptCenter.x - nRadius,ptCenter.y - nRadius,ptCenter.x + nRadius,ptCenter.y + nRadius };
				pRT->DrawArc(&rcRing, m_ringParam->m_startAngle, m_ringParam->m_sweepAngle, false);
			}
			break;
		}
		pRT->SelectObject(oldPen);
	}

}

SOUI_ATTRS_BEGIN(SSkinShape::SGradient)
	ATTR_ENUM_BEGIN(L"type", GradientType, TRUE)
	ATTR_ENUM_VALUE(L"linear", linear)
	ATTR_ENUM_VALUE(L"radial", radial)
	ATTR_ENUM_VALUE(L"sweep", sweep)
	ATTR_ENUM_END(m_Type)
	ATTR_FLOAT(L"angle", m_angle, TRUE)
	ATTR_FLOAT(L"centerX", m_centerX, TRUE)
	ATTR_FLOAT(L"centerY", m_centerY, TRUE)
	ATTR_COLOR(L"startColor", m_crStart, TRUE)
	ATTR_COLOR(L"centerColor", m_crCenter, TRUE)
	ATTR_COLOR(L"endColor", m_crEnd, TRUE)
	ATTR_LAYOUTSIZE(L"radius", m_radius, TRUE)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinShape::SGradient, L"gradient")


void SSkinShape::SGradient::Draw(IRenderTarget *pRT, LPCRECT rcDraw, BYTE byAlpha,int nScale)
{
	pRT->GradientFill2(rcDraw,m_Type,m_crStart,m_crCenter,m_crEnd,m_angle,m_centerX,m_centerY,m_radius.toPixelSize(nScale),byAlpha);
}



SSkinShape::SGradient::SGradient() :m_Type(linear), m_angle(0.0f), m_centerX(0.5f), m_centerY(0.5f) 
{

}

SSkinShape::SCornerSize::SCornerSize()
{

}

SOUI_ATTRS_BEGIN(SSkinShape::SCornerSize)
	ATTR_CUSTOM(L"radius", OnAttrRadius)
	ATTR_LAYOUTSIZE(L"radiusX", m_radiusX, TRUE)
	ATTR_LAYOUTSIZE(L"radiusY", m_radiusY, TRUE)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinShape::SCornerSize, L"corners")

HRESULT SSkinShape::SCornerSize::OnAttrRadius(const SStringW strValue,BOOL bLoading)
{
	SStringWList values;
	int nValues = SplitString(strValue,L',',values);
	if(nValues == 1)
	{
		m_radiusX=
		m_radiusY=SLayoutSize::fromString(values[0]);
		return S_OK;
	}else if(nValues == 2)
	{
		m_radiusX=SLayoutSize::fromString(values[0]);
		m_radiusY=SLayoutSize::fromString(values[1]);
		return S_OK;
	}
	return E_INVALIDARG;
}


//////////////////////////////////////////////////////////////////////////

SOUI_ATTRS_BEGIN(SSKinGroup)
	ATTR_SKIN(L"normal", m_skins[0], FALSE)
	ATTR_SKIN(L"hover", m_skins[1], FALSE)
	ATTR_SKIN(L"pushDown", m_skins[2], FALSE)
	ATTR_SKIN(L"disable", m_skins[3], FALSE)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSKinGroup, L"group")

// SSKinGroup
int SSKinGroup::GetStates()
{
	return 4;
}



BOOL SSKinGroup::IgnoreState() 
{
	return FALSE; 
}

void SSKinGroup::_Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha)
{
	if((int)dwState>= GetStates())
		return;
	ISkinObj *pSkin = m_skins[dwState];
	if(!pSkin) return;
	pSkin->Draw(pRT,rcDraw,0,byAlpha);
}

SIZE SSKinGroup::GetSkinSize()
{
	for(int i=0;i<GetStates();i++)
	{
		if(m_skins[i]) return m_skins[i]->GetSkinSize();
	}
	return CSize();
}

/*
IBitmap中的内存为RGBA格式，.9中使用alpha通道==0或者255来确定如何拉伸
*/


SOUI_ATTRS_BEGIN(SSkinImgFrame2)
	ATTR_CUSTOM(L"src", OnAttrSrc)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinImgFrame2, L"imgframe2")

HRESULT SSkinImgFrame2::OnAttrSrc(const SStringW & strValue,BOOL bLoading)
{
	IBitmap *pImg=LOADIMAGE2(strValue); 
	if(!pImg) return E_FAIL;
	int nWid=pImg->Width();
	int nHei = pImg->Height();
	if(nWid<2 || nHei<2) return E_INVALIDARG;
	LPBYTE pBuf = (LPBYTE)pImg->LockPixelBits();

	int left,right,top,bottom;
	//检测第一扫描行中定义的left,right位置
	LPBYTE p = pBuf;
	int i=1;
	while(i<nWid-1 && p[3]==0) i++,p+=4;
	left = i-1;

	while(i<nWid-1 && p[3]!=0) i++,p+=4;
	right = i-1;

	//检测第一列中定义的top,bottom位置
	i=1;
	p = pBuf+(nWid*4);
	while(i<nHei-1 && p[3]==0) i++,p+=nWid*4;
	top = i-1;
	while(i<nHei-1 && p[3]!=0) i++,p+=nWid*4;
	bottom = i-1;

	
	m_rcMargin.left = left;
	m_rcMargin.right = nWid - 2 - right;
	m_rcMargin.top = top;
	m_rcMargin.bottom = nHei - 2 - bottom;

	HRESULT hRet = S_OK;
	IBitmap *pImgCenter=NULL;
	if(!pImg->GetRenderFactory()->CreateBitmap(&pImgCenter))
	{
		return E_OUTOFMEMORY;
	}
	hRet = pImgCenter->Init(nWid-2,nHei-2);
	if(hRet!=S_OK)
	{
		return hRet;
	}
	LPBYTE pBuf2= (LPBYTE)pImgCenter->LockPixelBits();


	LPBYTE pSrc=pBuf+(nWid*4);
	LPBYTE pDst = pBuf2;

	i=1;
	pSrc+=4;
	while(i<nHei-1)
	{
		memcpy(pDst,pSrc,(nWid-2)*4);
		pDst += (nWid-2)*4;
		pSrc += nWid*4;
		i++;
	}

	pImg->UnlockPixelBits(pBuf);
	pImgCenter->UnlockPixelBits(pBuf2);

	m_pImg = pImgCenter;
	pImgCenter->Release();
	pImg->Release();

	return hRet;
}

SOUI_ATTRS_BEGIN(SSkinShape::SShapeRing)
	ATTR_FLOAT(L"startAngle", m_startAngle, TRUE)
	ATTR_FLOAT(L"sweepAngle", m_sweepAngle, TRUE)
	ATTR_LAYOUTSIZE(L"innerRadio", m_innerRadio, TRUE)
SOUI_ATTRS_END() 

SOUI_CLASS_NAME(SSkinShape::SShapeRing, L"ring")

SSkinShape::SShapeRing::SShapeRing() :m_startAngle(0.0f), m_sweepAngle(360.0f)
{
}

SOUI_ATTRS_BEGIN(SSkinShape::SStroke)
	ATTR_LAYOUTSIZE(L"width", m_width, TRUE)
	ATTR_COLOR(L"color", m_color, TRUE)
	ATTR_ENUM_BEGIN(L"style", int, TRUE)
	ATTR_ENUM_VALUE(L"solid", PS_SOLID)
	ATTR_ENUM_VALUE(L"dash", PS_DASH)
	ATTR_ENUM_VALUE(L"dashDot", PS_DASHDOT)
	ATTR_ENUM_VALUE(L"dashDotDot", PS_DASHDOTDOT)
	ATTR_ENUM_END(m_style)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinShape::SStroke, L"stroke")

SSkinShape::SStroke::SStroke() :m_color(CR_INVALID), m_style(PS_SOLID) 
{
}



SOUI_ATTRS_BEGIN(SSkinShape::SShapeSize)
	ATTR_LAYOUTSIZE(L"width", m_width, TRUE)
	ATTR_LAYOUTSIZE(L"height", m_height, TRUE)
SOUI_ATTRS_END()

SOUI_CLASS_NAME(SSkinShape::SShapeSize, L"size")

SSkinShape::SShapeSize::SShapeSize()
{

}

}//namespace SOUI