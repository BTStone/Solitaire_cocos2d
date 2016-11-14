#include "CardDataManager.h"
#include "IOManager.h"
#include "ServerManager.h"
#include "DailyChallengeManager.h"
#include "NewGameManager.h"
#include "ClientCtrl.h"
#include "GameLayer.h"

void CardDataManager::stTest_print()
{
	m_collectCardArea.stTest_print();
	m_placeCardArea.stTest_print();
	m_handCardArea.stTest_print();
}

int CardDataManager::drill()
{
	m_collectNum = 0;
	m_shuffleNum = 0;
	int totalSufNum = 1;
	TipInfo tipRet = Invalid_Tip_Info;
	do 
	{
		tipRet = m_tipManager->newTip();
		doByTipInfo(tipRet);
	} while (!TipManager::isInvalidTipInfo(tipRet) && m_shuffleNum<totalSufNum);
	/*while (m_shuffleNum < totalSufNum)
	{
		TipInfo tipRet = m_tipManager->newTip();
		doByTipInfo(tipRet);
	}*/
	//CCLOG("collectNum:%d",m_collectNum);
	return m_collectNum;
}

string CardDataManager::convertToInfoStr()
{
	return m_placeCardArea.convertToInfoStr();
}

void CardDataManager::doByTipInfo(TipInfo tipInfo)
{
	ST_RETURN_IF(TipManager::isInvalidTipInfo(tipInfo));
	CardArea resArea = tipInfo.m_resArea;
	int resQurIdx = tipInfo.m_resQueIdx;
	int resCardIdx = tipInfo.m_resCardIdx;
	CardArea desArea = tipInfo.m_desArea;
	int desQueIdx = tipInfo.m_desQueIdx;
	vector<Card> cards;
	switch (resArea)
	{
	case HAND_CLOSE_AREA:
	{
		bool isShuffle = !m_handCardAreaBak.flipCard();
		if (isShuffle)
		{
			m_shuffleNum++;
		}
		return;
		break;
	}
	case HAND_OPEN_AREA:
	{
		Card cd = m_handCardAreaBak.removeTheLastCardOfOpenQue();
		if (!cd.isInvalidCard())
		{
			cards.push_back(cd);
		}
		break;
	}
	case PLACE_AREA:
		cards = m_placeCardAreaBak.removeCardsAt(resQurIdx, resCardIdx);
		m_placeCardAreaBak.openAllLastCard();

		break;
	default:
		break;
	}
	doByTipDest(cards, desArea, desQueIdx);
}

void CardDataManager::doByTipDest(vector<Card> cards, CardArea desAre, int desQueIdx)
{
	//提示的目的区域不会是手牌区
	switch (desAre)
	{
	case COLLECT_AREA:
		m_collectNum++;
		m_collectCardAreaBak.addCardsAt(desQueIdx,cards);
		break;
	case PLACE_AREA:
		m_placeCardAreaBak.addCardsAt(desQueIdx, cards);
		break;
	default:
		break;
	}
}

void CardDataManager::initEmptyArea()
{
	vector<Card> cards;
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_placeCardArea.setQue(i, cards);
	}
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		m_collectCardArea.setQue(i, cards);
	}

	m_handCardArea.setClosedQueue(cards);

	m_handCardArea.setOpenQueue(cards);
}


void CardDataManager::init()
{
	//随机种子
	static int initNum = 0;
	initNum++;
	if (initNum == 1)
	{
		srand((unsigned int)time(0));    //随机种子只配一次就好，不能循环配
	}
	

	/*
	//默认普通
	int totalNum = UserDefault::getInstance()->getIntegerForKey("strFirstPlayGame", 0);
	if (totalNum == 0)
	{
		initEmptyArea();
		UserDefault::getInstance()->setIntegerForKey("strFirstPlayGame", totalNum + 1);
		return;
	}
	*/
	init52Cards();
	initEachArea();
	
	//备份
	m_handCardAreaBak = m_handCardArea;
	m_collectCardAreaBak = m_collectCardArea;
	m_placeCardAreaBak = m_placeCardArea;
	m_placeCardAreaBak.openAllLastCard();


	m_tipManager = TipManager::create(&m_collectCardAreaBak, &m_handCardAreaBak, &m_placeCardAreaBak);
	ST_RETURN_IF(!m_tipManager);
	if (m_initGameType == kNewGameInit || m_initGameType == kChallengeInit)
	{
		bakReplayCards();
		UserDefault::getInstance()->setBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
		if (m_initGameType == kChallengeInit)
		{
			UserDefault::getInstance()->setBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, true);
		}
	}
}


