#ifndef __ST_SOLITAIRE_PLACE_CARD_AREA_H__
#define __ST_SOLITAIRE_PLACE_CARD_AREA_H__

#include "PlaceCardQueue.h"
#include "STBase.h"

struct TipInfo;
class PlaceCardArea
{
public:
	void clear();
	void initQueue(int queIdx, CardQueue que);
	PlaceCardQueue operator[](int index);
	string bakAllCards();
	void setQue(int idx, CardQueue que);
	void addCardsAt(int queIdx, vector<Card> cards);
	void stTest_print();
	vector<Card> getAllOpenCardsAt(int queIdx);              //获取所有打开的牌
	vector<Card> getCardsAt(int queIdx, int cardIdx);        //
	vector<Card> removeCardsAt(int queIdx, int cardIdx);     //获取某一队列某一下标后的纸牌队列
	void openAllLastCard();                                  //将每一列最后一张牌设置为打开
	int tryToPlaceTheCard(Card card);                        //尝试放置这张牌，若可以则返回队列下标，否则返回-1
	int searchTheCardAt(Card card, int queIdx);              //从queIdx中搜索card，存在则返回card的下标，不存在则返回-1
	int canPlaceTheCardFromCollectArea(Card card);           //是否能够从收牌区取回这张牌
	int theFirstOpenCardOfQueCanBehindTheCard(Card card);         //七个队列里的最后一张牌是否能够连接在card之后
	string convertToInfoStr();
protected:
	PlaceCardQueue m_queues[Place_Area_Num];
};


#endif
