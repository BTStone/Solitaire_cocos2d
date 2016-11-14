#ifndef __ST_SOLITAIRE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_CARD_QUEUE_H__

#include "Card.h"
#include "STBase.h"
#include <vector>

using namespace std;

class CardQueue
{
public:
	CardQueue();
	CardQueue(vector<Card> cards);
	void init(CardQueue que);
	Card removeTheLastCard();                                //ɾ�����һ��Ԫ��
	vector<Card> removeCardsAfterIndex(int index);           //ɾ��ĳ��Ԫ�أ��������Ԫ�أ�������Ԫ��
	vector<Card> removeLastCardByNum(int num);               //�Ӷ���ĩβɾ��num��������
	void pushBack(Card card);                                //��������һ��Ԫ��
	void pushBack(vector<Card> cards);                       //��������һ����Ԫ��
	void pushBack(CardQueue queData);                        //��������һ����Ԫ��
	Card operator [](int index);                             //����[]�����
	CardQueue& operator=(CardQueue &source);                 //����=�����

	int size();                                              //m_cards�ĳ���
	void clear();                                            //���
	Card getTheLastCard();                                   //��ȡ���һ����
	bool isEmpty();                                          //�ж϶������Ƿ�Ϊ��

	bool canCollectTheCard(Card card);                       //�ж��Ƿ�������ƣ����ƹ���ͬɫ&&�����һ���ƴ���һ
	void collectTheCard(Card card);                          //ֱ����ȡ��

	bool canPlaceTheCard(Card card);                         //�ж��Ƿ��ܹ����������ƣ���������
	void openTheLastCard();                                  //�����һ��������Ϊ��״̬
	void closeTheLastCard();                                 //�����һ��������Ϊ�ر�״̬
	void closeAllCard();                                     //�����е�Card�Ĵ�״̬����Ϊfalse
	void openAllCard();                                      //�����е�Card�Ĵ�״̬����Ϊtrue

	void reverse();                                          //������
	string convertToString(bool openLast = false);           //�����������Ƶ���Ϣ���һ���ַ����ﲢ����,��ʽ2��3��45��������
	string convertToInfoStr();                               //�����������Ƶ���Ϣ���һ���ַ����ﲢ����,��ʽ0-1��2-11��
	void stTest_print();
protected:
	vector<Card> m_cards;
};

const CardQueue EMPTY_CARD_QUEUE;
#endif
