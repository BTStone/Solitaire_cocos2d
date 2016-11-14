#include "Card.h"

int Card::stTest_convertToInt()
{
	int num = m_number;
	int color = (int)m_color;
	int numPerColor = 13;
	return color * numPerColor + num;
}

Card::Card()
{

}

Card::Card(int num, CardColor color, bool isOpen)
{
	m_number = num;
	m_color = color;
	m_isOpen = isOpen;
}

bool Card::operator==(const Card &card)
{
	if (this->m_number == card.m_number &&
		this->m_color == card.m_color)
	{
		return true;
	}

	return false;
}

Card& Card::operator=(Card card)
{
	m_number = card.m_number;
	m_color = card.m_color;
	m_isOpen = card.m_isOpen;
	return (*this);
}

bool Card::isColorSame(Card card)
{
	return m_color == card.m_color ? true : false;
}

bool Card::isColorAlike(Card card)
{
	bool bl1 = (m_color == BLACK_CARD && (card.m_color == BLACK_CARD || card.m_color == FLOWER_CARD));
	bool bl2 = (m_color == FLOWER_CARD && (card.m_color == BLACK_CARD || card.m_color == FLOWER_CARD));
	bool bl3 = (m_color == RED_CARD && (card.m_color == RED_CARD || card.m_color == SQUARE_CARD));
	bool bl4 = (m_color == SQUARE_CARD && (card.m_color == RED_CARD || card.m_color == SQUARE_CARD));
	if (bl1 || bl2 || bl3 || bl4)
	{
		return true;
	}
	return false;
}

bool Card::canPlaceTheCard(Card card)
{
	bool isAlike = isColorAlike(card);
	ST_RETURN_FALSE_IF(isAlike);
	ST_RETURN_FALSE_IF(card.isInvalidCard());
	int num = card.m_number;
	if (num == (m_number - 1))
	{
		return true;
	}
	return false;
}

bool Card::isInvalidCard()
{
	return (m_number == INVALID_CARD.m_number);
}

