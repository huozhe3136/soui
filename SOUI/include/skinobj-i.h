#pragma once

#include "duiobject.h"
#include "unknown/obj-ref-i.h"
#include "unknown/obj-ref-impl.hpp"

namespace SOUI
{

class SOUI_EXP ISkinObj : public SObject,public TObjRefImpl2<IObjRef,ISkinObj>
{
public:
    ISkinObj():m_strOwner("")
    {
    }
    virtual ~ISkinObj()
    {
    }

    void SetOwner(CDuiStringA strOwner)
    {
        m_strOwner=strOwner;
    }

    CDuiStringA GetOwner()
    {
        return m_strOwner;
    }

    virtual void Draw(IRenderTarget *pRT, LPCRECT rcDraw, DWORD dwState,BYTE byAlpha=0xFF)=0;

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return TRUE;
    }

    virtual int GetStates()
    {
        return 1;
    }
protected:
    CDuiStringA	m_strOwner;
};



}//namespace SOUI