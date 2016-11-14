#ifndef __ST_SOLITAIRE_UI_PLACE_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_PLACE_CARD_AREA_H__


#include "UIPlaceCardQueue.h"
#include "PlaceCardArea.h"
#include "TransportCardQueue.h"

class UIPlaceCardArea : public Node
{
public:
	static UIPlaceCardArea* create(PlaceCardArea areaData);
	virtual bool init(PlaceCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	int getQueSize(int queIdx);
	TransportCardQueue* search(Vec2 worldPos);                    //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	int searchTheCard(Card card);                                 //返回此牌所在队列下标，没有则返回-1
	int getQueIndexByPos(Vec2 worldPos);                          //根据点确定此点所在的列的下标
	int canPlaceTheCard(TransportCardQueue *que);                 //整体判断是否能收取这个牌，能则返回所在队列的下标，不能则返回-1
	bool canPlaceTheCard(TransportCardQueue *que, int queIdx);    //是否能放置这个牌
	Vec2 getNextPosition(int queIdx);                             //下张牌要放置的世界坐标

	void addCardsAtIndex(CardQueue queData, int queIdx);          //在某一队列下添加元素

	bool tryToOpenTheLastCardOfTheQue(int queIdx);                //尝试打开某一列的最后一张牌
	bool tryToCloseTheLastCardOfTheQue(int queIdx);               //尝试关闭某一列的最后一张牌
	bool isInTheLastCardOfTheQue(int queIdx, Vec2 worldPos);      //是否点击在某一列的最后一张牌上，用于拖动时

	bool isMoveEffectly(int resIdx, int destIdx);                 //置牌区之间的移动是否是有效移动（循环移动）

	Vec2 getCardWorldPosition(int row, int cow);                  //根据行，列（从0开始）获取坐标
	vector<Vec2> getAllCardsWorldPosition();                      //获取所有牌的世界坐标
	void openLastCards();                                         //打开所有队列的最后一张
	void updateCard();
	Vec2 getTheLastCardPosOfTheQue(int queIdx);
	void syncData();                                              //同步数据，将m_areaDate与各队列的数据同步下
	string bakAllCards();                                         //备份所有牌，写进文件(返回将要备份的字符串）
	bool isEmpty();
	bool isAllCardsOpen();
	vector<Card> getCardsAt(int queIdx, int cardIdx);             //获取某一队列某一下标之后的所有牌的信息
	CC_SYNTHESIZE_READONLY(PlaceCardArea, m_areaData, AreaData);
	void addTipLightAt(int queIdx, int cardIdx);
	void removeAllTipLight();
	vector<Card> removeTheLastNumCardsOfQue(int queIdx, int num);

	void tryToScale();
	bool tryToScaleTheQue(int queIdx);
	PlaceCardArea* getData()
	{
		return &m_areaData;
	}
	void stTest_scale();
protected:
	UIPlaceCardQueue *m_queues[Place_Area_Num]; 
	float m_emptyOffX;            //无用宽度X
};


#endif
