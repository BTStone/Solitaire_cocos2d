#ifndef __ST_SOLITAIRE_UI_COLLECT_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_COLLECT_CARD_AREA_H__


#include "UICollectCardQueue.h"
#include "CollectCardArea.h"
#include "TransportCardQueue.h"

class UICollectCardArea : public Node
{
public:
	static UICollectCardArea* create(CollectCardArea areaData);
	virtual bool init(CollectCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	int isContain(Vec2 worldPos);                                 //此点是否在收牌区内,是则返回队列下标，否则返回-1
	Card getTheNextCollectCard();                                 //获取下一个可以收取的牌的信息
	vector<Card> getTheCardsCanCollect();
	TransportCardQueue* search(Vec2 touchPos);                    //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	int getQueSize(int queIdx);
	int getAllCardSize();
	int getQueIndexByPos(Vec2 pos);                               //根据点确定此点所在的列的下标
	int canCollectTheCard(TransportCardQueue *que);               //整体判断是否能收取这个牌，能则返回所在队列的下标，不能则返回-1
	int canCollectTheCard(Card card);
	bool canCollectTheCard(TransportCardQueue *que, int queIdx);  //是否能收取这个牌
	void collectTheCard(TransportCardQueue *que, int queIdx);     //直接收取这张牌
	Vec2 getNextPosition(int queIdx);                             //下张牌要放置的世界坐标

	void addCardsAtIndex(CardQueue queData, int queIdx, bool withEffect = true);          //在某一队列下添加元素
	void clear();
	void updateCard();
	void syncData();                                              //同步数据，将m_areaDate与各队列的数据同步下
	string bakAllCards();                                         //备份所有牌，写进文件(返回将要备份的字符串）
	bool isEmpty();

	CardSprite* getTheLastCard(int queIdx);

	CC_SYNTHESIZE_READONLY(CollectCardArea, m_areaData, AreaData);
	vector<Card> removeTheLastNumCardsOfQue(int queIdx, int num);
	CollectCardArea* getData()
	{
		return &m_areaData;
	}
protected:
	UICollectCardQueue *m_queues[Collect_Area_Num];
};


#endif
