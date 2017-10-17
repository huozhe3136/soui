#pragma once

namespace SOUI
{
    class SOUI_EXP SAppDir 
    {
    public:
        SAppDir(HINSTANCE hInst);
		~SAppDir();

        const SStringT & AppDir();
    protected:
        SStringT    m_strAppPath;
    };

}
