#ifndef __ST_SOLITAIRE_UI_HAND_CLOSED_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_HAND_CLOSED_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UIHandClosedCardQueue : public UICardQueue
{
public:
	static UIHandClosedCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                               //设置位置
	void resetCardBg();                                     //重置空位时的背景图片
	TransportCardQueue* search(Vec2 touchPos);              //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	bool containThePos(Vec2 worldPos);
	void addCard(Card card);
	void addCards(CardQueue queData);
	void addTip();
	void removeTip();
protected:
	Sprite *m_tip;
};

#endif