#ifndef __ST_SOLITAIRE_PLACE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_PLACE_CARD_QUEUE_H__

#include "CardQueue.h"

class PlaceCardQueue : public CardQueue
{
public:
	Card getTheFirstOpenCard();                                   //获取最上方打开牌
	int getTheFirstOpenCardIndex();                               //获取最上方打开牌的下标
	bool canPlaceTheCard(Card card);                              //是否可以放置牌
	void placeTheCard(Card card);                                 //直接放置这一张牌
	void placeTheCard(vector<Card> cards);                        //直接放置这一序列牌
	vector<Card> getAllOpenCards();                               //获取所有打开的牌
	vector<Card> getCardsAt(int cardIdx);                         //获取某一下标后的纸牌队列
	PlaceCardQueue& operator=(CardQueue &source);                 //重载=运算符
	int searchTheCard(Card card);                                 //在队列中搜索card，存在则返回下标，否则返回-1
};

#endif
