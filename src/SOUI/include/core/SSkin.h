#pragma once

#include "SSkinObjBase.h"

namespace SOUI
{

// State Define
enum
{
    WndState_Normal       = 0x00000000UL,
    WndState_Hover        = 0x00000001UL,
    WndState_PushDown     = 0x00000002UL,
    WndState_Check        = 0x00000004UL,
    WndState_Invisible    = 0x00000008UL,
    WndState_Disable      = 0x00000010UL,
};

#define IIF_STATE2(the_state, normal_value, hover_value) \
    (((the_state) & WndState_Hover) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
    (((the_state) & (WndState_PushDown|WndState_Check)) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
    (((the_state) & WndState_Disable) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))


//////////////////////////////////////////////////////////////////////////


class SOUI_EXP SSkinImgList: public SSkinObjBase
{
    SOUI_CLASS_NAME_DECL(SSkinImgList, L"imglist")

public:
    SSkinImgList();
    ~SSkinImgList() override;


     SIZE GetSkinSize() override;

     BOOL IgnoreState() override;
    
     int GetStates() override;
	 virtual void SetStates(int nStates) ;



	virtual bool SetImage(IBitmap *pImg) ;

	virtual IBitmap * GetImage() ;

	virtual void SetTile(BOOL bTile) ;
	virtual BOOL IsTile() ;

	virtual void SetVertical(BOOL bVertical) ;
	virtual BOOL IsVertical()  ;
    
    void OnColorize(COLORREF cr) override;

protected:
	virtual void _Scale(ISkinObj *skinObj, int nScale);
    void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha) override;;

    virtual UINT GetExpandMode();
    
    CAutoRefPtr<IBitmap> m_pImg;
    int  m_nStates;
    BOOL m_bTile;
    BOOL m_bAutoFit;
    BOOL m_bVertical;
    CAutoRefPtr<IBitmap> m_imgBackup;   //色调调整前的备分

    FilterLevel m_filterLevel;
    
    SOUI_ATTRS_DECL()
         
};

//////////////////////////////////////////////////////////////////////////
class SOUI_EXP SSkinImgFrame : public SSkinImgList
{
    SOUI_CLASS_NAME_DECL(SSkinImgFrame, L"imgframe")

public:
    SSkinImgFrame();


    void SetMargin(const CRect rcMargin){m_rcMargin=rcMargin;}

    CRect GetMargin(){return m_rcMargin;}

protected:
    void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha) override ; 
    UINT GetExpandMode() override;
	void _Scale(ISkinObj *skinObj, int nScale) override;

    CRect m_rcMargin;

    SOUI_ATTRS_DECL()
};

//////////////////////////////////////////////////////////////////////////
// 加载android .9的皮肤
class SOUI_EXP SSkinImgFrame2 : public SSkinImgFrame
{
	SOUI_CLASS_NAME_DECL(SSkinImgFrame2, L"imgframe2")
public:
	SOUI_ATTRS_DECL() 

protected:
	HRESULT OnAttrSrc(const SStringW & strValue,BOOL bLoading);
};

//////////////////////////////////////////////////////////////////////////
class SOUI_EXP SSkinButton : public SSkinObjBase
{
    SOUI_CLASS_NAME_DECL(SSkinButton, L"button")

    enum{
        ST_NORMAL=0,
        ST_HOVER,
        ST_PUSHDOWN,
        ST_DISABLE,
    };

    struct BTNCOLORS{
        COLORREF m_crBorder;

        COLORREF    m_crUp[4];
        COLORREF    m_crDown[4];
    };
public:
    SSkinButton();


    BOOL IgnoreState() override;

    int GetStates() override;

    void SetColors(COLORREF crUp[4],COLORREF crDown[4],COLORREF crBorder);

	ISkinObj * Scale(int nScale) override;

protected:
    void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha) override;
    void OnColorize(COLORREF cr) override;
    BTNCOLORS   m_colors;
    BTNCOLORS   m_colorsBackup;

    int         m_nCornerRadius;
    
    SOUI_ATTRS_DECL()


};

//////////////////////////////////////////////////////////////////////////

class SOUI_EXP SSkinGradation  : public SSkinObjBase
{
    SOUI_CLASS_NAME_DECL(SSkinGradation, L"gradation")
public:
    SSkinGradation();
    
    void SetColorFrom(COLORREF crFrom)
    {
        m_crFrom=crFrom;
    }

    void SetColorTo(COLORREF crTo)
    {
        m_crTo=crTo;
    }

    void SetVertical(BOOL bVertical)
    {
        m_bVert=bVertical;
    }

protected:
    void _Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha) override;
    void OnColorize(COLORREF cr) override;
	ISkinObj * Scale(int nScale) override;

    COLORREF m_crFrom;
    COLORREF m_crTo;
    BOOL m_bVert;

    COLORREF    m_crColorize;
    COLORREF    m_crFromBackup,m_crToBackup;

    SOUI_ATTRS_DECL() 


};


//////////////////////////////////////////////////////////////////////////
//
enum SBSTATE{
    SBST_NORMAL=0,    //正常状态
    SBST_HOVER,        //hover状态
    SBST_PUSHDOWN,    //按下状态
    SBST_DISABLE,    //禁用状态
    SBST_INACTIVE,    //失活状态,主要针对两端的箭头
};

#define MAKESBSTATE(sbCode,nState1,bVertical) MAKELONG((sbCode),MAKEWORD((nState1),(bVertical)))
#define SB_CORNOR        10
#define SB_THUMBGRIPPER    11    //滚动条上的可拖动部分

