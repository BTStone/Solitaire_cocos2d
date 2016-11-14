#ifndef __ST_SOLITAIRE_PLACE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_PLACE_CARD_QUEUE_H__

#include "CardQueue.h"

class PlaceCardQueue : public CardQueue
{
public:
	Card getTheFirstOpenCard();                                   //��ȡ���Ϸ�����
	int getTheFirstOpenCardIndex();                               //��ȡ���Ϸ����Ƶ��±�
	bool canPlaceTheCard(Card card);                              //�Ƿ���Է�����
	void placeTheCard(Card card);                                 //ֱ�ӷ�����һ����
	void placeTheCard(vector<Card> cards);                        //ֱ�ӷ�����һ������
	vector<Card> getAllOpenCards();                               //��ȡ���д򿪵���
	vector<Card> getCardsAt(int cardIdx);                         //��ȡĳһ�±���ֽ�ƶ���
	PlaceCardQueue& operator=(CardQueue &source);                 //����=�����
	int searchTheCard(Card card);                                 //�ڶ���������card�������򷵻��±꣬���򷵻�-1
};

#endif
