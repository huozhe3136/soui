//////////////////////////////////////////////////////////////////////////
//   File Name: sresprovider.h
// Description: Resource Provider
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "interface/SResProvider-i.h"
#include <helper/SResID.h>
#include <unknown/obj-ref-impl.hpp>

namespace SOUI
{
    //定义3种系统资源类型
    extern const  TCHAR KTypeBitmap[];
    extern const  TCHAR KTypeCursor[];
    extern const  TCHAR KTypeIcon[];  
    extern const  TCHAR KTypeHtml[];

    class SOUI_EXP SResLoadFromMemory
    {
    public:
        static IBitmap * LoadImage(LPVOID pBuf,size_t size);
        static IImgX   * LoadImgX(LPVOID pBuf,size_t size);
    };
    
    class SResProviderPE:public TObjRefImpl<IResProvider>
    {
    public:
        SResProviderPE();
        BOOL Init(WPARAM wParam,LPARAM lParam) override;
        HBITMAP    LoadBitmap(LPCTSTR pszResName) override;
        HICON   LoadIcon(LPCTSTR pszResName,int cx=0,int cy=0) override;
        HCURSOR   LoadCursor(LPCTSTR pszResName) override;
        IBitmap * LoadImage(LPCTSTR strType,LPCTSTR pszResName) override;
        IImgX   * LoadImgX(LPCTSTR strType,LPCTSTR pszResName) override;
        BOOL GetRawBuffer(LPCTSTR strType,LPCTSTR pszResName,LPVOID pBuf,size_t size) override;
        BOOL HasResource(LPCTSTR strType,LPCTSTR pszResName) override;
        size_t GetRawBufferSize(LPCTSTR strType,LPCTSTR pszResName) override;
        LPVOID GetRawBufferPtr(LPCTSTR strType,LPCTSTR pszResName);
        
#ifdef _DEBUG
        void CheckResUsage(const SMap<SStringT,int> & mapResUsage) override;
#endif
        
    protected:
        HRSRC MyFindResource(LPCTSTR strType, LPCTSTR pszResName );


        HINSTANCE m_hResInst;
    };


    class SOUI_EXP SResLoadFromFile
    {
    public:
        static HBITMAP LoadBitmap(LPCTSTR pszFileName);
        static HICON   LoadIcon(LPCTSTR pszFileName,int cx=0,int cy=0);
        static HCURSOR LoadCursor(LPCTSTR pszFileName);
        static IBitmap * LoadImage(LPCTSTR pszFileName);
        static IImgX   * LoadImgX(LPCTSTR pszFileName);
        static size_t GetRawBufferSize(LPCTSTR pszFileName);
        static BOOL GetRawBuffer(LPCTSTR pszFileName,LPVOID pBuf,size_t size);
    };

    class SResProviderFiles:public TObjRefImpl<IResProvider>
    {
    public:

        SResProviderFiles();
		~SResProviderFiles() override;

        BOOL Init(WPARAM wParam,LPARAM lParam) override;

        BOOL HasResource(LPCTSTR strType,LPCTSTR pszResName) override;
        HBITMAP    LoadBitmap(LPCTSTR pszResName) override;
        HICON   LoadIcon(LPCTSTR pszResName,int cx=0,int cy=0) override;
        HCURSOR LoadCursor(LPCTSTR pszResName) override;
        IBitmap * LoadImage(LPCTSTR strType,LPCTSTR pszResName) override;
        IImgX   * LoadImgX(LPCTSTR strType,LPCTSTR pszResName) override;
        size_t GetRawBufferSize(LPCTSTR strType,LPCTSTR pszResName) override;
        BOOL GetRawBuffer(LPCTSTR strType,LPCTSTR pszResName,LPVOID pBuf,size_t size) override;
        
#ifdef _DEBUG
        void CheckResUsage(const SMap<SStringT,int> & mapResUsage) override;
#endif
    protected:
        SStringT GetRes( LPCTSTR strType,LPCTSTR pszResName );


        SStringT m_strPath;
        SMap<SResID,SStringT> m_mapFiles;
    };

    BOOL SOUI_EXP CreateResProvider(BUILTIN_RESTYPE resType,IObjRef **pObj);

}//namespace SOUI