#define THUMB_MINSIZE    18

class SOUI_EXP SSkinScrollbar : public SSkinImgList
{
    SOUI_CLASS_NAME_DECL(SSkinScrollbar, L"scrollbar")

public:

    SSkinScrollbar();

    //指示滚动条皮肤是否支持显示上下箭头
	virtual BOOL HasArrow();
    
	virtual int GetIdealSize();

protected:
     void _Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha) override;
    //返回源指定部分在原位图上的位置。
    virtual CRect GetPartRect(int nSbCode, int nState,BOOL bVertical);
	 void _Scale(ISkinObj *skinObj, int nScale) override;

    int         m_nMargin;
    BOOL        m_bHasGripper;
    BOOL        m_bHasInactive;//有失活状态的箭头时，滚动条皮肤有必须有5行，否则可以是3行或者4行

    SOUI_ATTRS_DECL() 
};


//////////////////////////////////////////////////////////////////////////
class SOUI_EXP SSkinColorRect : public SSkinObjBase
{
    SOUI_CLASS_NAME_DECL(SSkinColorRect,L"colorrect")
public:
    SSkinColorRect();
    ~SSkinColorRect() override;

protected:
    void _Draw(IRenderTarget *pRT, LPCRECT prcDraw, DWORD dwState,BYTE byAlpha) override;

	BOOL IgnoreState() override;

	 int GetStates() override;
	 ISkinObj * Scale(int nScale) override;

    SOUI_ATTRS_DECL() 


    int      m_nRadius;
    COLORREF m_crStates[4];
};

//////////////////////////////////////////////////////////////////////////

class SOUI_EXP SSkinShape : public SSkinObjBase
{
	SOUI_CLASS_NAME_DECL(SSkinShape,L"shape")
	enum Shape {rectangle,oval,ring};

	class SGradient : public SObject , public TObjRefImpl<IObjRef>
	{
		SOUI_CLASS_NAME_DECL(SGradient,L"gradient")
	public:
		SGradient();

		void Draw(IRenderTarget *pRT, LPCRECT rcDraw, BYTE byAlpha,int nScale);

		SOUI_ATTRS_DECL() 
	protected:
		GradientType m_Type;
		float m_angle; //渐变角度，必须为45的倍数，0为从左到右，90为从上到下 
		float m_centerX; //渐变中心X的相当位置，范围为0～1  
		float m_centerY;//渐变中心Y的相当位置，范围为0～1  
		COLORREF m_crStart;//渐变开始点的颜色  
		COLORREF m_crCenter; //渐变中间点的颜色，在开始与结束点之间
		COLORREF m_crEnd;//渐变结束点的颜色  
		SLayoutSize m_radius;//渐变的半径，只有当渐变类型为radial时才能使用  
	};

	class SStroke : public SObject, public TObjRefImpl<IObjRef>
	{
		SOUI_CLASS_NAME_DECL(SStroke,L"stroke")
	public:

		SStroke();

		SOUI_ATTRS_DECL() 
	public:
		SLayoutSize m_width;  //描边的宽度  
		COLORREF	m_color;  //描边的颜色
		int			m_style; //线型
	};

	class SCornerSize : public SObject, public TObjRefImpl<IObjRef>
	{
		SOUI_CLASS_NAME_DECL(SCornerSize,L"corners")

	public:
		SCornerSize();

		HRESULT OnAttrRadius(const SStringW strValue,BOOL bLoading);

		SOUI_ATTRS_DECL() 


		POINT GetConner(int nScale)
		{
			POINT pt={m_radiusX.toPixelSize(nScale),m_radiusY.toPixelSize(nScale)};
			return pt;
		}

		SLayoutSize m_radiusX,m_radiusY;
	};

	class SShapeSize : public SObject, public TObjRefImpl<IObjRef>
	{
		SOUI_CLASS_NAME_DECL(SShapeSize,L"size")

	public:
		SShapeSize();

		SOUI_ATTRS_DECL() 

		SLayoutSize m_width,m_height;
	};

	class SShapeRing : public SObject, public TObjRefImpl<IObjRef>
	{
		SOUI_CLASS_NAME_DECL(SShapeRing, L"ring")

	public:
		SShapeRing();

		SOUI_ATTRS_DECL()
			


		float m_startAngle;
		float m_sweepAngle;
		SLayoutSize m_innerRadio;
	};

public:
	SSkinShape();
	~SSkinShape() override;

	SIZE GetSkinSize() override;

	 BOOL IgnoreState() override;

	 int GetStates() override;

	SOUI_ATTRS_DECL() 
protected:
	void OnInitFinished(pugi::xml_node xmlNode) override;

	virtual void _Scale(ISkinObj *pObj, int nScale);

	void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha) override;

	Shape  m_shape;

	COLORREF	m_crSolid;
	CAutoRefPtr<SShapeSize>		m_shapeSize;
	CAutoRefPtr<SCornerSize>	m_cornerSize;
	CAutoRefPtr<SStroke>	  	m_stroke;
	CAutoRefPtr<SGradient>      m_gradient;
	CAutoRefPtr<SShapeRing>		m_ringParam;
};


class SOUI_EXP SSKinGroup : public SSkinObjBase
{
SOUI_CLASS_NAME_DECL(SSKinGroup,L"group")
public:

	int GetStates() override;
	BOOL IgnoreState() override;
	SIZE GetSkinSize() override;

	SOUI_ATTRS_DECL() 

protected:
	 void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha) override;

	CAutoRefPtr<ISkinObj> m_skins[4];
};
}//namespace SOUI