﻿#include "ImageMergerHandler.h"
#include "droptarget.h"
#include "../../controls.extend/FileHelper.h"

using namespace SOUI;

EVENT_MAP_BEGIN(CImageMergerHandler)
	EVENT_CHECK_SENDER_ROOT(m_pPageRoot)
	EVENT_NAME_COMMAND(L"btn_save", OnSave)
	EVENT_NAME_COMMAND(L"btn_clear", OnClear)
	EVENT_NAME_COMMAND(L"radio_horz", OnModeHorz)
	EVENT_NAME_COMMAND(L"radio_vert", OnModeVert)
	EVENT_NAME_COMMAND(L"btn_split", OnSplit)
EVENT_MAP_BREAK()

CImageMergerHandler::CImageMergerHandler(void):m_pPageRoot(NULL),m_pImgCanvas(NULL)
{
}

CImageMergerHandler::~CImageMergerHandler(void)
{
}

void CImageMergerHandler::OnFileDropdown( HDROP hdrop )
{
    bool success = false;
    WCHAR filename[MAX_PATH];
    UINT files = DragQueryFile(hdrop,(UINT)-1,NULL,0);
    for(UINT i=0;i<files;i++)
    {
        success=!!DragQueryFileW(hdrop, i, filename, MAX_PATH);
        if(success)
        {
            AddFile(filename);
        }
    }

}

void CImageMergerHandler::OnInit(SWindow *pRoot)
{
    m_pPageRoot = pRoot->FindChildByName(L"page_imagemerge");
    m_pImgCanvas = m_pPageRoot->FindChildByName2<SImgCanvas>(L"wnd_canvas");//CDropTarget_Canvas
    IDropTarget *pDT = new CDropTarget(this);
    pRoot->GetContainer()->RegisterDragDrop(m_pImgCanvas->GetSwnd(),pDT);
    pDT->Release();
}

void CImageMergerHandler::AddFile(LPCWSTR pszFileName)
{
    m_pImgCanvas->AddFile(pszFileName);
}

void CImageMergerHandler::OnSave()
{
    CFileDialogEx dlgSave(FALSE,_T("png"),0,6,_T("png files(*.png)\0*.png\0All files (*.*)\0*.*\0\0"));
    if(dlgSave.DoModal()==IDOK)
    {
        m_pImgCanvas->Save2File(S_CT2W(dlgSave.m_szFileName));
    }
}

void CImageMergerHandler::OnClear()
{
    m_pImgCanvas->Clear();
}

void CImageMergerHandler::OnModeHorz()
{
    if(!m_pImgCanvas) return;
    m_pImgCanvas->SetVertical(FALSE);
}

void CImageMergerHandler::OnModeVert()
{
    if(!m_pImgCanvas) return;
    m_pImgCanvas->SetVertical(TRUE);
}

void CImageMergerHandler::OnSplit()
{
    int nNumber = _ttoi(m_pPageRoot->FindChildByName(L"edit_number")->GetWindowText());
    if(nNumber<2 || nNumber>100)
    {
        SMessageBox(GetActiveWindow(),_T("支持切分为2-100子图"),_T("输入错误"),MB_ICONERROR|MB_OK);
    }else
    {
        CFileDialogEx dlgSave(FALSE,_T("png"),0,6,_T("png files(*.png)\0*.png\0All files (*.*)\0*.*\0\0"));
        if(dlgSave.DoModal() == IDOK)
        {
            m_pImgCanvas->Save2File(S_CT2W(dlgSave.m_szFileName),nNumber);
        }
    }
}

