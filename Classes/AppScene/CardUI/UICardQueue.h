#ifndef __ST_SOLITAIRE_UI_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_CARD_QUEUE_H__

#include "CardSprite.h"
#include "CardQueue.h"

enum CardArea
{
	COLLECT_AREA,              //收牌区
	HAND_CLOSE_AREA,           //手牌不可见区
	HAND_OPEN_AREA,            //手牌可见区
	PLACE_AREA                 //置牌区
};

class UICardQueue : public Node
{
public:
	static UICardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCards();
	virtual void initCardBg();
	Card removeTheLastCard();                                       //删除最后一个元素
	vector<Card> removeCardsAfterIndex(int index);                  //删除某个元素（包括这个元素）的所有元素
	vector<Card> removeLastCardByNum(int num);                      //从队列末尾删除num数量的牌
	void removeAllCards();                                          //删除所有牌
	CardSprite* operator [](int index);                             //重载[]运算符
	CardSprite* getTheLastCard();                                   //获取最后一张牌
	CardSprite* getTheFirstCard();                                  //获取第一张牌
	void addTipLightAfterIndex(int idx);                            //添加提示光圈
	void removeTipLight();
	void removeSelf(Node *node);
	//virtual TransportCardQueue* search(Vec2 touchPos) = 0;          //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除

	int size();                                                     //m_cards的长度
	void clear();                                                   //清空
	bool isEmpty();                                                 //判断队列中是否为空
	bool isAllCardsOpen();
	Vec2 getWorldPosOfCardBg();                                     //获取第一个位置（cardBg的位置）的世界坐标
	void updateCard();

	CC_SYNTHESIZE(CardQueue, m_queData, QueData);                   //数据队列
	CC_SYNTHESIZE(CardArea, m_resArea, ResArea);                    //当前所属区域

protected:                                          
	vector<CardSprite*> m_cards;                                    //UI队列
	Sprite *m_cardBg;                                               //每一队列空的时候的背景
	Node*m_tipLight;
};

#endif
