#ifndef __ST_CALENDAR_ITEM_H__
#define __ST_CALENDAR_ITEM_H__

#include "STBase.h"

class CalendarItem : public Node
{
public:
	static CalendarItem* create(int day);
	virtual bool init(int day);
	void initContent();                    //��ʼ����ť�͵��������m_dayLabel
	void initCrown();                      //��ʼ���ʹ�
	void refreshStatus();                  //��������ˢ��״̬
	Size getSize();
	void select();      //ѡ��,��ʼ��ʱ������Ϣ
	void unselect();                       //δѡ��
	bool isAfterToday();                   //�����Ƿ��ڽ���֮��
	bool isToday();                        //�����ǽ���
	bool isTodaySuccess();                 //�жϵ����Ƿ�ɹ�
	bool isYesterday();                    //�Ƿ�������
	bool isTheDayBeforeYesterday();              //�Ƿ���ǰ��
	void afterToday();                     //�ڵ���֮��
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
	Button* m_button;                       //��Ӧ����İ�ť
	STLabelBMFont *m_dayLabel;
	Sprite *m_todaySpriteBg;                //�����ʶ
	Sprite *m_selectSpriteBg;               //ѡ��ʱ�ı���
	Sprite *m_crownSprite;                  //�ʹ�
};

#endif
