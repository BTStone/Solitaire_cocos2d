#ifndef __ST_SOLITAIRE_COLLECT_CARD_AREA_H__
#define __ST_SOLITAIRE_COLLECT_CARD_AREA_H__

#include "CardQueue.h"
#include "STBase.h"

class CollectCardArea
{
public:
	void initQueue(int queIdx, CardQueue que);             //��ʼ��ĳ������
	CardQueue operator[](int index);
	string bakAllCards();                                  //���������ƣ�д���ļ�(���ؽ�Ҫ���ݵ��ַ�����
	void stTest_print();
	void clear();
	void setQue(int idx, CardQueue que);
	void addCardsAt(int queIdx, vector<Card> cards);

	Card getTheLastCardOfQue(int queIdx);

	int tryToCollectTheCard(Card card);                    //������ȡ�����ƣ������򷵻ض����±꣬�����򷵻�-1

	vector<Card> getTheCardsCanCollect();

protected:
	CardQueue m_queues[Collect_Area_Num];
};


#endif
