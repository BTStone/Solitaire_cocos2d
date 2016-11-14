#include "PlaceCardQueue.h"

bool PlaceCardQueue::canPlaceTheCard(Card card)
{
	ST_RETURN_FALSE_IF(card.isInvalidCard());
	Card lastCard = getTheLastCard();
	if ((lastCard.isInvalidCard() && card.m_number==13) ||
		(lastCard.m_isOpen && (lastCard.m_number == (card.m_number + 1)) && !lastCard.isColorAlike(card)))
	{
		return true;
	}
	return false;
}

int PlaceCardQueue::getTheFirstOpenCardIndex()
{
	for (int i = 0; i < (int)m_cards.size(); i++)
	{
		if (m_cards[i].m_isOpen)
		{
			return i;
		}
	}
	return -1;
}

Card PlaceCardQueue::getTheFirstOpenCard()
{
	for (int i = 0; i < (int)m_cards.size(); i++)
	{
		if (m_cards[i].m_isOpen)
		{
			return m_cards[i];
		}
	}
	return INVALID_CARD;
}


void PlaceCardQueue::placeTheCard(Card card)
{
	pushBack(card);
}
void PlaceCardQueue::placeTheCard(vector<Card> cards)
{
	pushBack(cards);
}


PlaceCardQueue& PlaceCardQueue::operator=(CardQueue &source)
{
	m_cards.clear();
	int len = source.size();
	for (int i = 0; i < len; i++)
	{
		Card card = source[i];
		m_cards.push_back(card);
	}
	return (*this);
}

vector<Card> PlaceCardQueue::getAllOpenCards()
{
	vector<Card> re;
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		Card card = m_cards[i];
		if (card.m_isOpen)
		{
			re.push_back(card);
		}
	}
	return re;
}

vector<Card> PlaceCardQueue::getCardsAt(int cardIdx)
{
	vector<Card> re;
	int sz = m_cards.size();
	if (cardIdx < 0 || cardIdx >= sz){ return re; }
	for (int i = cardIdx; i < sz; i++)
	{
		Card card = m_cards[i];
		re.push_back(card);
	}
	return re;
}

int PlaceCardQueue::searchTheCard(Card card)
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		Card cd = m_cards[i];
		if (cd == card)
		{
			return i;
		}
	}
	return -1;
}