void CardDataManager::init52Cards()
{
	m_cards.clear();
	for (int i = 0; i < 52; i++)
	{
		Card card;
		card.m_number = i / 4 + 1;
		if (i % 4 == 0)
		{
			card.m_color = BLACK_CARD;
		}
		else if (i % 4 == 1)
		{
			card.m_color = RED_CARD;
		}
		else if (i % 4 == 2)
		{
			card.m_color = FLOWER_CARD;
		}
		else
		{
			card.m_color = SQUARE_CARD;
		}
		m_cards.push_back(card);
	}
}

void CardDataManager::removeAlreadyHasCardFrom52Cards(vector<Card> hasCards)
{
	int len1 = m_cards.size();
	int len2 = hasCards.size();
	for (int i = 0; i < len2; i++)
	{
		Card hasCd = hasCards[i];
		for (int j = 0; j < len1; j++)
		{
			Card cd = m_cards[j];
			if (cd == hasCd)
			{
				m_cards.erase(m_cards.begin() + j);
				len1 = m_cards.size();
				break;
			}
		}
	}
}

void CardDataManager::clear()
{
	m_collectCardArea.clear();
	m_handCardArea.clear();
	m_placeCardArea.clear();
}

void CardDataManager::initEachArea()
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	int newGameNum = UserDefault::getInstance()->getIntegerForKey(STRG_NEW_GAME_NUM, 0);
	int challengeNum = UserDefault::getInstance()->getIntegerForKey(STRG_CHALLENGE_NUM, 0);
	int replayNum = UserDefault::getInstance()->getIntegerForKey(STRG_REPLAY_NUM, 0);

	if (m_initGameType != kCommonInit)
	{
		UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_PLAY_NUM, playNum + 1);
	}
	if (m_initGameType == kNewGameInit && newGameNum < 1)  //前1局是活局
	{
		initFirstFewGames();
		UserDefault::getInstance()->setIntegerForKey(STRG_NEW_GAME_NUM, newGameNum + 1);
		return;
	}

	initCollectCardArea();
	initPlaceCardArea();
	initHandCardArea();

	if (m_initGameType == kNewGameInit)
	{
		UserDefault::getInstance()->setIntegerForKey(STRG_NEW_GAME_NUM, newGameNum + 1);
	}
	else if (m_initGameType == kChallengeInit)
	{
		UserDefault::getInstance()->setIntegerForKey(STRG_CHALLENGE_NUM, challengeNum + 1);
	}
	else if (m_initGameType == kChallengeInit)
	{
		UserDefault::getInstance()->setIntegerForKey(STRG_REPLAY_NUM, replayNum + 1);
	}

}


void CardDataManager::initFirstFewGames()
{
	const char *games[] = {
		"0-12 |3-10 3-8 |3-1 1-7 3-6 |0-8 3-3 0-7 2-2 |1-4 2-7 2-9 0-1 3-12 |2-13 1-2 1-9 2-10 2-3 0-13 |0-3 1-8 3-4 2-4 0-9 2-11 1-3 |",
		"0-13 |3-1 2-4 |2-3 2-12 1-11 |3-8 0-4 2-1 3-5 |2-8 2-13 0-9 1-5 3-9 |3-7 1-9 0-12 0-8 3-2 3-13 |0-10 0-3 1-4 0-1 1-3 0-6 0-11 |",
		"0-2 |1-8 0-5 |2-8 1-3 0-3 |1-9 1-2 2-5 0-9 |3-10 2-2 2-3 0-6 3-7 |2-7 2-1 3-13 3-5 3-1 3-4 |1-7 1-10 0-11 1-4 0-13 0-7 1-12 |"
		};
	int num = 3;
	int newGameNum = UserDefault::getInstance()->getIntegerForKey(STRG_NEW_GAME_NUM, 0);
	int idx = rand() % num; //+ newGameNum*off;
	//place area
	for (int i = 0; i < Place_Area_Num; i++)
	{
		vector<Card> cards = ServerManager::share()->getCardsOfPlaceQue(games[idx], i);
		removeAlreadyHasCardFrom52Cards(cards);
		//挑战测试=================
		int sz = cards.size();
		for (int i = 0; i < sz; i++)
		{
			if (i == (sz - 1) || i >= (Place_Area_Num - 1))
			{
				cards[i].m_isOpen = true;
			}
		}

		//挑战测试=================
		m_placeCardArea.initQueue(i, cards);
	}

	initHandCardAreaWithChallenge();       //初始化handArea
}

