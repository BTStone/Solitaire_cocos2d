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

	//������ȡ������
	re = tryToCollectPlaceArea();
	if (isInvalidTipInfo(re))
	{
		//������ȡ������
		re = tryToCollectHandArea();
	}
	return re;
}

TipInfo TipManager::tryToCollectHandArea()
{
	TipInfo re;

	//������ȡ������
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
	//������ȡ������
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
	
	//���Է�������������
	re = tryToPlacePlaceArea();
	if (isInvalidTipInfo(re))
	{
		//���Է�������������
		re = tryToPlaceHandArea();
	}

	return re;
}

TipInfo TipManager::tryToPlaceHandArea()
{
	TipInfo re;

	//���Է�������������
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

	//���Է�������������
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

	//���Է�������������
	for (int i = 0; i < Place_Area_Num; i++)
	{
		int queSz = (*m_placeAreaData)[i].size();
		vector<Card> openCards = m_placeAreaData->getAllOpenCardsAt(i);
		int sz = openCards.size();
		for (int j = 0; j < sz; j++)
		{
			Card card = openCards[j];
			bool emptyMoveK = (queSz == sz) && (card.m_number == 13);        //�ƶ�k��λ������Ч��λ
			int desIdx = m_placeAreaData->tryToPlaceTheCard(card);
			if ((j == 0) && (desIdx != -1) && !emptyMoveK)                   //�����е�һ�Ŵ��ƿ��ƶ�
			{
				re.m_resArea = PLACE_AREA;
				re.m_resQueIdx = i;
				re.m_resCardIdx = m_placeAreaData->searchTheCardAt(card, i);
				re.m_desArea = PLACE_AREA;
				re.m_desQueIdx = desIdx;
				return re;
			}
			else if (desIdx != -1)             //�����е�2��3...�Ŵ��ƿ��ƶ�(���ж��Ƿ����ѭ���ƶ�)
			{
				int cardIdx = m_placeAreaData->searchTheCardAt(card, i);
				Card preCard = (*m_placeAreaData)[i][cardIdx - 1];
				int collectIdx = m_collectAreaData->tryToCollectTheCard(preCard);
				if (collectIdx != -1)          //�ƶ��������
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