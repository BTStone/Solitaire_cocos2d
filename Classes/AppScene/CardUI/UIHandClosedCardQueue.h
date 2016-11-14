#ifndef __ST_SOLITAIRE_UI_HAND_CLOSED_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_HAND_CLOSED_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UIHandClosedCardQueue : public UICardQueue
{
public:
	static UIHandClosedCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                               //����λ��
	void resetCardBg();                                     //���ÿ�λʱ�ı���ͼƬ
	TransportCardQueue* search(Vec2 touchPos);              //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	bool containThePos(Vec2 worldPos);
	void addCard(Card card);
	void addCards(CardQueue queData);
	void addTip();
	void removeTip();
protected:
	Sprite *m_tip;
};

#endif