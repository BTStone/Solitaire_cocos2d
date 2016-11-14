#ifndef __ST_SOLITAIRE_COLLECT_CARD_AREA_H__
#define __ST_SOLITAIRE_COLLECT_CARD_AREA_H__

#include "CardQueue.h"
#include "STBase.h"

class CollectCardArea
{
public:
	void initQueue(int queIdx, CardQueue que);             //初始化某个队列
	CardQueue operator[](int index);
	string bakAllCards();                                  //备份所有牌，写进文件(返回将要备份的字符串）
	void stTest_print();
	void clear();
	void setQue(int idx, CardQueue que);
	void addCardsAt(int queIdx, vector<Card> cards);

	Card getTheLastCardOfQue(int queIdx);

	int tryToCollectTheCard(Card card);                    //尝试收取这张牌，能收则返回队列下标，不能则返回-1

	vector<Card> getTheCardsCanCollect();

protected:
	CardQueue m_queues[Collect_Area_Num];
};


#endif
