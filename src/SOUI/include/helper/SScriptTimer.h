﻿#pragma once

#include "core/SSingletonMap.h"


namespace SOUI
{

    struct SCRIPTTIMERINFO
    {
		SCRIPTTIMERINFO();
		~SCRIPTTIMERINFO();
        HWND hwnd;
        SStringA strScriptFunc;
        BOOL bRepeat;
    };

    class SScriptTimer : public SSingletonMap<SScriptTimer,SCRIPTTIMERINFO,UINT_PTR>
    {
    public:
        ~SScriptTimer() override;

        UINT SetTimer(HWND hwnd,const SStringA & strScriptFunc,UINT nElapse,BOOL bRepeat);

		void ClearTimer(UINT_PTR uID);

        static VOID CALLBACK _TimerProc(HWND hwnd,
            UINT uMsg,
            UINT_PTR idEvent,
            DWORD dwTime
            );
    };

}//namespace SOUI