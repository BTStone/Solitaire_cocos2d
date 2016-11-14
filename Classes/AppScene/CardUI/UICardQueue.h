#ifndef __ST_SOLITAIRE_UI_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_CARD_QUEUE_H__

#include "CardSprite.h"
#include "CardQueue.h"

enum CardArea
{
	COLLECT_AREA,              //������
	HAND_CLOSE_AREA,           //���Ʋ��ɼ���
	HAND_OPEN_AREA,            //���ƿɼ���
	PLACE_AREA                 //������
};

class UICardQueue : public Node
{
public:
	static UICardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCards();
	virtual void initCardBg();
	Card removeTheLastCard();                                       //ɾ�����һ��Ԫ��
	vector<Card> removeCardsAfterIndex(int index);                  //ɾ��ĳ��Ԫ�أ��������Ԫ�أ�������Ԫ��
	vector<Card> removeLastCardByNum(int num);                      //�Ӷ���ĩβɾ��num��������
	void removeAllCards();                                          //ɾ��������
	CardSprite* operator [](int index);                             //����[]�����
	CardSprite* getTheLastCard();                                   //��ȡ���һ����
	CardSprite* getTheFirstCard();                                  //��ȡ��һ����
	void addTipLightAfterIndex(int idx);                            //�����ʾ��Ȧ
	void removeTipLight();
	void removeSelf(Node *node);
	//virtual TransportCardQueue* search(Vec2 touchPos) = 0;          //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��

	int size();                                                     //m_cards�ĳ���
	void clear();                                                   //���
	bool isEmpty();                                                 //�ж϶������Ƿ�Ϊ��
	bool isAllCardsOpen();
	Vec2 getWorldPosOfCardBg();                                     //��ȡ��һ��λ�ã�cardBg��λ�ã�����������
	void updateCard();

	CC_SYNTHESIZE(CardQueue, m_queData, QueData);                   //���ݶ���
	CC_SYNTHESIZE(CardArea, m_resArea, ResArea);                    //��ǰ��������

protected:                                          
	vector<CardSprite*> m_cards;                                    //UI����
	Sprite *m_cardBg;                                               //ÿһ���пյ�ʱ��ı���
	Node*m_tipLight;
};

#endif
