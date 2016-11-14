#ifndef __ST_SOLITAIRE_UI_PLACE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_PLACE_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

#define CLOSE_CARD_GAP_SCALE 0.3

class UIPlaceCardQueue : public UICardQueue
{
public:
	static UIPlaceCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	TransportCardQueue* search(Vec2 touchPos);                    //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	void initCardsPosition();                                     //����λ��
	void resetCardBg();                                           //���ÿ�λʱ�ı���ͼƬ
	void addCards(CardQueue queData);
	Vec2 getPositionAt(int index);                                //��ȡĳһ�±���ֽ�����ڵ�λ�ã����������꣩
	bool canPlaceTheCard(Card card);                              //�Ƿ��ܹ�����������
	Vec2 getNextPosition();                                       //������Ҫ���õ���������
	bool tryToOpenTheLastCard();                                  //���Դ����һ���ƣ�����Ѿ����򷵻�false�����û�д���򿪣�����true
	bool tryToCloseTheLastCard();                                 //���Թر����һ���ƣ� ����Ѿ��رշ���false�����򷵻�true
	bool isInTheLastCard(Vec2 worldPos);                          //�˵��Ƿ������һ�����ϣ����㣩
	bool intersectsTheCard(Vec2 worldCardPos);                    //�˵��Ƿ������һ�����ϣ����Ӵ���
	CardSprite *getTheSecondToLastCard();                         //��ȡ�����ڶ�����
	CardSprite *at(int idx);
	Vec2 getCardWorldPosition(int idx);
	vector<Card> getCardsAfter(int idx);
	bool containTheCard(Card card);

	int getOpenCardNum();                                         //��ȡ���Ƶĸ���
	float getOverLenth();                                         //��ȡ�����ĳ���
	bool willOverScreen();                                        //�Ƿ�Ҫ������Ļ
	bool alreadyOverScreen();                                     //�Ƿ��Ѿ�������Ļ
	bool tryToScale();                                            //�������ţ�������Ļ��,���������򷵻�true�����򷵻�false
	float getOpenCardMoveByLenth();
	void openCardMoveBy(float mvBy);                              //��״̬�������ţ���������һ����
	void scaleOverCallFunc();
	void scaleWhenOverScreen();                                   //������Ļ������
	bool scaleWhenRemoveCard();
	bool isOriginGapOfOpenCard();                                 //�ж�m_openCardGapY�Ƿ��ǳ�ʼ���cardSize.height * Place_Area_Gap_Y;
protected:
	float m_openCardGapY;                                  //չ����֮��ļ��Y
	float m_originGapY;                                    //չ����֮��ĳ�ʼ���Y
};

#endif
