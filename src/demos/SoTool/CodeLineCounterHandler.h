#pragma once

#include "CodeLineCounter.h"
#include "FolderHander.h"

class CCodeLineCounterHandler : public CFolderHander
{
    friend class CMainDlg;
    struct FILEINFO
    {
        SOUI::SStringW strFileName;
        DWORD    dwSize;
        SOUI::CCodeConfig cfg;
    };
public:
    CCodeLineCounterHandler(void);
    ~CCodeLineCounterHandler(void);

    void OnInit(SOUI::SWindow *pRoot);
protected:
    
    void OnBtnGo();

    EVENT_MAP_DECL_EX()

	SOUI::SWindow * m_pPageRoot;
protected:
	typedef SOUI::SMap<SOUI::SStringW, SOUI::CCodeConfig> CODECFGMAP;

	DWORD EnumFileInfo(const CODECFGMAP &cfg, const SOUI::SStringW & strPath, HSTREEITEM hItem, SOUI::SList<FILEINFO> & lstFileInfo, int &nDirs);
    CODECFGMAP m_mapCodeCfg;
};