void CardDataManager::initCollectCardArea()
{
	if (m_initGameType == kCommonInit)
	{
		initCollectCardAreaWithCommon();
	}
}

void CardDataManager::initPlaceCardArea()
{
	if (m_initGameType == kChallengeInit)
	{
		initPlaceCardAreaWithChallenge();
	}
	else if (m_initGameType == kReplayInit)
	{
		initPlaceCardAreaWithReplay();
	}
	else if (m_initGameType == kNewGameInit)
	{
		initPlaceCardAreaWithNewGame();
	}
	else
	{
		initPlaceCardAreaWithCommon();
	}
}

void CardDataManager::initHandCardArea()
{
	if (m_initGameType == kChallengeInit)
	{
		initHandCardAreaWithChallenge();
	}
	else if (m_initGameType == kReplayInit)
	{
		initHandCardAreaWithReplay();
	}
	else if (m_initGameType == kNewGameInit)
	{
		initHandCardAreaWithNewGame();
	}
	else
	{
		initHandCardAreaWithCommon();
	}

}

void CardDataManager::initCollectCardAreaWithCommon()
{
	char buf[64];
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		sprintf(buf, "collect_%d", i + 1);
		vector<Card> cards = IOManager::share()->readCardByKey("commonBak.json", buf);
		m_collectCardArea.setQue(i, cards);
	}
}



void CardDataManager::initPlaceCardAreaWithCommon()
{
	char buf[64];
	for (int i = 0; i < Place_Area_Num; i++)
	{
		sprintf(buf, "place_%d", i + 1);
		vector<Card> cards = IOManager::share()->readCardByKey("commonBak.json", buf);
		int sz = cards.size();
		if (sz > 0)
		{
			cards[sz - 1].m_isOpen = true;
		}
		m_placeCardArea.setQue(i, cards);
	}
}

void CardDataManager::initHandCardAreaWithCommon()
{
	vector<Card> closedCards = IOManager::share()->readCardByKey("commonBak.json","hand_closed");
	m_handCardArea.setClosedQueue(closedCards);

	vector<Card> openCards = IOManager::share()->readCardByKey("commonBak.json", "hand_open");
	m_handCardArea.setOpenQueue(openCards);
}

void CardDataManager::initPlaceCardAreaWithNewGame()
{
	m_placeCardArea.clear();
	int count = 0;
	vector<Card> cards;
	int idx;
	int defaultRateNum = 0;
	int winPercentNum = defaultRateNum;
	bool isRand = true;
	string cfgStr = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_DAILY_CHALLENGE_PARAM);
	string value = ClientCtrl::share()->parseUMengOnlineConfig(cfgStr, "winning_rate");
	log("initPlaceCardAreaWithNewGame,cfgStr:%s, value:%s", cfgStr.c_str(), value.c_str());
	winPercentNum = atoi(value.c_str());
	winPercentNum = ((winPercentNum <= 0) || (winPercentNum >= 100)) ? defaultRateNum : winPercentNum;
	
	isRand = ((rand() % 100) > winPercentNum);
	if (GameLayer::m_isStoreCardLibMode)
	{
		isRand = true;
	}
	if(!isRand)
	{
		log("initPlaceCardAreaWithNewGame not rand");
		string infoStr = NewGameManager::share()->getNewGameInfo();
		initPlaceAreaWithInfo(infoStr);
	}
	else
	{
		log("initPlaceCardAreaWithNewGame is rand");
		for (int i = 1; i < Place_Area_Num + 1; i++)
		{
			for (int j = 0; j < i; j++)
			{
				Card card = getCardByRand();
				card.m_isOpen = false;// (j == i - 1);
				cards.push_back(card);
			}
			idx = i - 1;
			m_placeCardArea.initQueue(idx, cards);
			cards.clear();
		}
	}
}

