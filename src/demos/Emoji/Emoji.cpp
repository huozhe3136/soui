// dui-demo.cpp : main source file
//

#include "MainDlg.h"
#include "FaceList_soui.h"
#include "SVscrollbar.h"
#include "gif/SMyGifPlayer.h"
#include "SMyImage.h"
#include "res/R.h"
#include "com-cfg.h"
//从PE文件加载，注意从文件加载路径位置
#define RES_TYPE 1
//#define RES_TYPE 0   //从文件中加载资源
// #define RES_TYPE 1  //从PE资源中加载UI资源

SOUI_CLASS_NAME(SOUI::SSkinVScrollbar, L"vscrollbar")

//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
using namespace SOUI;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	HRESULT hRes = OleInitialize(NULL);
	(void)hRes;
	SASSERT(SUCCEEDED(hRes));

	int nRet = 0;

	SComMgr *pComMgr = new SComMgr;

	//将程序的运行路径修改到项目所在目录所在的目录
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos + 1, _T(""));
	SetCurrentDirectory(szCurrentDir);
	{
		BOOL bLoaded = FALSE;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		bLoaded = pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SApplication *theApp = new SApplication(pRenderFactory, hInstance);

		theApp->RegisterWindowClass<SMyImage>();
		theApp->RegisterWindowClass<SMyGifPlayer>();//注册MySGifPlayer
		theApp->RegisterSkinClass<SSkinVScrollbar>();
		theApp->RegisterSkinClass<SSkinGif>();//注册SkinGif

		{
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hInstance, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
		}

		CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
		CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
		if (!pResProvider->Init((LPARAM)_T("uires"), 0))
		{
			SASSERT(0);
			return 1;
		}
#else 
		CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
		pResProvider->Init((WPARAM)hInstance, 0);
#endif

		theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
		theApp->AddResProvider(pResProvider);
		SSkinGif::Gdiplus_Startup();
		// BLOCK: Run application
		{
			CFaceList_SOUI* pSystemfaceList = new CFaceList_SOUI(_T("Resource\\Face\\FaceConfig.xml"));
			pSystemfaceList->LoadConfigFile();
			CFaceList_SOUI* pMyfaceList = new CFaceList_SOUI(_T("test.xml"));
			pMyfaceList->LoadConfigFile();
			CEmojiDlg dlgEmoji;
			dlgEmoji.SetFaceList(pSystemfaceList, pMyfaceList, NULL);
			dlgEmoji.Create(GetActiveWindow(), WS_POPUP | WS_CLIPCHILDREN | WS_TABSTOP, 0, 0, 0, 0, 0);
			dlgEmoji.SendMessage(WM_INITDIALOG);
			dlgEmoji.CenterWindow(dlgEmoji.m_hWnd);
			dlgEmoji.ShowWindow(SW_SHOWNORMAL);
			nRet = theApp->Run(dlgEmoji.m_hWnd);
			delete pSystemfaceList;
			delete pMyfaceList;
			SStringT szResult;
			szResult.Format(_T("选择表情: %s\n"), dlgEmoji.m_strSelectEmoji);
			OutputDebugString(szResult);
		}
		SSkinGif::Gdiplus_Shutdown();
		delete theApp;
	}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}
