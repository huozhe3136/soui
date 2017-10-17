#pragma once

#include <core/shostwnd.h>

namespace SOUI
{
    class SMenuEx;
    class SOUI_EXP SMenuExItem : public SWindow
    {
        SOUI_CLASS_NAME_DECL(SMenuExItem,L"menuItem")
    public:
        SMenuExItem(SMenuEx *pOwnerMenu,ISkinObj *pItemSkin);

        ~SMenuExItem() override;

        SMenuEx * GetSubMenu();

        SMenuEx * GetOwnerMenu();

        void HideSubMenu();

        void ShowSubMenu(BOOL bCheckFirstItem);

        WCHAR GetHotKey() const;

        void OnSubMenuHided(BOOL bUncheckItem);
    protected:
        BOOL CreateChildren(pugi::xml_node xmlNode) override;

        CSize GetDesiredSize(LPCRECT pRcContainer) override;

        void GetTextRect(LPRECT pRect) override;


    protected:

        BOOL OnEraseBkgnd(IRenderTarget *pRT);

        void OnPaint(IRenderTarget *pRT);

        SOUI_MSG_MAP_DECL() 

    protected:
        SOUI_ATTRS_DECL() 

        SMenuEx * m_pSubMenu;
        SMenuEx * m_pOwnerMenu;
        int       m_iIcon;
        BOOL      m_bCheck;
        BOOL      m_bRadio;    
        WCHAR     m_cHotKey;
    };

    class SOUI_EXP SMenuEx : protected SHostWnd
    {
        friend class SMenuExItem;
        friend class SMenuExRunData;
    public:
        SMenuEx(void);
        ~SMenuEx(void) override;

        BOOL LoadMenu(LPCTSTR pszMenu);
        BOOL LoadMenu(pugi::xml_node xmlNode);

        UINT TrackPopupMenu(UINT flag,int x,int y,HWND hOwner,int nScale = 100);
		static void ExitPopupMenu(int nCmdId=0);

        SMenuExItem * GetParentItem() {return m_pParent;}
        SMenuEx * GetSubMenu(int nID);
        SMenuExItem * GetMenuItem(int nID);

		DWORD GetContextHelpId() const;

		void SetContextHelpId(DWORD dwId);
		BOOL InsertMenu(UINT uPos,UINT uFlag,int nId,LPCTSTR lpNewItem);
    protected:
        int OnMouseActivate(HWND wndTopLevel, UINT nHitTest, UINT message);
        void OnTimer(UINT_PTR timeID);
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

        BEGIN_MSG_MAP_EX_DECL()
    protected:
        BOOL _HandleEvent(EventArgs *pEvt) override;
		const SStringW & GetTranslatorContext() override;
		int GetScale() const override;

        void ShowMenu(UINT uFlag,int x,int y);
        void HideMenu(BOOL bUncheckParentItem);
        void HideSubMenu();
        void RunMenu(HWND hOwner);

        void PopupSubMenu(SMenuExItem * pItem,BOOL bCheckFirstItem);
        void OnSubMenuHided(BOOL bUncheckItem);

        SMenuEx(SMenuExItem *pParent);
		void SendInitPopupMenu2Owner(int idx);
		SMenuExItem * m_pParent;
        SMenuExItem * m_pHoverItem;
        SMenuExItem * m_pCheckItem;

		BOOL	m_bMenuInitialized;
    };

}
