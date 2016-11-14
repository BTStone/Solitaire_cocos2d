#ifndef __ST_SOLITAIRE_CARD_H__
#define __ST_SOLITAIRE_CARD_H__

#include "STBase.h"
enum CardColor
{
	BLACK_CARD,              //����
	RED_CARD,                //����
	FLOWER_CARD,             //÷��
	SQUARE_CARD              //����
};

     

class Card
{
public:
	Card();
	Card(int num, CardColor color, bool isOpen);
	bool operator==(const Card &card);
	Card& operator=(Card card);
	bool isInvalidCard();                     //�ж��Ƿ�����Ч��num==0
	
	bool isColorSame(Card card);              //�ж�һ���ƵĻ�ɫ�Ƿ��뱾�ƻ�ɫ��ȫ��ͬ��������==���ң�����==���飬����==���ң�÷��==÷��
	bool isColorAlike(Card card);             //�ж�һ���ƵĻ�ɫ�Ƿ��뱾��ɫ���������==�����ң����飩�ȡ�����
	bool canPlaceTheCard(Card card);          //���Է���������

	int stTest_convertToInt();

	int m_number;                             //ֽ�ƵĴ�С1~13,������A~K,0��������Чֽ�ƣ����ڷ�����Ч
	CardColor m_color;                        //ֽ�ƵĻ�ɫ
	bool m_isOpen;                            //�������Ƿ��
};

const Card INVALID_CARD = Card(0, BLACK_CARD, false);

#endif
