#ifndef __ST_SOLITAIRE_UI_HAND_OPEN_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_HAND_OPEN_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UIHandOpendCardQueue : public UICardQueue
{
public:
	static UIHandOpendCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                              //����λ��
	void resetCardBg();                                    //���ÿ�λʱ�ı���ͼƬ
	TransportCardQueue* search(Vec2 touchPos);             //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	void addCard(Card card);
	void addCards(CardQueue queData);
	void addCardEffectWithNum(int addNum);                 //�����Ԫ��ʱ���������Ԫ�صĸ�����������Ӧ���ƶ�
	void removeCardEffectWithNum(int remNum);              //��ɾ��Ԫ��ʱ���������Ԫ�صĸ�����������Ӧ���ƶ�

	void lastTwoCardsMoveByOnePosToLeft();                 //�������Ԫ�������ƶ�һ��λ��
	void lastTwoCardsMoveToMostLeft();                     //�������Ԫ�ض��ƶ�������ߵ�λ��
	void lastTwoCardsMoveByOnePosToRight();                //�������Ԫ�������ƶ�һ��λ��
	void lastTwoCardsMoveToRightIrregular();               //�������Ԫ�����Ҳ������ƶ�

	void addTip();
	void removeTip();

	bool isOnlyOneCard();
protected:
	Sprite *m_tip;
};

#endif