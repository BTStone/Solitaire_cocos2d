#ifndef __ST_SOLITAIRE_UI_PLACE_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_PLACE_CARD_AREA_H__


#include "UIPlaceCardQueue.h"
#include "PlaceCardArea.h"
#include "TransportCardQueue.h"

class UIPlaceCardArea : public Node
{
public:
	static UIPlaceCardArea* create(PlaceCardArea areaData);
	virtual bool init(PlaceCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	int getQueSize(int queIdx);
	TransportCardQueue* search(Vec2 worldPos);                    //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	int searchTheCard(Card card);                                 //���ش������ڶ����±꣬û���򷵻�-1
	int getQueIndexByPos(Vec2 worldPos);                          //���ݵ�ȷ���˵����ڵ��е��±�
	int canPlaceTheCard(TransportCardQueue *que);                 //�����ж��Ƿ�����ȡ����ƣ����򷵻����ڶ��е��±꣬�����򷵻�-1
	bool canPlaceTheCard(TransportCardQueue *que, int queIdx);    //�Ƿ��ܷ��������
	Vec2 getNextPosition(int queIdx);                             //������Ҫ���õ���������

	void addCardsAtIndex(CardQueue queData, int queIdx);          //��ĳһ���������Ԫ��

	bool tryToOpenTheLastCardOfTheQue(int queIdx);                //���Դ�ĳһ�е����һ����
	bool tryToCloseTheLastCardOfTheQue(int queIdx);               //���Թر�ĳһ�е����һ����
	bool isInTheLastCardOfTheQue(int queIdx, Vec2 worldPos);      //�Ƿ�����ĳһ�е����һ�����ϣ������϶�ʱ

	bool isMoveEffectly(int resIdx, int destIdx);                 //������֮����ƶ��Ƿ�����Ч�ƶ���ѭ���ƶ���

	Vec2 getCardWorldPosition(int row, int cow);                  //�����У��У���0��ʼ����ȡ����
	vector<Vec2> getAllCardsWorldPosition();                      //��ȡ�����Ƶ���������
	void openLastCards();                                         //�����ж��е����һ��
	void updateCard();
	Vec2 getTheLastCardPosOfTheQue(int queIdx);
	void syncData();                                              //ͬ�����ݣ���m_areaDate������е�����ͬ����
	string bakAllCards();                                         //���������ƣ�д���ļ�(���ؽ�Ҫ���ݵ��ַ�����
	bool isEmpty();
	bool isAllCardsOpen();
	vector<Card> getCardsAt(int queIdx, int cardIdx);             //��ȡĳһ����ĳһ�±�֮��������Ƶ���Ϣ
	CC_SYNTHESIZE_READONLY(PlaceCardArea, m_areaData, AreaData);
	void addTipLightAt(int queIdx, int cardIdx);
	void removeAllTipLight();
	vector<Card> removeTheLastNumCardsOfQue(int queIdx, int num);

	void tryToScale();
	bool tryToScaleTheQue(int queIdx);
	PlaceCardArea* getData()
	{
		return &m_areaData;
	}
	void stTest_scale();
protected:
	UIPlaceCardQueue *m_queues[Place_Area_Num]; 
	float m_emptyOffX;            //���ÿ��X
};


#endif
