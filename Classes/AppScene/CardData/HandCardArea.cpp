#include "HandCardArea.h"

void HandCardArea::init(CardQueue closeQue, CardQueue openQue)
{
	m_closedQueue.init(closeQue);
	m_openQueue.init(openQue);
}

void HandCardArea::clear()
{
	m_openQueue.clear();
	m_closedQueue.clear();
}

void HandCardArea::stTest_print()
{
	m_closedQueue.stTest_print();
	m_openQueue.stTest_print();
}


string HandCardArea::bakAllCards()
{
	string re = "";
	char buf[64];

	//closedÇøÓò
	sprintf(buf, "    \"hand_closed\": [\n");
	re += buf;
	string closedStr = m_closedQueue.convertToString();
	re += closedStr;
	sprintf(buf, "    ],\n");
	re += buf;

	//openÇøÓò
	sprintf(buf, "    \"hand_open\": [\n");
	re += buf;
	string openStr = m_openQueue.convertToString();
	re += openStr;
	sprintf(buf, "    ],\n");
	re += buf;
	
	return re;
}


Card HandCardArea::getTheLastCardOfOpenQue()
{
	return m_openQueue.getTheLastCard();
}

Card HandCardArea::removeTheLastCardOfOpenQue()
{
	return m_openQueue.removeTheLastCard();
}

Card HandCardArea::getTheLastCardOfClosedQue()
{
	return m_closedQueue.getTheLastCard();
}

bool HandCardArea::canFlip()
{
	return !m_closedQueue.isEmpty();
}

bool HandCardArea::flipCard()
{
	//int flipNum = 1;
	const char *strIsDraw3 = "st_is_draw_3";
	bool isDraw3 = UserDefault::getInstance()->getBoolForKey(strIsDraw3, false);
	int flipNum = 1;
	if (isDraw3)
	{
		flipNum = 3;
	}
	if (canFlip())
	{
		vector<Card> cards = m_closedQueue.removeLastCardByNum(flipNum);
		int sz = cards.size();
		for (int i = 0; i < sz; i++)
		{
			cards[i].m_isOpen = true;
		}
		m_openQueue.pushBack(cards);
		return true;
	}
	shuffle();
	return false;
}

bool HandCardArea::canShuffle()
{
	bool b1 = m_closedQueue.isEmpty();
	bool b2 = m_openQueue.isEmpty();
	return ((b1) && (!b2));
}

void HandCardArea::shuffle()
{
	ST_RETURN_IF(!canShuffle());
	m_openQueue.reverse();
	m_openQueue.closeAllCard();
	m_closedQueue.pushBack(m_openQueue);
	m_openQueue.clear();
}