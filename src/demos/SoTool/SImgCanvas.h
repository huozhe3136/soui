#pragma once

namespace SOUI
{
    class SImgCanvas : public SWindow
    {
        SOUI_CLASS_NAME_DECL(SImgCanvas,L"imgcanvas")
    public:
        SImgCanvas(void);
        ~SImgCanvas(void);
        
        BOOL AddFile(LPCWSTR pszFileName);
        void Clear();
       
        BOOL Save2File(LPCWSTR pszFileName,int nSplit=1);
        void SetVertical(BOOL bVert);
    protected:
        void OnPaint(IRenderTarget *pRT);
        
        
        SOUI_MSG_MAP_DECL()
        
        BOOL            m_bVert;
        SList<IBitmap*> m_lstImg;
    };
}
