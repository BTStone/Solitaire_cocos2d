/*
日历，包括日期区和上方操作区
通过传来的大小确定日历的大小，
日历的锚点在中心
*/

#ifndef __ST_CALENDAR_H__
#define __ST_CALENDAR_H__

#include "CalendarItem.h"
#define St_Left_Arrow_Menu  100
#define St_Right_Arrow_Menu  101

class Calendar : public Node
{
public:
	Calendar();
	~Calendar();
	static int getCurYear();
	static int getCurMonth();
	static Calendar* create(int year, int mon,int day);
	virtual bool init(int year, int mon, int day);
	void initRootCSBNode();

	void initPositionList();
	void handleButton();

	void refresh();
	void refreshCurMonthLabel();
	void refreshSelectDay();
	void refreshArrowMenu();
	void updateWeekLabel();
	void clearCalendar();

	int getTheLastNotSuccessDayOfMon(int year, int mon);

	void addSuccessEffect();

	static int getWeekDayOfDate(int year, int month, int day);  //根据日期获取星期几
	static int getTotalDayOfMonth(int year, int mon);           //获取一个月多少天

protected:
	void btnLeftCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnRightCallBack(Ref *pSender, Widget::TouchEventType type);

	void msgSelectCalendarDayCallBack(Ref *ref);

	void successEffectEndCallFunc(Node *node);

	CC_SYNTHESIZE_READONLY(Size, m_calendarSize, CalendarSize);
	CC_SYNTHESIZE_READONLY(int, m_selectDay, SelectDay);                //当前所选择的日期
protected:
	Node *m_rootCSBNode;

	int m_preMonCount;                        //可以往前月份选择的剩余数目
	int m_maxLookMonCount;                    //可以往前月份选择的总数目
	static int m_curYear;                     //当前年份
	static int m_curMonth;                    //当前月份
	vector<Vec2> m_positionList;              //6 * 7 = 42个位置集合
	vector<CalendarItem *> m_calendarItems;
	int m_originDay;
};
#endif
