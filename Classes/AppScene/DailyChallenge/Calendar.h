/*
�������������������Ϸ�������
ͨ�������Ĵ�Сȷ�������Ĵ�С��
������ê��������
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

	static int getWeekDayOfDate(int year, int month, int day);  //�������ڻ�ȡ���ڼ�
	static int getTotalDayOfMonth(int year, int mon);           //��ȡһ���¶�����

protected:
	void btnLeftCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnRightCallBack(Ref *pSender, Widget::TouchEventType type);

	void msgSelectCalendarDayCallBack(Ref *ref);

	void successEffectEndCallFunc(Node *node);

	CC_SYNTHESIZE_READONLY(Size, m_calendarSize, CalendarSize);
	CC_SYNTHESIZE_READONLY(int, m_selectDay, SelectDay);                //��ǰ��ѡ�������
protected:
	Node *m_rootCSBNode;

	int m_preMonCount;                        //������ǰ�·�ѡ���ʣ����Ŀ
	int m_maxLookMonCount;                    //������ǰ�·�ѡ�������Ŀ
	static int m_curYear;                     //��ǰ���
	static int m_curMonth;                    //��ǰ�·�
	vector<Vec2> m_positionList;              //6 * 7 = 42��λ�ü���
	vector<CalendarItem *> m_calendarItems;
	int m_originDay;
};
#endif
