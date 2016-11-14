#ifndef __ST_SOLITAIRE_UI_HAND_OPEN_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_HAND_OPEN_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UIHandOpendCardQueue : public UICardQueue
{
public:
	static UIHandOpendCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                              //设置位置
	void resetCardBg();                                    //重置空位时的背景图片
	TransportCardQueue* search(Vec2 touchPos);             //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	void addCard(Card card);
	void addCards(CardQueue queData);
	void addCardEffectWithNum(int addNum);                 //当添加元素时，根据添加元素的个数，作出相应的移动
	void removeCardEffectWithNum(int remNum);              //当删除元素时，根据添加元素的个数，作出相应的移动

	void lastTwoCardsMoveByOnePosToLeft();                 //最后两个元素向左移动一个位置
	void lastTwoCardsMoveToMostLeft();                     //最后两个元素都移动到最左边的位置
	void lastTwoCardsMoveByOnePosToRight();                //最后两个元素向右移动一个位置
	void lastTwoCardsMoveToRightIrregular();               //最后两个元素向右不规则移动

	void addTip();
	void removeTip();

	bool isOnlyOneCard();
protected:
	Sprite *m_tip;
};

#endif