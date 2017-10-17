#pragma once

#include <unknown/obj-ref-impl.hpp>
#include "interface/Sskinobj-i.h"


namespace SOUI
{

    class SOUI_EXP SSkinObjBase : public TObjRefImpl<SObjectImpl<ISkinObj>>
    {
		SOUI_CLASS_NAME_DECL_EX(SSkinObjBase, L"skinObjBase", Skin)
    public:
		SSkinObjBase();
		~SSkinObjBase() override;

        /**
        * GetAlpha
        * @brief    获得skin对象包含透明度
        * @return   BYTE -- 透明度
        * Describe  [0-255]
        */    
		BYTE GetAlpha() const override;

        /**
        * SetAlpha
        * @brief    设定skin对象包含透明度
        * @param    BYTE byAlpha-- 透明度
        * @return   void
        * Describe  
        */    
		void SetAlpha(BYTE byAlpha) override;
		void Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState, BYTE byAlpha) override;
        void Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState) override;
		SIZE GetSkinSize() override;
		BOOL IgnoreState() override;
		int GetStates() override;
		int GetScale() const override;
		ISkinObj * Scale(int nScale) override;
		LPCWSTR GetName() const  override;

        SOUI_ATTRS_DECL()

    protected:

		void _Scale(ISkinObj *pObj, int nScale) ; 

        /**
        * _Draw
        * @brief    Draw函数的实现
        * @param    IRenderTarget * pRT --  渲染目标
        * @param    LPCRECT rcDraw --  渲染位置
        * @param    DWORD dwState --  渲染状态
        * @param    BYTE byAlpha --  透明度
        * @return   void
        * Describe  每一个skin需要实现一个_Draw方法
        */    
        virtual void _Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha)=0;

        BYTE		m_byAlpha;
        COLORREF    m_crColorize;
        BOOL        m_bEnableColorize;
		int			m_nScale;

		SStringW	m_strName;

    };

}//namespace SOUI