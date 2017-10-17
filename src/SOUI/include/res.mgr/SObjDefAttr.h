﻿#pragma once

#include "core/SSingletonMap.h"


namespace SOUI
{

class SOUI_EXP SObjDefAttr :public SCmnMap<pugi::xml_node,SStringW>
{
public:
	SObjDefAttr();
	~SObjDefAttr() override;

    BOOL Init(pugi::xml_node xmlNode);
    
    bool IsEmpty(){return !!m_xmlRoot.root();}
    
    pugi::xml_node GetDefAttribute(LPCWSTR pszClassName);
protected:
    void BuildClassAttribute(pugi::xml_node & xmlNode, LPCWSTR pszClassName);

    pugi::xml_document m_xmlRoot;
};

}//namespace SOUI

