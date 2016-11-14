#ifndef __ST_SOLITAIRE_UI_COLLECT_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_COLLECT_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

class UICollectCardQueue : public UICardQueue
{
public:
	static UICollectCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	void initCardsPosition();                                     //����λ��
	TransportCardQueue* search(Vec2 touchPos);                    //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	bool containThePos(Vec2 worldPos);                            //�˵��Ƿ������������
	bool intersectsTheCard(Vec2 worldCardPos);

	bool canCollectTheCard(Card card);                            //�ж��Ƿ������ȡ������
	void collectTheCard(Card card);                               //ֱ����ȡ
	Vec2 getNextPosition();                                       //������Ҫ���õ���������

	void addCards(CardQueue queData, bool withEffect = true);

	void addCollectCardEffect(CardColor color);                   //������Ч
};

#endif