﻿#ifndef _S_CALENDER_H
#define _S_CALENDER_H

#include <windows.h>
namespace SOUI
{
	class SHBox : public SWindow
	{

	};

	class SVBox :public SWindow
	{

	};

	typedef struct tag_CalendarStyle
	{
		SStringT	nCalenderBorderColor;		//日历边框颜色
		SStringT	nCalendarBkColor;			//日历背景颜色
		SStringT	nMainTitleBkColor;			//日历主标题背景颜色
		SStringT	nSubTitleBkColor;			//日历副标题背景颜色
		SStringT	nWeekendColorA;				//周末的日期隔行背景颜色
		SStringT	nWeekendColorB;				//周末的日期隔行背景颜色 
		SStringT	nDayHotColor;				//日期获得焦点时背景颜色
		SStringT	nDayPushedColor;			//日期被按下时背景颜色
		SStringT	nDaySelectColor;			//日期被选中时背景颜色
		SStringT	nDayDisabledColor;			//日期被禁用时的背景色
		SStringT	nNoCurMonthDayColor;		//非本月日期的背景颜色
		SStringT	nWeekIntervalColorA;		//周隔行颜色A
		SStringT	nWeekIntervalColorB;		//周隔行颜色B
		SStringT	nStatusBkColor;				//底部信息栏背景色
	}TCalendarStyle;
	
	typedef struct tag_SubTitleString
	{
		SStringT	nSundayStr;
		SStringT	nMondayStr;
		SStringT	nTuesdayStr;
		SStringT	nWednesdayStr;
		SStringT	nThursdayStr;
		SStringT	nFridayStr;
		SStringT	nSaturdayStr;
		SStringT	nToDayString;
	}TSubTitleString;

	typedef struct tag_CalendarInfo
	{
		int		nYear;
		int		nMooth;
		int		nDay;
		int		nAsMooth;
		int		nWeek;
		int		nWeekLine;
	}TCalendarInfo;

	class ICalendar
	{
	public:
		virtual SWindow* CreateWeekPanel(SWindow* _Panent,int _Week) {return NULL;};
		virtual SWindow* CreateDayPanel(int _Week,LPCTSTR _GroupName) {return NULL;};
		virtual SWindow* CreateInfoPanel(){return NULL;};
		virtual void InitWeekPanel(SWindow* _Control,int _Week) {};
		virtual	void InitDayPanel(SWindow* _Control,bool bWeekEnd,TCalendarInfo& _CalendarInfo) {};
		virtual void InitDayPanel(SWindow* _Control,bool bWeekEnd,int _Year,int _Mooth,int _Day,int _Week,int _WeekLine,int _AsMooth) {};
		virtual void OnCalendarUpdateStart(int _Year,int _Mooth) {};
		virtual void OnCalendarUpdateEnd(int _Year,int _Mooth) {};
		virtual void OnCalendarChange(int _Year,int _Mooth,int _Day){};
	};
	
	class CCalendarDlg;

	class CCalendarUI : public SWindow,public ICalendar
	{
		friend class CCalendarDlg;
	public:
		CCalendarUI(void);
		~CCalendarUI(void);

		LPCTSTR	GetClass() const;
		
//		void	DoEvent(TEventUI& event);
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		SWindow *CreateWeekPanel(SWindow* _Panent,int _Week);
		SWindow *CreateDayPanel(int _Week,LPCTSTR _GroupName);
		SWindow *CreateInfoPanel();
		void InitWeekPanel(SWindow* _Control,int _Week);
		void InitDayPanel(SWindow* _Control,bool bWeekEnd,TCalendarInfo& _CalendarInfo);
		void InitDayPanel(SWindow* _Control,bool bWeekEnd,int _Year,int _Mooth,int _Day,int _Week,int _WeekLine,int _AsMooth);
		void OnCalendarUpdateStart(int _Year,int _Mooth);
		void OnCalendarChange(int _Year,int _Mooth,int _Day);
	public:
		bool IsLeapYear(int _Year);
		int  GetNumLeapYear(int _Year);
		int  DaysOfMonth(int _Mooth,int _Year = -1);
		void InitCalendarDis(int _Year,int _Month);
		void CalDateTime(TCalendarInfo* _TCalendarInfo);
		void CalDateTime(int _Year,int _Mooth,int _Day);
		SStringT	GetCurSelDateTime();
		void		SetComboTargetName(LPCTSTR pstrName);
		SStringT	GetComTargetName();
		SWindow *GetComTargetObj();
	public:
		void SetCallback(ICalendar* _ICalendar);

