#ifndef __ST_SOLITAIRE_PLACE_CARD_AREA_H__
#define __ST_SOLITAIRE_PLACE_CARD_AREA_H__

#include "PlaceCardQueue.h"
#include "STBase.h"

struct TipInfo;
class PlaceCardArea
{
public:
	void clear();
	void initQueue(int queIdx, CardQueue que);
	PlaceCardQueue operator[](int index);
	string bakAllCards();
	void setQue(int idx, CardQueue que);
	void addCardsAt(int queIdx, vector<Card> cards);
	void stTest_print();
	vector<Card> getAllOpenCardsAt(int queIdx);              //��ȡ���д򿪵���
	vector<Card> getCardsAt(int queIdx, int cardIdx);        //
	vector<Card> removeCardsAt(int queIdx, int cardIdx);     //��ȡĳһ����ĳһ�±���ֽ�ƶ���
	void openAllLastCard();                                  //��ÿһ�����һ��������Ϊ��
	int tryToPlaceTheCard(Card card);                        //���Է��������ƣ��������򷵻ض����±꣬���򷵻�-1
	int searchTheCardAt(Card card, int queIdx);              //��queIdx������card�������򷵻�card���±꣬�������򷵻�-1
	int canPlaceTheCardFromCollectArea(Card card);           //�Ƿ��ܹ���������ȡ��������
	int theFirstOpenCardOfQueCanBehindTheCard(Card card);         //�߸�����������һ�����Ƿ��ܹ�������card֮��
	string convertToInfoStr();
protected:
	PlaceCardQueue m_queues[Place_Area_Num];
};


#endif
