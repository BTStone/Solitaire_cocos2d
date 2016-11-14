#ifndef __ST_SOLITAIRE_UI_COLLECT_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_COLLECT_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UICollectCardQueue : public UICardQueue
{
public:
	static UICollectCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                                     //设置位置
	TransportCardQueue* search(Vec2 touchPos);                    //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	bool containThePos(Vec2 worldPos);                            //此点是否在这个队列内
	bool intersectsTheCard(Vec2 worldCardPos);

	bool canCollectTheCard(Card card);                            //判断是否可以收取这张牌
	void collectTheCard(Card card);                               //直接收取
	Vec2 getNextPosition();                                       //下张牌要放置的世界坐标

	void addCards(CardQueue queData, bool withEffect = true);

	void addCollectCardEffect(CardColor color);                   //收牌特效
};

#endif