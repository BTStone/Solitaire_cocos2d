#ifndef __ST_SOLITAIRE_UI_HAND_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_HAND_CARD_AREA_H__


#include "UIHandClosedCardQueue.h"
#include "UIHandOpenCardQueue.h"
#include "HandCardArea.h"
#include "TransportCardQueue.h"

class UIHandCardArea : public Node
{
public:
	static UIHandCardArea* create(HandCardArea areaData);
	virtual bool init(HandCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	Vec2 getClosedAreaPosition();                                  //��ȡδ���������������
	Vec2 getOpenAreaLastCardPosition();                            //��ȡ�������һ��Ԫ�ص�����
	Vec2 getNextPositionOfOpenQue();                               //��ȡ������һ���Ƶķ���λ��
	Vec2 getNextPositionOfClosedQue();                             //��ȡδ������һ���Ƶķ���λ��

	TransportCardQueue* search(Vec2 touchPos);
	bool inColosedArea(Vec2 worldPos);

	void addCardsAtOpenArea(CardQueue queData);                    //�ڴ������ֽ��
	void addCardsAtClosedArea(CardQueue queData);                  //�ڷǴ������ֽ��

	void flipCard();                                               //����
	void shuffleCard();                                            //ϴ��
	void openAreaRemoveCardEffectWithNum(int remNum);              //����������Ƶ�ʱ�������ƶ�λ��
	void openAreaAddCardEffectWithNum(int addNum);                 //�����������Ƶ�ʱ�������ƶ�λ��
	bool isEmpty();                                                //�򿪺͹ر����Ƿ�Ϊ��
	void updateCard();

	int getClosedAreaSize();
	int getOpenAreaSize();

	CardSprite* getOpenAreaLastCard();

	CC_SYNTHESIZE_READONLY(HandCardArea, m_areaData, AreaData);
	CC_SYNTHESIZE_READONLY(Size, m_areaSize, AreaSize);
	void addClosedAreaTip();                                     //������ʾ
	void addOpenAreaTip();                                       //������ʾ
	vector<Card> removeTheLastNumCardsOfOpenQue(int num);
	vector<Card> removeTheLastNumCardsOfClosedQue(int num);

	bool canAutoCollect();                                       //�ܷ��Զ�����
	
	HandCardArea* getData()
	{
		return &m_areaData;
	}
	
	void syncData();                                              //ͬ�����ݣ���m_areaDate������е�����ͬ����
	string bakAllCards();                                          //���������ƣ�д���ļ�(���ؽ�Ҫ���ݵ��ַ�����

	void setHandType(bool isLeft);
	void setLeftHandType();
	void setRightHandType();

protected:
	void transportArrivedCallFunc(Node *node);                     //�����ൽ��Ŀ�ĵ�

protected:
	UIHandClosedCardQueue *m_closedQueue;
	UIHandOpendCardQueue *m_openQueue;
};


#endif
