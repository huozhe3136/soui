﻿#pragma once

//作者：望天涯

#include "helper/SMatrix.h"
namespace SOUI
{

class SRotateWindow :
	public SWindow
{
public:
	SOUI_CLASS_NAME_DECL(SRotateWindow,L"RotateWindow");
	
	enum {TIMER_REFRESH = 1};
	enum {TURN_LEFT, TURN_RIGHT};
	SRotateWindow(void)
	{
		m_nStep = 0;
		m_nSpeed = 0;
		m_nTurn = TURN_RIGHT;
	}


protected:

	int	OnCreate(void*)
	{
		SetTimer(TIMER_REFRESH, 25);
		return 0;
	}

	SMatrix InitMatrix(double angle, CPoint &center)
	{
		return SMatrix().translate((FLOAT)center.x,(FLOAT)center.y)
			.rotate((FLOAT)angle/*-90*/)
			.translate((FLOAT)-center.x,(FLOAT)-center.y);
	}

	void OnPaint(SOUI::IRenderTarget * pRT)
	{
		SWindow::OnPaint(pRT);

		if ( !m_pAnimImg )
			return ;

		int nTmp = m_nSpeed * m_nStep;
		if (nTmp % 360 == 0)
			m_nStep = 0;


		CRect m_rcItem;
		GetClientRect(&m_rcItem);
		CPoint center( (m_rcItem.right - m_rcItem.left)/2 + m_rcItem.left, (m_rcItem.bottom - m_rcItem.top)/2 + m_rcItem.top);

		SMatrix form = InitMatrix( m_nTurn == TURN_RIGHT ?  nTmp : -nTmp, center);
		CRect rcImg(CPoint(0,0),m_pAnimImg->Size());

		pRT->SetTransform(&form, NULL);
		pRT->DrawBitmapEx(m_rcItem, m_pAnimImg ,&rcImg , EM_TILE, 200);
		SMatrix s;
		pRT->SetTransform(&s);
	}

	void OnTimer(char cTimerID)
	{
		m_nStep++;
		Invalidate();
	}

	SOUI_MSG_MAP_DECL()

	SOUI_ATTRS_DECL()

protected:
	INT m_nSpeed;
	int m_nTurn;
private:
	int m_nStep;
	CAutoRefPtr<IBitmap> m_pAnimImg;
	
};



}