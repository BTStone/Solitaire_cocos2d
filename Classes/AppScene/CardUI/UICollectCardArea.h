#ifndef __ST_SOLITAIRE_UI_COLLECT_CARD_AREA_H__
#define __ST_SOLITAIRE_UI_COLLECT_CARD_AREA_H__


#include "UICollectCardQueue.h"
#include "CollectCardArea.h"
#include "TransportCardQueue.h"

class UICollectCardArea : public Node
{
public:
	static UICollectCardArea* create(CollectCardArea areaData);
	virtual bool init(CollectCardArea areaData);
	void initQueues();
	void setLandscapeStatus();
	void setPortraitStatus();
	int isContain(Vec2 worldPos);                                 //�˵��Ƿ�����������,���򷵻ض����±꣬���򷵻�-1
	Card getTheNextCollectCard();                                 //��ȡ��һ��������ȡ���Ƶ���Ϣ
	vector<Card> getTheCardsCanCollect();
	TransportCardQueue* search(Vec2 touchPos);                    //���ݵ����λ�ò��ҵ�����Ķ��󣬼��뵽������У�����ԭ���Ķ�����ɾ��
	int getQueSize(int queIdx);
	int getAllCardSize();
	int getQueIndexByPos(Vec2 pos);                               //���ݵ�ȷ���˵����ڵ��е��±�
	int canCollectTheCard(TransportCardQueue *que);               //�����ж��Ƿ�����ȡ����ƣ����򷵻����ڶ��е��±꣬�����򷵻�-1
	int canCollectTheCard(Card card);
	bool canCollectTheCard(TransportCardQueue *que, int queIdx);  //�Ƿ�����ȡ�����
	void collectTheCard(TransportCardQueue *que, int queIdx);     //ֱ����ȡ������
	Vec2 getNextPosition(int queIdx);                             //������Ҫ���õ���������

	void addCardsAtIndex(CardQueue queData, int queIdx, bool withEffect = true);          //��ĳһ���������Ԫ��
	void clear();
	void updateCard();
	void syncData();                                              //ͬ�����ݣ���m_areaDate������е�����ͬ����
	string bakAllCards();                                         //���������ƣ�д���ļ�(���ؽ�Ҫ���ݵ��ַ�����
	bool isEmpty();

	CardSprite* getTheLastCard(int queIdx);

	CC_SYNTHESIZE_READONLY(CollectCardArea, m_areaData, AreaData);
	vector<Card> removeTheLastNumCardsOfQue(int queIdx, int num);
	CollectCardArea* getData()
	{
		return &m_areaData;
	}
protected:
	UICollectCardQueue *m_queues[Collect_Area_Num];
};


#endif
