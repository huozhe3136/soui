#pragma once
#include "FolderHander.h"
#include "IconvWorker.h"

class C2UnicodeHandler : public CFolderHander
{
    friend class CMainDlg;
public:
    C2UnicodeHandler(void);
    ~C2UnicodeHandler(void);

    void OnInit(SOUI::SWindow *pPageRoot);
protected:
    void OnGo();

    EVENT_MAP_DECL_EX()

protected:
	typedef SOUI::SMap<SOUI::SStringW, int> FILETYPEMAP;
	void EnumFileInfo(const FILETYPEMAP &cfg, const SOUI::SStringW & strPath, HSTREEITEM hItem, SOUI::SList<SOUI::SStringW> & lstFileInfo, int &nDirs);
};
