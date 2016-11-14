#ifndef __ST_SOLITAIRE_TIP_MANAGER_H__
#define __ST_SOLITAIRE_TIP_MANAGER_H__

#include "STBase.h"
#include "UICardQueue.h"
#include "CollectCardArea.h"
#include "HandCardArea.h"
#include "PlaceCardArea.h"

struct TipInfo
{
	TipInfo(){}
	TipInfo(CardArea resArea, CardArea desArea)
	{
		m_resArea = resArea;
		m_desArea = desArea;
	}
	~TipInfo(){}

	CardArea m_resArea;       //源区域
	int m_resQueIdx;          //源队列
	int m_resCardIdx;         //源纸牌所在队列下标
	CardArea m_desArea;       //目标区域 
	int m_desQueIdx;          //目标队列
};

const TipInfo Invalid_Tip_Info(COLLECT_AREA, COLLECT_AREA);     //无效提示

class TipManager
{
public:
	static TipManager* create(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData);
	virtual bool init(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData);
	TipInfo newTip();
	static bool isInvalidTipInfo(TipInfo info);

protected:
	TipInfo tryToCollect();                        //尝试收牌
	TipInfo tryToCollectHandArea();                //尝试收取手牌区
	TipInfo tryToCollectPlaceArea();               //尝试收取置牌区

	TipInfo tryToPlace();                          //尝试放置牌
	TipInfo tryToPlaceHandArea();                  //尝试放置手牌区的牌到置牌区
	TipInfo tryToPlacePlaceArea();                 //尝试放置置牌区内部牌
	TipInfo tryToPlaceCollectArea();               //如果从收牌区里取出牌可以翻开置牌区的牌

	TipInfo tryToFlip();                           //尝试翻牌
	TipInfo tryToShuffle();                        //尝试洗牌
protected:
	HandCardArea *m_handAreaData;
	PlaceCardArea *m_placeAreaData;
	CollectCardArea *m_collectAreaData;

};


#endif
