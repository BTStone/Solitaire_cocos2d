#include "CollectCardArea.h"
#include "IOManager.h"

void CollectCardArea::initQueue(int queIdx, CardQueue que)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	m_queues[queIdx].init(que);
}

void CollectCardArea::stTest_print()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		m_queues[i].stTest_print();
	}
}

CardQueue CollectCardArea::operator[](int index)
{
	if (index < 0 || index >= Collect_Area_Num)
	{
		CardQueue re;
		return re;
	}
	return m_queues[index];
}

string CollectCardArea::bakAllCards()
{
	char buf[64];
	string re = "";
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		sprintf(buf, "    \"collect_%d\": [\n", i + 1);
		re += buf;
		string queStr = m_queues[i].convertToString();
		re += queStr;
		sprintf(buf, "    ],\n");
		re += buf;
	}
	return re;
}

void CollectCardArea::clear()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		m_queues[i].clear();
	}
}

void CollectCardArea::setQue(int idx, CardQueue que)
{
	ST_RETURN_IF(idx < 0 || idx >= Collect_Area_Num);
	m_queues[idx] = que;
}

void CollectCardArea::addCardsAt(int queIdx, vector<Card> cards)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	m_queues[queIdx].pushBack(cards);
}

int CollectCardArea::tryToCollectTheCard(Card card)
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		if (m_queues[i].canCollectTheCard(card))
		{
			return i;
		}
	}
	return -1;
}


vector<Card> CollectCardArea::getTheCardsCanCollect()
{
	vector<Card> cards;
	//添加4个A
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		Card cd;
		cd.m_number = 1;
		cd.m_color = (CardColor)i;
		cards.push_back(cd);
	}

	for (int i = 0; i < Collect_Area_Num; i++)
	{
		bool isEmp = m_queues[i].isEmpty();
		if (!isEmp)
		{
			Card lastCd = m_queues[i].getTheLastCard();
			int sz = cards.size();
			//删除已有花色A
			for (int i = 0; i < sz; i++)
			{
				Card c;
				c.m_number = 1;
				c.m_color = lastCd.m_color;
				if (cards[i] == c)
				{
					cards.erase(cards.begin() + i);
					break;
				}
			}

			//添加下一张牌
			if (lastCd.m_number > 0 && lastCd.m_number < 14)
			{
				cards.push_back(Card(lastCd.m_number+1, lastCd.m_color, true));
			}
			
		}
	}
	return cards;
}

Card CollectCardArea::getTheLastCardOfQue(int queIdx)
{
	if (queIdx < 0 || queIdx >= Collect_Area_Num)
	{
		return INVALID_CARD;
	}
	return m_queues[queIdx].getTheLastCard();
}