void CardDataManager::initHandCardAreaWithNewGame()
{
	int num = 24;
	vector<Card> cards;
	m_handCardArea.clear();
	for (int i = 0; i < num; i++)
	{
		Card card = getCardByRand();
		card.m_isOpen = false;
		if (!card.isInvalidCard())
		{
			cards.push_back(card);
		}
		
	}
	m_handCardArea.init(cards);
}

void CardDataManager::initPlaceCardAreaWithChallenge()
{
	//const char *testStr = "3-10 |2-1 2-7 ||2-12 |2-9 2-13|3-2 3-1 2-2  |2-10 0-12 3-11 1-9 2-11 2-3 0-13 1-12 0-11 1-10 0-9 1-8 0-7 1-6 0-5 1-4 0-3 1-2 0-1|";
	int year = DailyChallengeManage::share()->getChallengeYear();
	int mon = DailyChallengeManage::share()->getChallengeMonth();
	int day = DailyChallengeManage::share()->getChallengeDay();
	string testStr = DailyChallengeManage::share()->getCardInfoByData(year, mon, day);
	initPlaceAreaWithInfo(testStr);
	return;
	for (int i = 0; i < Place_Area_Num; i++)
	{
		vector<Card> cards = ServerManager::share()->getCardsOfPlaceQue(testStr.c_str(), i);
		removeAlreadyHasCardFrom52Cards(cards);
		//挑战测试=================
		int sz = cards.size();
		for (int i = 0; i < sz; i++)
		{
			if (i == (sz - 1) || i>=(Place_Area_Num-1))
			{
				cards[i].m_isOpen = true;
			}
		}

		//挑战测试=================
		m_placeCardArea.initQueue(i, cards);
	}
}

void CardDataManager::initPlaceAreaWithInfo(string placeInfo)
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		vector<Card> cards = ServerManager::share()->getCardsOfPlaceQue(placeInfo.c_str(), i);
		removeAlreadyHasCardFrom52Cards(cards);
		//挑战测试=================
		int sz = cards.size();
		for (int i = 0; i < sz; i++)
		{
			if (i == (sz - 1) || i >= (Place_Area_Num - 1))
			{
				cards[i].m_isOpen = true;
			}
		}

		//挑战测试=================
		m_placeCardArea.initQueue(i, cards);
	}
}

void CardDataManager::initHandCardAreaWithChallenge()
{
	initHandCardAreaWithNewGame();
}

void CardDataManager::initPlaceCardAreaWithReplay()
{
	char buf[64];
	for (int i = 0; i < Place_Area_Num; i++)
	{
		sprintf(buf, "place_%d", i + 1);
		vector<Card> cards = IOManager::share()->readCardByKey("replayBak.json",buf);
		int sz = cards.size();
		ST_RETURN_IF(sz <= 0);
		cards[sz-1].m_isOpen = true;
		m_placeCardArea.setQue(i, cards);
	}
}

void CardDataManager::initHandCardAreaWithReplay()
{
	vector<Card> closedCards = IOManager::share()->readCardByKey("replayBak.json","hand_closed");
	m_handCardArea.setClosedQueue(closedCards);

	vector<Card> openCards = IOManager::share()->readCardByKey("replayBak.json","hand_open");
	m_handCardArea.setOpenQueue(openCards);
}


Card CardDataManager::getCardByRand()
{
	if (m_cards.size() <= 0)
	{
		return INVALID_CARD;
	}
	int index = rand() % m_cards.size();
	Card card = m_cards[index];
	m_cards.erase(m_cards.begin() + index);
	return card;
}

void CardDataManager::bakReplayCards()
{
	string re = "";

	//json开头
	const char *jsonBeginStr = "{\n";
	re += jsonBeginStr;

	//收牌区
	string collectStr = m_collectCardArea.bakAllCards();
	re += collectStr;

	//手牌区
	string handStr = m_handCardArea.bakAllCards();
	re += handStr;

	//置牌区
	string placeStr = m_placeCardArea.bakAllCards();
	re += placeStr;

	//json结尾
	const char *jsonEndStr = "\n}";
	re += jsonEndStr;

	IOManager::share()->write(re, "replayBak.json", "w");
}
