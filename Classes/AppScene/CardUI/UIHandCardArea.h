#ifndef __ST_SOLITAIRE_UI_HAND_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_HAND_CARD_AREA_H__


#include "UIHandClosedCardQueue.h"
#include "UIHandOpenCardQueue.h"
#include "HandCardArea.h"
#include "TransportCardQueue.h"

class UIHandCardArea : public Node
{
public:
	static UIHandCardArea* create(HandCardArea areaData);
	virtual bool init(HandCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	Vec2 getClosedAreaPosition();                                  //获取未打开区域的世界坐标
	Vec2 getOpenAreaLastCardPosition();                            //获取打开区最后一个元素的坐标
	Vec2 getNextPositionOfOpenQue();                               //获取打开区下一张牌的放置位置
	Vec2 getNextPositionOfClosedQue();                             //获取未打开区下一张牌的放置位置

	TransportCardQueue* search(Vec2 touchPos);
	bool inColosedArea(Vec2 worldPos);

	void addCardsAtOpenArea(CardQueue queData);                    //在打开区添加纸牌
	void addCardsAtClosedArea(CardQueue queData);                  //在非打开区添加纸牌

	void flipCard();                                               //翻牌
	void shuffleCard();                                            //洗牌
	void openAreaRemoveCardEffectWithNum(int remNum);              //打开区域减少牌的时候重新移动位置
	void openAreaAddCardEffectWithNum(int addNum);                 //打开区域增减牌的时候重新移动位置
	bool isEmpty();                                                //打开和关闭区是否都为空
	void updateCard();

	int getClosedAreaSize();
	int getOpenAreaSize();

	CardSprite* getOpenAreaLastCard();

	CC_SYNTHESIZE_READONLY(HandCardArea, m_areaData, AreaData);
	CC_SYNTHESIZE_READONLY(Size, m_areaSize, AreaSize);
	void addClosedAreaTip();                                     //增加提示
	void addOpenAreaTip();                                       //增加提示
	vector<Card> removeTheLastNumCardsOfOpenQue(int num);
	vector<Card> removeTheLastNumCardsOfClosedQue(int num);

	bool canAutoCollect();                                       //能否自动收牌
	
	HandCardArea* getData()
	{
		return &m_areaData;
	}
	
	void syncData();                                              //同步数据，将m_areaDate与各队列的数据同步下
	string bakAllCards();                                          //备份所有牌，写进文件(返回将要备份的字符串）

	void setHandType(bool isLeft);
	void setLeftHandType();
	void setRightHandType();

protected:
	void transportArrivedCallFunc(Node *node);                     //运输类到达目的地

protected:
	UIHandClosedCardQueue *m_closedQueue;
	UIHandOpendCardQueue *m_openQueue;
};


#endif
