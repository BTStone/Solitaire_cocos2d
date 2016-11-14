#include "CardQueue.h"
#include "IOManager.h"

void CardQueue::stTest_print()
{
	string str = "";
	for (int i = 0; i < (int)m_cards.size(); i++)
	{
		Card card = m_cards[i];
		int cardToInt = card.stTest_convertToInt();
		char buf[4];
		sprintf(buf, "%d ", cardToInt);
		str += buf;
	}
}

CardQueue::CardQueue()
{

}
CardQueue::CardQueue(vector<Card> cards)
{
	m_cards.clear();
	int sz = cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards.push_back(cards[i]);
	}
}

void CardQueue::init(CardQueue que)
{
	(*this) = que;
}

Card CardQueue::removeTheLastCard()
{
	int sz = m_cards.size();
	if (sz <= 0){ return INVALID_CARD;}
	Card re = m_cards[sz - 1];
	m_cards.pop_back();
	//m_cards.erase(m_cards.begin() + sz - 1);
	return re;
}


vector<Card> CardQueue::removeCardsAfterIndex(int index)
{
	vector<Card> reCards;
	int sz = m_cards.size();
	while (sz > index)
	{
		Card lastCard = m_cards[sz - 1];
		reCards.insert(reCards.begin(), lastCard);
		m_cards.erase(m_cards.begin() + sz - 1);
		sz = m_cards.size();
	}
	return reCards;
}

vector<Card> CardQueue::removeLastCardByNum(int num)
{
	vector<Card> cards;
	for (int i = 0; i < num; i++)
	{
		if (m_cards.size() > 0)
		{
			cards.push_back(m_cards[m_cards.size() - 1]);
			m_cards.pop_back();
		}
	}
	return cards;
}

void CardQueue::pushBack(Card card)
{
	m_cards.push_back(card);
}

void CardQueue::pushBack(vector<Card> cards)
{
	int len = cards.size();
	for (int i = 0; i < len; i++)
	{
		Card card = cards[i];
		m_cards.push_back(card);
	}
}

void CardQueue::pushBack(CardQueue queData)
{
	int len = queData.size();
	for (int i = 0; i < len; i++)
	{
		Card card = queData[i];
		m_cards.push_back(card);
	}
}

Card CardQueue::operator[](int index)
{
	if (index < 0 || index >= (int)(m_cards.size()))
	{
		return INVALID_CARD;
	}
	return m_cards[index];
}

CardQueue& CardQueue::operator=(CardQueue &source)
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

int CardQueue::size()
{
	return (int)m_cards.size();
}


void CardQueue::clear()
{
	m_cards.clear();
}

Card CardQueue::getTheLastCard()
{
	int sz = m_cards.size();
	//log("CardQueue::getTheLastCard,sz:%d", sz);
	if (sz <= 0)
	{
		return INVALID_CARD;
	}
	return m_cards[sz-1];
}

bool CardQueue::isEmpty()
{
	return (m_cards.size()==0);
}

bool CardQueue::canCollectTheCard(Card card)
{
	ST_RETURN_FALSE_IF(card.isInvalidCard());
	Card lastCard = getTheLastCard();

	//集合里没有牌和有牌这两种情况
	if ((lastCard.isInvalidCard() && card.m_number == 1) ||
		(lastCard.m_color == card.m_color && (lastCard.m_number + 1) == card.m_number))
	{
		return true;
	}
	return false;
}

void CardQueue::collectTheCard(Card card)
{
	pushBack(card);
}

bool CardQueue::canPlaceTheCard(Card card)
{
	Card lastCard = getTheLastCard();
	log("CardQueue::canPlaceTheCard,line1");
	//集合里没有牌和有牌这两种情况
	if ((lastCard.isInvalidCard() && card.m_number == 13) ||
		(lastCard.m_isOpen && (!lastCard.isColorAlike(card)) && (lastCard.m_number - 1) == card.m_number))
	{
		log("CardQueue::canPlaceTheCard,line2");
		return true;
	}
	log("CardQueue::canPlaceTheCard,line3");
	return false;
}

void CardQueue::openTheLastCard()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	m_cards[sz - 1].m_isOpen = true;
}


void CardQueue::closeTheLastCard()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	m_cards[sz - 1].m_isOpen = false;
}


void CardQueue::closeAllCard()
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards[i].m_isOpen = false;
	}
}

void CardQueue::openAllCard()
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards[i].m_isOpen = true;
	}
}

void CardQueue::reverse()
{
	vector<Card> tempCards;
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		tempCards.push_back(m_cards[i]);
	}
	m_cards.clear();

	for (int i = sz - 1; i >= 0; i--)
	{
		m_cards.push_back(tempCards[i]);
	}
}

string CardQueue::convertToString(bool openLast)
{
	string re = "";
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		Card cd = m_cards[i];
		if (openLast && i == (sz - 1))
		{
			cd.m_isOpen = true;
		}
		string cdStr = IOManager::share()->convertCardToString(cd);
		if (i != (sz - 1))
		{
			cdStr += ",\n";
		}
		else
		{
			cdStr += "\n";
		}
		re += cdStr;
	}
	return re;
}

string CardQueue::convertToInfoStr()
{
	string re = "";
	char buf[64];
	int len = m_cards.size();
	for (int i = 0; i < len; i++)
	{
		Card cd = m_cards[i];
		int num = cd.m_number;
		int color = cd.m_color;
		sprintf(buf, "%d-%d ", color, num);
		re += buf;
	}
	return re;
}