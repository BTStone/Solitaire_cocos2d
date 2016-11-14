#include "TipManager.h"


TipManager* TipManager::create(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData)
{
	TipManager *re = new TipManager();
	if (re && re->init(collectData, handData, placeData))
	{
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool TipManager::init(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData)
{
	m_handAreaData = handData;
	m_placeAreaData = placeData;
	m_collectAreaData = collectData;

	return true;
}


TipInfo TipManager::newTip()
{
	TipInfo re;
	re = tryToCollect();
	if (isInvalidTipInfo(re))
	{
		re = tryToPlace();
	}
	/*
	if (isInvalidTipInfo(re))
	{
		re = tryToPlaceCollectArea();
	}*/
	if (isInvalidTipInfo(re))
	{
		re = tryToFlip();
	}
	if (isInvalidTipInfo(re))
	{
		re = tryToShuffle();
	}
	return re;
}


TipInfo TipManager::tryToCollect()
{
	TipInfo re;

	//尝试收取置牌区
	re = tryToCollectPlaceArea();
	if (isInvalidTipInfo(re))
	{
		//尝试收取收牌区
		re = tryToCollectHandArea();
	}
	return re;
}

TipInfo TipManager::tryToCollectHandArea()
{
	TipInfo re;

	//尝试收取手牌区
	Card openCard = m_handAreaData->getTheLastCardOfOpenQue();
	int desIdx = m_collectAreaData->tryToCollectTheCard(openCard);
	if (desIdx != -1)
	{
		re.m_resArea = HAND_OPEN_AREA;
		re.m_resQueIdx = 0;
		re.m_desArea = COLLECT_AREA;
		re.m_desQueIdx = desIdx;
		return re;
	}
	return Invalid_Tip_Info;
}

TipInfo TipManager::tryToCollectPlaceArea()
{
	TipInfo re;
	//尝试收取置牌区
	for (int i = 0; i < Place_Area_Num; i++)
	{
		Card card = (*m_placeAreaData)[i].getTheLastCard();
		int sz = (*m_placeAreaData)[i].size();
		int desIdx = m_collectAreaData->tryToCollectTheCard(card);
		if (desIdx != -1)
		{
			re.m_resArea = PLACE_AREA;
			re.m_resQueIdx = i;
			re.m_resCardIdx = sz - 1;
			re.m_desArea = COLLECT_AREA;
			re.m_desQueIdx = desIdx;
			return re;
		}
	}
	return Invalid_Tip_Info;
}

TipInfo TipManager::tryToPlace()
{
	TipInfo re;
	
	//尝试放置置牌区的牌
	re = tryToPlacePlaceArea();
	if (isInvalidTipInfo(re))
	{
		//尝试放置手牌区的牌
		re = tryToPlaceHandArea();
	}

	return re;
}

TipInfo TipManager::tryToPlaceHandArea()
{
	TipInfo re;

	//尝试放置手牌区的牌
	Card openCard = m_handAreaData->getTheLastCardOfOpenQue();
	int desIdx = m_placeAreaData->tryToPlaceTheCard(openCard);
	if (desIdx != -1)
	{
		re.m_resArea = HAND_OPEN_AREA;
		re.m_resQueIdx = 0;
		re.m_desArea = PLACE_AREA;
		re.m_desQueIdx = desIdx;
		return re;
	}
	return Invalid_Tip_Info;
}


TipInfo TipManager::tryToPlaceCollectArea()
{
	TipInfo re;

	//尝试放置手牌区的牌
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		Card lastCd = m_collectAreaData->getTheLastCardOfQue(i);
		int canPlaceIdx = m_placeAreaData->canPlaceTheCardFromCollectArea(lastCd);
		if (canPlaceIdx != -1)
		{
			re.m_resArea = COLLECT_AREA;
			re.m_resQueIdx = i;
			re.m_desArea = PLACE_AREA;
			re.m_desQueIdx = canPlaceIdx;
			return re;
		}
	}
	return Invalid_Tip_Info;
}

TipInfo TipManager::tryToPlacePlaceArea()
{
	TipInfo re;

	//尝试放置置牌区的牌
	for (int i = 0; i < Place_Area_Num; i++)
	{
		int queSz = (*m_placeAreaData)[i].size();
		vector<Card> openCards = m_placeAreaData->getAllOpenCardsAt(i);
		int sz = openCards.size();
		for (int j = 0; j < sz; j++)
		{
			Card card = openCards[j];
			bool emptyMoveK = (queSz == sz) && (card.m_number == 13);        //移动k空位不算有效移位
			int desIdx = m_placeAreaData->tryToPlaceTheCard(card);
			if ((j == 0) && (desIdx != -1) && !emptyMoveK)                   //本队列第一张打开牌可移动
			{
				re.m_resArea = PLACE_AREA;
				re.m_resQueIdx = i;
				re.m_resCardIdx = m_placeAreaData->searchTheCardAt(card, i);
				re.m_desArea = PLACE_AREA;
				re.m_desQueIdx = desIdx;
				return re;
			}
			else if (desIdx != -1)             //本队列第2、3...张打开牌可移动(得判断是否会死循环移动)
			{
				int cardIdx = m_placeAreaData->searchTheCardAt(card, i);
				Card preCard = (*m_placeAreaData)[i][cardIdx - 1];
				int collectIdx = m_collectAreaData->tryToCollectTheCard(preCard);
				if (collectIdx != -1)          //移动后可收牌
				{
					re.m_resArea = PLACE_AREA;
					re.m_resQueIdx = i;
					re.m_resCardIdx = cardIdx;
					re.m_desArea = PLACE_AREA;
					re.m_desQueIdx = desIdx;
					return re;
				}
			}
		}

	}
	return Invalid_Tip_Info;
}

TipInfo TipManager::tryToFlip()
{
	TipInfo re;
	bool canFlip = m_handAreaData->canFlip();
	if (canFlip)
	{
		re.m_resArea = HAND_CLOSE_AREA;
		re.m_desQueIdx = 0;
		re.m_desArea = HAND_OPEN_AREA;
		re.m_desQueIdx = 0;
		return re;
	}
	return Invalid_Tip_Info;
}
TipInfo TipManager::tryToShuffle()
{
	TipInfo re;
	bool canShuffle = m_handAreaData->canShuffle();
	if (canShuffle)
	{
		re.m_resArea = HAND_CLOSE_AREA;
		re.m_desQueIdx = 0;
		re.m_desArea = HAND_OPEN_AREA;
		re.m_desQueIdx = 0;
		return re;
	}
	return Invalid_Tip_Info;
}

bool TipManager::isInvalidTipInfo(TipInfo info)
{
	bool b1 = (info.m_resArea == COLLECT_AREA);
	bool b2 = (info.m_desArea == COLLECT_AREA);
	return (b1 && b2);
}