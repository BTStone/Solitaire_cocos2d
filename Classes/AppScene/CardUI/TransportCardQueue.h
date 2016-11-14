/*此类用于拖动或者智能移动过程中的运输队列*/
#ifndef __ST_SOLITAIRE_TRANSPORT_CARD_QUEUE_H__
#define __ST_SOLITAIRE_TRANSPORT_CARD_QUEUE_H__

#include "UICardQueue.h"


/// 振动效果
class STShock : public ActionInterval
{
public:
	//super methods
	static STShock* create(float dt);
	bool init(float dt);
	void update(float time);
	void shock();
protected:
	int m_time;            //进入的次数
};

class TransportCardQueue : public UICardQueue
{
public:
	static TransportCardQueue* create(CardQueue que, CardArea area, int preIndex = 0);
	virtual bool init(CardQueue que, CardArea area, int preIndex);
	void setPortraitStatus();                                  //竖直方向设置位置
	void setLandscapeStatus();                                 //水平方向设置位置
	void setHandOpenAreaStatus();                              //水平方向设置位置
	void resetCardBg();                                        //重置空位时的背景图片

	CC_SYNTHESIZE(int, m_resQueIndex, ResQueIndex);            //此队列之前所在区域的下标
	CC_SYNTHESIZE(CardArea, m_destArea, DestArea);             //目的区域
	CC_SYNTHESIZE(int, m_destQueIndex, DestQueIndex);          //目的区域的队列

protected:

};

#endif