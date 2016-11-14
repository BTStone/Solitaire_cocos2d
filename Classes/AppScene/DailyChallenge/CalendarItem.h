#ifndef __ST_CALENDAR_ITEM_H__
#define __ST_CALENDAR_ITEM_H__

#include "STBase.h"

class CalendarItem : public Node
{
public:
	static CalendarItem* create(int day);
	virtual bool init(int day);
	void initContent();                    //初始化按钮和点击背景和m_dayLabel
	void initCrown();                      //初始化皇冠
	void refreshStatus();                  //根据数据刷新状态
	Size getSize();
	void select();      //选中,初始化时不发消息
	void unselect();                       //未选中
	bool isAfterToday();                   //当天是否在今天之后
	bool isToday();                        //当天是今天
	bool isTodaySuccess();                 //判断当天是否成功
	bool isYesterday();                    //是否是昨天
	bool isTheDayBeforeYesterday();              //是否是前天
	void afterToday();                     //在当天之后
	void addTodayEffect();
	void hideCrown();
	float getCrownScale();
	void showDayLabel();
	void hideDayLabel();
protected:
	void menuSelectCallBack(Ref *pSender, Widget::TouchEventType type);
	void todayEffctScaleEndCallFunc(Node *node);

protected:
	CC_SYNTHESIZE_READONLY(string, m_crownPicName, CrownPicName);
protected:
	int m_day;
	Button* m_button;                       //响应点击的按钮
	STLabelBMFont *m_dayLabel;
	Sprite *m_todaySpriteBg;                //当天标识
	Sprite *m_selectSpriteBg;               //选择时的背景
	Sprite *m_crownSprite;                  //皇冠
};

#endif
