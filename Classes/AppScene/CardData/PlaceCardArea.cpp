#include "PlaceCardArea.h"
#include "TipManager.h"

void PlaceCardArea::initQueue(int queIdx, CardQueue que)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	m_queues[queIdx].init(que);
}

void PlaceCardArea::clear()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i].clear();
	}
}

void PlaceCardArea::stTest_print()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i].stTest_print();
	}
}

PlaceCardQueue PlaceCardArea::operator [](int index)
{
	if (index < 0 || index >= Place_Area_Num)
	{
		PlaceCardQueue re;
		return re;
	}
	return m_queues[index];
}

string PlaceCardArea::bakAllCards()
{
	char buf[64];
	string re = "";
	for (int i = 0; i < Place_Area_Num; i++)
	{
		int sz = m_queues[i].size();
		sprintf(buf, "    \"place_%d\": [\n", i + 1);
		re += buf;
		string queStr = m_queues[i].convertToString(true);
		re += queStr;
		if (i == (Place_Area_Num - 1))
		{
			sprintf(buf, "    ]\n");
		}
		else
		{
			sprintf(buf, "    ],\n");
		}
		re += buf;
	}
	return re;
}

void PlaceCardArea::setQue(int idx, CardQueue que)
{
	ST_RETURN_IF(idx < 0 || idx >= Place_Area_Num);
	m_queues[idx] = que;
}

void PlaceCardArea::addCardsAt(int queIdx, vector<Card> cards)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	m_queues[queIdx].pushBack(cards);
}

void PlaceCardArea::openAllLastCard()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i].openTheLastCard();
	}
}

int PlaceCardArea::tryToPlaceTheCard(Card card)
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		if (m_queues[i].canPlaceTheCard(card))
		{
			return i;
		}
	}
	return -1;
}


vector<Card> PlaceCardArea::getAllOpenCardsAt(int queIdx)
{
	vector<Card> re;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return re;
	}
	return m_queues[queIdx].getAllOpenCards();
}

vector<Card> PlaceCardArea::getCardsAt(int queIdx, int cardIdx)
{
	vector<Card> re;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return re;
	}
	return m_queues[queIdx].getCardsAt(cardIdx);
}

vector<Card> PlaceCardArea::removeCardsAt(int queIdx, int cardIdx)
{
	vector<Card> reTem;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return reTem;
	}
	vector<Card> re = m_queues[queIdx].removeCardsAfterIndex(cardIdx);
	/*vector<Card> tempCards;
	int sz = re.size();
	for (int i = 0; i < sz; i++)
	{
		tempCards.push_back(re[i]);
	}
	re.clear();

	for (int i = sz - 1; i >= 0; i--)
	{
		re.push_back(tempCards[i]);
	}*/
	return re;
}

int PlaceCardArea::searchTheCardAt(Card card, int queIdx)
{
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return -1;
	}
	return m_queues[queIdx].searchTheCard(card);
}

int PlaceCardArea::canPlaceTheCardFromCollectArea(Card card)
{
	int placeIdx = tryToPlaceTheCard(card);
	if (placeIdx != -1)
	{
		int idx = theFirstOpenCardOfQueCanBehindTheCard(card);
		placeIdx = (idx == -1) ? -1 : placeIdx;
	}
	return placeIdx;
}

int PlaceCardArea::theFirstOpenCardOfQueCanBehindTheCard(Card card)
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		Card lastCd = m_queues[i].getTheFirstOpenCard();
		if (card.canPlaceTheCard(lastCd))
		{
			return i;
		}
	}
	return -1;
}

string PlaceCardArea::convertToInfoStr()
{
	string re = "\"";
	for (int i = 0; i < Place_Area_Num; i++)
	{
		string infoStr = m_queues[i].convertToInfoStr();
		re += infoStr;
		re += "|";
	}
	re += "\",\n";
	return re;
}