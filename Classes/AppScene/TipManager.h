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

	CardArea m_resArea;       //Դ����
	int m_resQueIdx;          //Դ����
	int m_resCardIdx;         //Դֽ�����ڶ����±�
	CardArea m_desArea;       //Ŀ������ 
	int m_desQueIdx;          //Ŀ�����
};

const TipInfo Invalid_Tip_Info(COLLECT_AREA, COLLECT_AREA);     //��Ч��ʾ

class TipManager
{
public:
	static TipManager* create(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData);
	virtual bool init(CollectCardArea *collectData, HandCardArea *handData, PlaceCardArea *placeData);
	TipInfo newTip();
	static bool isInvalidTipInfo(TipInfo info);

protected:
	TipInfo tryToCollect();                        //��������
	TipInfo tryToCollectHandArea();                //������ȡ������
	TipInfo tryToCollectPlaceArea();               //������ȡ������

	TipInfo tryToPlace();                          //���Է�����
	TipInfo tryToPlaceHandArea();                  //���Է������������Ƶ�������
	TipInfo tryToPlacePlaceArea();                 //���Է����������ڲ���
	TipInfo tryToPlaceCollectArea();               //�������������ȡ���ƿ��Է�������������

	TipInfo tryToFlip();                           //���Է���
	TipInfo tryToShuffle();                        //����ϴ��
protected:
	HandCardArea *m_handAreaData;
	PlaceCardArea *m_placeAreaData;
	CollectCardArea *m_collectAreaData;

};


#endif
