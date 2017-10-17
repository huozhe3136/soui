/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       SHostDialog.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    SOUI的Dialog模块
*/

#pragma once

#include "shostwnd.h"
#include "smsgloop.h"

namespace SOUI
{
    class SOUI_EXP SHostDialog : public SHostWnd
    {
        SOUI_CLASS_NAME_DECL(SHostDialog,L"hostdlg")
    public:
        SHostDialog(LPCTSTR pszXmlName);
        ~SHostDialog(void) override;
        
        virtual INT_PTR DoModal(HWND hParent=NULL);
        
        virtual void EndDialog(INT_PTR nResult);

    protected:
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        void OnOK();
        void OnCancel();
		SMessageLoop * GetMsgLoop() override; 
        
        EVENT_MAP_DECL()

        BEGIN_MSG_MAP_EX_DECL()

        INT_PTR m_nRetCode;
        
        SMessageLoop * m_MsgLoop;
    };

}
