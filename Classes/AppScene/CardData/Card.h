#ifndef __ST_SOLITAIRE_CARD_H__
#define __ST_SOLITAIRE_CARD_H__

#include "STBase.h"
enum CardColor
{
	BLACK_CARD,              //黑桃
	RED_CARD,                //红桃
	FLOWER_CARD,             //梅花
	SQUARE_CARD              //方块
};

     

class Card
{
public:
	Card();
	Card(int num, CardColor color, bool isOpen);
	bool operator==(const Card &card);
	Card& operator=(Card card);
	bool isInvalidCard();                     //判断是否是无效牌num==0
	
	bool isColorSame(Card card);              //判断一张牌的花色是否与本牌花色完全相同，即红桃==红桃，方块==方块，黑桃==黑桃，梅花==梅花
	bool isColorAlike(Card card);             //判断一张牌的花色是否与本花色相像，如红桃==（红桃，方块）等。。。
	bool canPlaceTheCard(Card card);          //可以放置这张牌

	int stTest_convertToInt();

	int m_number;                             //纸牌的大小1~13,代表着A~K,0代表着无效纸牌，用于返回无效
	CardColor m_color;                        //纸牌的花色
	bool m_isOpen;                            //这张牌是否打开
};

const Card INVALID_CARD = Card(0, BLACK_CARD, false);

#endif
