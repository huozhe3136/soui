﻿#pragma once

#include <core/simplewnd.h>
#include <Scintilla.h>

using namespace SOUI;
// CScintillaWnd
class CScintillaWnd : public CSimpleWnd
{
public:
	static int InitScintilla(HINSTANCE hInst);
	static void UninitScintilla();
	static LPCTSTR GetScintillaWndClass();

	CScintillaWnd();
	virtual ~CScintillaWnd();
	BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND hParentWnd, UINT nID,HINSTANCE hInst);
	BOOL OpenFile(LPCTSTR lpFileName);
	BOOL SaveFile(LPCTSTR lpFileName);

	LPCTSTR GetOpenedFileName(){return m_strFileName;}
	void SetOpenedFileName(LPCTSTR pszFileName){m_strFileName=pszFileName;}

	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
		return ::SendMessage(m_hWnd, Msg, wParam, lParam);
	}
protected:
	// 显示行号
	void UpdateLineNumberWidth(void);
	void InitScintillaWnd(void);
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff,0xff,0xff), int size = 0, const char* face = NULL);
	// 设置XML的语法规则
	void SetXmlLexer();
	void GetRange(int start, int end, char* text);

	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

	BEGIN_MSG_MAP_EX_DECL()

	SStringT m_strFileName;
};
