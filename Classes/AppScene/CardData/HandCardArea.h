#ifndef __ST_SOLITAIRE_HAND_CARD_AREA_H__
#define __ST_SOLITAIRE_HAND_CARD_AREA_H__

#include "CardQueue.h"

class HandCardArea
{
public:
	void init(CardQueue closeQue, CardQueue openQue = EMPTY_CARD_QUEUE);
	void clear();
	string bakAllCards();
	void stTest_print();
	Card removeTheLastCardOfOpenQue();
	Card getTheLastCardOfClosedQue();
	Card getTheLastCardOfOpenQue();
	bool canFlip();
	bool flipCard();                   //·­ÅÆ·µ»Øfalse£¬ Ï´ÅÆ·µ»Øtrue
	bool canShuffle();
	void shuffle();
	CC_SYNTHESIZE(CardQueue, m_closedQueue, ClosedQueue);
	CC_SYNTHESIZE(CardQueue, m_openQueue, OpenQueue);
protected:
};

#endif
