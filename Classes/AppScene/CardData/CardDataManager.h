/*负责初始化牌局*/

#ifndef __ST_SOLITAIRE_CARD_DATA_MANAGER_H__
#define __ST_SOLITAIRE_CARD_DATA_MANAGER_H__

#include "CollectCardArea.h"
#include "PlaceCardArea.h"
#include "HandCardArea.h"
#include "STBase.h"
#include "TipManager.h"

enum InitGameType
{
	kCommonInit = 0,              //普通进入游戏时根据最后一次退出时的牌局进行初始化
	kNewGameInit,                 //新游戏
	kReplayInit,                  //replay本局游戏
	kChallengeInit                //每日挑战
};

class CardDataManager
{
public:
	void init();
	void init52Cards();                                //初始化52张牌
	void initEachArea();                               //初始化各个区域
	void initCollectCardArea();                        //初始化收牌区
	void initCollectCardAreaWithCommon();              //普通初始化收牌区
	void initPlaceCardArea();                          //初始化置牌区
	void initPlaceCardAreaWithCommon();                //正常开始游戏时初始化置牌区
	void initPlaceCardAreaWithReplay();                //replay时初始化置牌区
	void initPlaceCardAreaWithNewGame();               //新游戏初始化置牌区
	void initPlaceCardAreaWithChallenge();             //每日挑战时初始化置牌区
	void initHandCardArea();                           //初始化手牌区
	void initHandCardAreaWithCommon();                 //普通开始游戏时初始化手牌区
	void initHandCardAreaWithReplay();                 //replay时初始化手牌区
	void initHandCardAreaWithNewGame();                //新游戏时初始化手牌区
	void initHandCardAreaWithChallenge();              //每日挑战时初始化手牌区
	void initFirstFewGames();                          //初始化前几局牌，固定
	void initPlaceAreaWithInfo(string placeInfo);
	
	int drill();                                       //模拟，返回可收集牌的个数
	void doByTipInfo(TipInfo tipInfo);                 //根据提示信息进行模拟
	void doByTipDest(vector<Card> cards,
		CardArea desAre, int desQueIdx);               //往提示的目的区域添加牌
	Card getCardByRand();                              //随机从m_cards中抽取一张牌，为防止抽到重复的牌，必须得从m_cards里删除抽取的牌

	void bakReplayCards();                             //存储newgame的牌局，方便replay
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
	vector<Card> m_cards;                              //52牌集合
	int m_collectNum;                                  //模拟收牌数量
	TipManager *m_tipManager;                          //提示
	int m_shuffleNum;                                  //洗牌次数
};

#endif
