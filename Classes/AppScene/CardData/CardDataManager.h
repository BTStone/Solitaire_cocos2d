/*�����ʼ���ƾ�*/

#ifndef __ST_SOLITAIRE_CARD_DATA_MANAGER_H__
#define __ST_SOLITAIRE_CARD_DATA_MANAGER_H__

#include "CollectCardArea.h"
#include "PlaceCardArea.h"
#include "HandCardArea.h"
#include "STBase.h"
#include "TipManager.h"

enum InitGameType
{
	kCommonInit = 0,              //��ͨ������Ϸʱ�������һ���˳�ʱ���ƾֽ��г�ʼ��
	kNewGameInit,                 //����Ϸ
	kReplayInit,                  //replay������Ϸ
	kChallengeInit                //ÿ����ս
};

class CardDataManager
{
public:
	void init();
	void init52Cards();                                //��ʼ��52����
	void initEachArea();                               //��ʼ����������
	void initCollectCardArea();                        //��ʼ��������
	void initCollectCardAreaWithCommon();              //��ͨ��ʼ��������
	void initPlaceCardArea();                          //��ʼ��������
	void initPlaceCardAreaWithCommon();                //������ʼ��Ϸʱ��ʼ��������
	void initPlaceCardAreaWithReplay();                //replayʱ��ʼ��������
	void initPlaceCardAreaWithNewGame();               //����Ϸ��ʼ��������
	void initPlaceCardAreaWithChallenge();             //ÿ����սʱ��ʼ��������
	void initHandCardArea();                           //��ʼ��������
	void initHandCardAreaWithCommon();                 //��ͨ��ʼ��Ϸʱ��ʼ��������
	void initHandCardAreaWithReplay();                 //replayʱ��ʼ��������
	void initHandCardAreaWithNewGame();                //����Ϸʱ��ʼ��������
	void initHandCardAreaWithChallenge();              //ÿ����սʱ��ʼ��������
	void initFirstFewGames();                          //��ʼ��ǰ�����ƣ��̶�
	void initPlaceAreaWithInfo(string placeInfo);
	
	int drill();                                       //ģ�⣬���ؿ��ռ��Ƶĸ���
	void doByTipInfo(TipInfo tipInfo);                 //������ʾ��Ϣ����ģ��
	void doByTipDest(vector<Card> cards,
		CardArea desAre, int desQueIdx);               //����ʾ��Ŀ�����������
	Card getCardByRand();                              //�����m_cards�г�ȡһ���ƣ�Ϊ��ֹ�鵽�ظ����ƣ�����ô�m_cards��ɾ����ȡ����

	void bakReplayCards();                             //�洢newgame���ƾ֣�����replay
	void clear();
	
	void removeAlreadyHasCardFrom52Cards(vector<Card> hasCards);

	void stTest_print();
	void initEmptyArea();

	string convertToInfoStr();
	
	CC_SYNTHESIZE(InitGameType, m_initGameType, InitGameType);
	CC_SYNTHESIZE_READONLY(CollectCardArea, m_collectCardArea, CollectCardArea);
	CC_SYNTHESIZE_READONLY(PlaceCardArea, m_placeCardArea, PlaceCardArea);
	CC_SYNTHESIZE_READONLY(HandCardArea, m_handCardArea, HandCardArea);
	CC_SYNTHESIZE_READONLY(CollectCardArea, m_collectCardAreaBak, CollectCardAreaBak);      
	CC_SYNTHESIZE_READONLY(PlaceCardArea, m_placeCardAreaBak, PlaceCardAreaBak);
	CC_SYNTHESIZE_READONLY(HandCardArea, m_handCardAreaBak, HandCardAreaBak);
protected:
	vector<Card> m_cards;                              //52�Ƽ���
	int m_collectNum;                                  //ģ����������
	TipManager *m_tipManager;                          //��ʾ
	int m_shuffleNum;                                  //ϴ�ƴ���
};

#endif