	public:
		bool SetSubTitleString(LPCTSTR _Name);
		void SetSubTitleString(TSubTitleString& _SubTitleString);
		void SetCalendarStyle(TCalendarStyle& _TCalendarStyle);
		bool AddSubTitleString(LPCTSTR _Name,TSubTitleString& _SubTitleString);
		bool AddSubTitleString(LPCTSTR _Name,LPCTSTR _Sunday,LPCTSTR _Monday,LPCTSTR _Tuesday,LPCTSTR _Wednesday,LPCTSTR _Thursday,LPCTSTR _Friday,LPCTSTR _Saturday);
		TSubTitleString& GetSubTitleString(LPCTSTR _Name = NULL);
		bool RemoveAtSubTitleString(LPCTSTR _Name);
		void RemoveAllSubTitleString();

	public:
		bool OnLastYear(void* _Param);
		bool OnMoothSelect(void* _Param);
		bool OnNextYear(void* _Param);
		bool OnSelectMooth(void* _Param);
		bool OnSelcetDay(void* _Param);
		bool OnToday(void* _Param);

	public:
		void SetEnabledMoothSel(bool _Enabled = true);
		bool GetEnabledMoothSel();
		void SetEnabledYearSel(bool _Enabled = true);
		bool GetEnabledYearSel();
		void SetMainTitleHeight(int _Height);
		int GetMainTitleHeight();
		void SetSubTitleHeight(int _Height);
		int GetSubTitleHeight();
		void SetStatusInfoHeight(int _Height);
		int GetStatusInfoHeight();

		void AnyCalendarStyle(LPCTSTR _StyleStr,TCalendarStyle* _TCalendarStyle = NULL);

		static void ShowCalendarDlg(SWindow* _pControl);
	private:
		SStringT				m_sComboTargetName;
		SHBox*	pMainTitleHoriz;
		SButton*				pLastYearBtn;
		SButton*				pMoothSelectBtn;
		SButton*				pNextYearBtn;

		SHBox*	pSubTitleHoriz;
		SButton*				pSundayBtn;
		SButton*				pMondayBtn;
		SButton*				pTuesdayBtn;
		SButton*				pWednesdayBtn;
		SButton*				pThursdayBtn;
		SButton*				pFridayBtn;
		SButton*				pSaturdayBtn;

		SButton*				pDateTimeBtn;
		SButton*				pToDayBtn;

		SHBox*	pMoothPanelHorz;
		SVBox*		pWeekPanelVert;
		SHBox*	pInfoPanelHorz;

		SStringT				mLastYearBtnString;
		SStringT				mMoothSelectBtnStrign;
		SStringT				mNextYearBtnString;

		TCHAR					m_cSep;
		int						mYear;
		int						mMonth;
		int						mToday;
		bool					pEnabledYearSel;
		bool					pEnabledMoothSel;
		struct tm*				pCurDateTime;

		ICalendar*				pICalendar;

		TCalendarStyle			m_DefaultStyle;
		TSubTitleString			mSubTitleString;
		SArray<TCalendarInfo*>			mTCalendarInfoArray;
		SMap<SStringT, TSubTitleString *>		mSubTitleStringArray;
	};
}

#endif // _S_CALENDER_H
