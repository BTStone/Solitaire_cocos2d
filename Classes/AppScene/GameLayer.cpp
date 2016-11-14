#include "GameLayer.h"
#include "SkinManager.h"
#include "PlayBoard.h"
#include "RootOptionBoard.h"
#include "BackDropBoard.h"
#include "BackStyleOptionBoard.h"
#include "FrontStyleOptionBoard.h"
#include "StatisticsBoard.h"
#include "IOManager.h"
#include "ClientAndroid.h"
#include "ScreenDirectionManager.h"
#include "OptionBoardManager.h"
#include "WinBoard.h"
#include "XMLParser.h" 
#include "ClientCtrl.h"
#include "ServerManager.h"
#include "Calendar.h"
#include "DailyChallengeBoard.h"
#include "DailyChallengeHelpBoard.h"
#include "DailyChallengeManager.h"
#include "NetworkFailBoard.h"
#include "LanguageSelectBoard.h"
#include "InviteDailyChallengeBoard.h"
#include "RateBoard.h"
#include "RateSuccessBoard.h"
#include "ResetDataBoard.h"
#include "InviteFriendBoard.h"
#include "InviteFriendSimpleBoard.h"
#include "NewThemeTipBoard.h"
#include "InviteSuccessBoard.h"
#include "NotificationManage.h"
#include"cocostudio/CocoStudio.h"
#include "TrophyInfoBoard.h"
using namespace cocostudio::timeline;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//android
#include "MobClickCpp.h"
using namespace umeng;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  //ios
#include "MobClickCpp.h"
using namespace umeng;
#else         //win32

#endif

const int Score_Collect_Move_Success = 15;
const int Score_Not_Collect_Move_Success = 5;
const char *strScoreOfLastGame = "strScoreOfLastGame";
const char *strTimeOfLastGame = "strTimeOfLastGame";
const char *strMovesOfLastGame = "strMovesOfLastGame";
bool GameLayer::m_isStoreCardLibMode = false;

GameLayer::GameLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgSetTouchEnableCallBack), ST_SET_GAME_LAYER_TOUCH_ENABLE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgShowPlayBoardCallBack), ST_SHOW_PLAY_BOARD, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgNewGameCallBack), ST_NEW_GAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgReplayCallBack), ST_REPLAY_GAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgTipCallBack), ST_TIP_FOR_GAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgUndoCallBack), ST_UNDO_FOR_GAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgShowOptionBoardCallBack), ST_SHOW_OPTION_BOARD, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgShowOptionBoardWithOutEffectCallBack), ST_SHOW_OPTION_BOARD_WITHOUT_EFFECT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgUpdateUICallBack), ST_UPDATE_AFTER_SELECT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgSwitchScreen), ST_SWITCH_SCREEN, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgSwitchHandTypeCallBack), ST_SWITCH_HAND_TYPE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgDailyChallengeGameCallBack), ST_DAILY_CHALLENGE_GAME_START, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgConnectNetworkShowDailyChallengeCallBack), ST_CONNECT_NETWORK, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgUpdateLanguageCallBack), ST_UPDATE_LANGUAGE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgGameEnterBackGroundCallBack), ST_GAME_ENTER_BACKGROUND, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgExitGameCallBack), ST_EXIT_GAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgClickAndroidReturnButtonCallBack), ST_CLICK_ANDROID_RETURN_BUTTON, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgChangeToNewThemeCallBack), ST_CHANGE_TO_NEW_THEME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::msgShowNewThemeBoardCallBack), ST_SHOW_NEW_THEME_BOARD_DAILY, NULL);

}

GameLayer::~GameLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

bool GameLayer::init()
{
	ScreenDirectionManager::share();
	resumeFrameAndDesignSize(); 

	log("GameLayer, beginInit");
	if (!Layer::init())
	{
		return false;
	}

	onEnterGameLayer();
	m_bg = NULL;
	m_touchAble = true;                //默认可以点击
	m_isDrag = false;
	m_score = UserDefault::getInstance()->getIntegerForKey(strScoreOfLastGame, 0);
	m_time = UserDefault::getInstance()->getIntegerForKey(strTimeOfLastGame, 0);;
	m_moveTimes = UserDefault::getInstance()->getIntegerForKey(strMovesOfLastGame, 0);;
	m_canTime = false;
	m_tipping = false;
	m_canClickReturnBt = true;
	m_isAutoCollecting = false;
	m_autoTipTime = 0;
	m_bottomMenu = NULL;
	m_autoCollectMenu = NULL;
	m_uiCollectArea = NULL;
	m_uiHandArea = NULL;
	m_uiPlaceArea = NULL;
	m_transportQue = NULL;

	m_isLeftHandType = UserDefault::getInstance()->getBoolForKey("st_is_left_hand", false);
	if (!m_isStoreCardLibMode)
	{
		//正常游戏
		m_cardDataManager.setInitGameType(kCommonInit);
		m_cardDataManager.init();
	}
	else
	{
		//搜集卡牌库模式
		int ttNum = 5000;
		int newGameNum = 0;
		int collectNum = 0;
		m_cardDataManager.setInitGameType(kNewGameInit);
		for (int i = 0; i < ttNum; i++)
		{
			//模拟收牌，获取收牌数量======      begin       =====
			do
			{
				m_cardDataManager.init();
				collectNum = m_cardDataManager.drill();
				newGameNum++;
				log("newGameNum : %d", newGameNum);
				log("has run num----------------------------- : %d", i + 1);
			} while (collectNum != 52);
			collectNum = 0;
			string infoStr = m_cardDataManager.convertToInfoStr();
			int isDraw3 = UserDefault::getInstance()->getBoolForKey("st_is_draw_3", false);
			string fileNamm = "Lib_Draw3.txt";
			if (!isDraw3)
			{
				fileNamm = "Lib_Draw1.txt";
			}
			IOManager::share()->write(infoStr, fileNamm, "a");
			//模拟收牌，获取收牌数量======      end         =====
		}
	}

	log("GameLayer, midinit");
	initGameBg();
	initEachArea();
	initHeadLabel();
	addTouchListerner();
	initBottonMenu();
	
	if (m_cardDataManager.getInitGameType() == kNewGameInit ||
		m_cardDataManager.getInitGameType() == kChallengeInit)
	{
		addDealCardEffect();
	}

	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (!isPortrait)
	{
		setLandscapeStatus();
	}
	if (m_isLeftHandType)
	{
		msgSwitchHandTypeCallBack((Ref*)m_isLeftHandType);
	}

	//WinBoard::setData(100, 100, 100, 100, 100);
	//NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kRateBoard);

	/*Node* node = CSLoader::createNode("Node_hourglass.csb");
	auto action = CSLoader::createTimeline("Node_hourglass.csb");
	node->runAction(action);
	node->setPosition(CLDWinCenter());
	action->gotoFrameAndPlay(0, 100, true);
	this->addChild(node, 100);*/

	tryToShowInvitePlayDailyBoard();
	NotificationManage::share()->tryToPostDailyChallenge();
	NotificationManage::share()->tryToPostThreeDaysLeave();
	scheduleUpdate();
	ClientCtrl::share()->tryToShowBanner();

	if (SkinManager::share()->getFindNewTheme())  
	{
		///显示新主题提示界面
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kNewThemeTipBoard);
	}
	return true;
}

void GameLayer::msgChangeToNewThemeCallBack(Ref *ref)
{
	useNewTheme();
}

void GameLayer::useNewTheme()
{
	string gameBgName = SkinManager::share()->getNewGameBgName();
	string cardBgName = SkinManager::share()->getNewCardBackName();

	int gameBgId = SkinManager::share()->getGameBgIdByGameBgName(gameBgName);
	int cardBgId = SkinManager::share()->getCardBgIdByGameBgName(cardBgName);

	SkinManager::share()->selectGameBg(gameBgId);
	SkinManager::share()->selectCardBack(cardBgId);

	NotificationCenter::getInstance()->postNotification(ST_UPDATE_AFTER_SELECT);
}

void GameLayer::resumeFrameAndDesignSize()
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();

	Size frameSize = glview->getFrameSize();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	frameSize.width = ClientCtrl::share()->getScreenWidth();
	frameSize.height = ClientCtrl::share()->getScreenHeight();
#endif
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	glview->setFrameSize(smallNum, bigNum);

	Size normalFrameSize = Size(smallNum, bigNum);
	Size resSize = Size(720, 1280);
	glview->setDesignResolutionSize(resSize.width,
		resSize.width * normalFrameSize.height / normalFrameSize.width, kResolutionShowAll);

	Size designSize = glview->getDesignResolutionSize();

	if (designSize.width > designSize.height)
	{
		glview->setDesignResolutionSize(designSize.height, designSize.width, kResolutionShowAll);
	}
	else
	{
		glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	}



	Size designSize2 = glview->getDesignResolutionSize();
	log("resumeFrameAndDesignSize,afterDesign,w:%f, h:%f", designSize2.width, designSize2.height);

}

void GameLayer::NotificationDailyChallenge()
{
	int hour = 11;       //第二天的几点开始
	time_t localTm = STTimeManager::share()->getLocalTime();
	struct tm *nowTm = localtime(&localTm);
	int curYear = nowTm->tm_year + 1900;
	int curMon = nowTm->tm_mon + 1;
	int curDay = nowTm->tm_mday;

	//如果当天已经推送就不再推送
	char buf[63];
	sprintf(buf, "Is_notification_daily_challenge_%d_%d_%d", curYear, curMon, curDay);
	bool hasPost = UserDefault::getInstance()->getBoolForKey(buf, false);
	ST_RETURN_IF(hasPost);      

	UserDefault::getInstance()->setBoolForKey(buf, true);
	int curHour = nowTm->tm_hour;
	int curMinute = nowTm->tm_min;
	int curSec = nowTm->tm_sec;
	int offHour = hour + 24 - curHour - 1;
	int offMin = 60 - curMinute - 1;
	int offSec = 60 - curSec;

	int totalOffSec = offHour * 60 * 60 + offMin * 60 + offSec;



	int delayMs = totalOffSec * 1000;         //毫秒
	string titleStr = "Daily challenge is ready";
	string contStr = "Daily challenge is ready";
	ClientCtrl::share()->setNotification(titleStr, contStr, delayMs, Notification_Daily_Challenge_Id);
	/*测试定时发送消息
	struct tm tm1 = { 0 };
	tm1.tm_year = 2016 - 1900;
	tm1.tm_mon = 5 - 1;
	tm1.tm_mday = 4;
	tm1.tm_hour = curHour;
	tm1.tm_min = curMinute;
	tm1.tm_sec = curSec;


	struct tm tm2 = { 0 };
	tm2.tm_year = 2016 - 1900;
	tm2.tm_mon = 5 - 1;
	tm2.tm_mday = 4;
	tm2.tm_hour = 11;
	tm2.tm_min = 30;
	tm2.tm_sec = 0;

	int diffTm = STTimeManager::share()->getDiffTime(tm1, tm2);
	diffTm *= 1000;
	ClientCtrl::share()->setNotification(titleStr, contStr, diffTm);*/
}

void GameLayer::stTest_netTime()
{
	NetTime * netTime = NetTime::create();
	netTime->requestNetTime(this, netTime_selector(GameLayer::onNetSuccessComplete), netTime_selector(GameLayer::onNetFailComplete));
	this->addChild(netTime);
}


void GameLayer::stTest_chinese()
{
	/*
	XMLParser *pXmlParser = XMLParser::parseWithFile("data/lang/English.xml");
	String *pTitle = pXmlParser->getString("app_name");
	Dictionary* dic = Dictionary::createWithContentsOfFile("data/lang/English.xml");
	String* strchinese = (String*)dic->objectForKey("startGame");*/
	
	/*string re = STLang::shared()->getValue("LangName");
	STLabelBMFont *l = STLabelBMFont::createWithBMFont("EurostileBold.fnt", re);
	//LabelTTF *l = LabelTTF::create(re.c_str(), "fonts/arial.ttf", 100);
	l->setPosition(CLDWinCenter());
	l->setColor(Color3B::BLACK);
	this->addChild(l, 1000); */

	//将sql装换
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	pBytes = FileUtils::getInstance()->getFileData("ss.sql", "r", &size);
	int num = 0;
	string str = "";
	bool isWriting = false;
	for (int i = 0; i < size; i++)
	{
		if (pBytes[i] == '\'')
		{
			num++;
			if (num % 2 == 1)
			{
				str += '\"';
			}
			else
			{
				str += "\",\n";
			}
			isWriting = (num % 2 == 1);
		}
		else if (isWriting)
		{
			str += pBytes[i];
		}
		
	}
	FILE *file = fopen("ss.txt", "w");
	if (file)
	{
		fwrite(str.c_str(), 1, strlen(str.c_str()), file);
		fclose(file);
	}
	delete pBytes;
}

void GameLayer::changeFrameSizeByIsPortrait(bool isPort)
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();
	Size frameSize = glview->getFrameSize();
	float width = (isPort && (frameSize.width > frameSize.height)) ? frameSize.height : frameSize.width;
	float height = ((!isPort) && (frameSize.width < frameSize.height)) ? frameSize.height : frameSize.width;
	glview->setFrameSize(width, height);
}

void GameLayer::setPortraitStatus(float dt)
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(dt),
		CallFunc::create(this, callfunc_selector(GameLayer::setPortraitStatusDelay))));


}

void GameLayer::setLandscapeStatus(float dt)
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(dt),
		CallFunc::create(this, callfunc_selector(GameLayer::setLandscapeStatusDelay))));

	
}


void GameLayer::setFrameAndDesignSizeWhenLandscape()
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();

	Size frameSize = glview->getFrameSize();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	frameSize.width = ClientCtrl::share()->getScreenWidth();
	frameSize.height = ClientCtrl::share()->getScreenHeight();
#endif

	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	log("setFrameAndDesignSizeWhenLandscape,fframeSize,w:%f, h:%f", frameSize.width, frameSize.height);

	glview->setFrameSize(bigNum, smallNum);
	Size normalFrameSize = Size(smallNum, bigNum);
	Size resSize = Size(720, 1280);
	glview->setDesignResolutionSize(resSize.width,
		resSize.width * normalFrameSize.height / normalFrameSize.width, kResolutionShowAll);

	Size designSize = glview->getDesignResolutionSize();
	if (designSize.width < designSize.height)
	{
		glview->setDesignResolutionSize(designSize.height, designSize.width, kResolutionShowAll);
	}
	else
	{
		glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	}


	Size designSize2 = glview->getDesignResolutionSize();
	log("setFrameAndDesignSizeWhenLandscape,afterDesign,w:%f, h:%f", designSize2.width, designSize2.height);
}

void GameLayer::setFrameAndDesignSizeWhenPortrait()
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();

	Size frameSize = glview->getFrameSize();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	frameSize.width = ClientCtrl::share()->getScreenWidth();
	frameSize.height = ClientCtrl::share()->getScreenHeight();
#endif
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	glview->setFrameSize(smallNum, bigNum);

	Size normalFrameSize = Size(smallNum, bigNum);
	Size resSize = Size(720, 1280);
	glview->setDesignResolutionSize(resSize.width,
		resSize.width * normalFrameSize.height / normalFrameSize.width, kResolutionShowAll);

	Size designSize = glview->getDesignResolutionSize();

	log("setFrameAndDesignSizeWhenPortrait,fframeSize,w:%f, h:%f", frameSize.width, frameSize.height);
	
	log("setFrameAndDesignSizeWhenPortrait,beforeframeSize,w:%f, h:%f", frameSize.width, frameSize.height);

	Size frameSize2 = glview->getFrameSize();
	log("setFrameAndDesignSizeWhenPortrait,afterframeSize,w:%f, h:%f", frameSize2.width, frameSize2.height);

	if (designSize.width > designSize.height)
	{
		glview->setDesignResolutionSize(designSize.height, designSize.width, kResolutionShowAll);
	}
	else
	{
		glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	}


	Size designSize2 = glview->getDesignResolutionSize();
	log("setFrameAndDesignSizeWhenPortrait,afterDesign,w:%f, h:%f", designSize2.width, designSize2.height);
}

void GameLayer::setLandscapeStatusDelay()
{
	log("GameLayer::setLandscapeStatusDelay");
	setFrameAndDesignSizeWhenLandscape();

	ScreenDirectionManager::share()->setScreenType(kScreenLandscape);
	m_bg->setPosition(CLDWinCenter());
	fixGameBg();

	//个区域切换
	bool isLeft = m_isLeftHandType;
	msgSwitchHandTypeCallBack((Ref*)false);
	m_uiCollectArea->setLandscapeStatus();
	m_uiPlaceArea->setLandscapeStatus();
	m_uiHandArea->setLandscapeStatus();
	msgSwitchHandTypeCallBack((Ref*)isLeft);

	Size winSize = CLDWinSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 originPos = Director::getInstance()->getVisibleOrigin();

	float posY = winSize.height - Game_Layer_Head_Label_Height*0.5 + 5;
	m_scoreLabel->setPosition(Vec2(winSize.width*0.15, posY));
	m_timeLabel->setPosition(Vec2(winSize.width*0.5, posY));
	m_moveLabel->setPosition(Vec2(winSize.width*0.85, posY));
	initBottonMenu();
	OptionBoardManager::share()->refreshBoards();
	refreshPlayBoardWhenSwitchScreen();
	addAutoCollectMenu();
	m_uiPlaceArea->tryToScale();
	m_uiPlaceArea->removeAllTipLight();
	ClientCtrl::share()->hideBanner();

	placeTransQue();
	addTouchListerner();

	Node *moreGaIcon = this->getChildByName(STR_MORE_GAME_ICON);
	if (moreGaIcon)
	{
		moreGaIcon->setVisible(false);
	}
}

void GameLayer::setPortraitStatusDelay()
{
	log("GameLayer::setPortraitStatusDelay");
	setFrameAndDesignSizeWhenPortrait();

	ScreenDirectionManager::share()->setScreenType(kScreenPortrait);

	//个区域切换
	bool isLeft = m_isLeftHandType;
	msgSwitchHandTypeCallBack((Ref*)false);
	m_uiCollectArea->setPortraitStatus();
	m_uiPlaceArea->setPortraitStatus();
	m_uiHandArea->setPortraitStatus();
	msgSwitchHandTypeCallBack((Ref*)isLeft);

	m_bg->setPosition(CLDWinCenter());
	fixGameBg();

	Size winSize = CLDWinSize();
	float posY = winSize.height - Game_Layer_Head_Label_Height*0.5 + 5;
	m_scoreLabel->setPosition(Vec2(winSize.width*0.05, posY));
	m_timeLabel->setPosition(Vec2(winSize.width*0.5, posY));
	m_moveLabel->setPosition(Vec2(winSize.width*0.95, posY));

	initBottonMenu();
	OptionBoardManager::share()->refreshBoards();
	refreshPlayBoardWhenSwitchScreen();
	addAutoCollectMenu();
	m_uiPlaceArea->tryToScale();
	m_uiPlaceArea->removeAllTipLight();
	ClientCtrl::share()->tryToShowBanner();

	placeTransQue();
	addTouchListerner();

	Node *moreGaIcon = this->getChildByName(STR_MORE_GAME_ICON);
	if (moreGaIcon)
	{
		moreGaIcon->setVisible(true);
	}
}

void GameLayer::placeTransQue()
{
	ST_RETURN_IF(!m_transportQue);
	//将目的地暂时初始为始发地
	m_transportQue->stopAllActions();
	m_transportQue->setDestArea(m_transportQue->getResArea());
	m_transportQue->setDestQueIndex(m_transportQue->getResQueIndex());
	transportMove();
}

void GameLayer::refreshPlayBoardWhenSwitchScreen()
{
	Node *node = getChildByTag(ST_PLAY_BOARD_TAG);
	ST_RETURN_IF(!node);
	node->removeFromParent();
	showPlayBoardWithOutEffect();
}

void GameLayer::addDealCardEffect()
{
	ClientCtrl::share()->playSound(SOUND_NEW_GAME_DEAL);
	m_uiPlaceArea->setVisible(false);
	Vec2 worldPos = m_uiHandArea->getClosedAreaPosition();
	Vec2 beginPos = convertToNodeSpace(worldPos);
	vector<Vec2> endWorldPos = getDealPositions();
	int sz = endWorldPos.size();
	for (int i = 0; i < sz; i++)
	{
		Vec2 endPos = convertToNodeSpace(endWorldPos[i]);
		Card closedCd;
		closedCd.m_isOpen = false;
		CardSprite *cardSp = CardSprite::create(closedCd);
		ST_RETURN_IF(!cardSp);
		cardSp->setPosition(beginPos);
		this->addChild(cardSp, 10);
		m_touchAble = false;
		float mvDt = 0.2f;
		float delayOff = 0.01f;
		MoveTo *mvTo = MoveTo::create(mvDt, endPos);
		ST_RETURN_IF(!mvTo);
		DelayTime *delay = DelayTime::create(i * delayOff);
		ST_RETURN_IF(!delay);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(GameLayer::dealCardOverCallFunc));
		ST_RETURN_IF(!callFunc);
		Sequence *seq = NULL;
		if (i == sz - 1)
		{
			seq = Sequence::create(delay, mvTo, callFunc, NULL);
		}
		else
		{
			seq = Sequence::create(delay, mvTo, NULL);
		}
		ST_RETURN_IF(!seq);
		m_dealCards.push_back(cardSp);
		cardSp->runAction(seq);
	}

}

vector<Vec2> GameLayer::getDealPositions()
{
	vector<Vec2> re;
	int rowNum = 7, cowNum = 7;
	for (int row = 0; row < rowNum; row++)
	{
		for (int cow = 0; cow < cowNum; cow++)
		{
			if (cow >= row)
			{
				Vec2 pos = m_uiPlaceArea->getCardWorldPosition(row, cow);
				re.push_back(pos);
			}
		}
	}
	return re;
}

void GameLayer::dealCardOverCallFunc()
{
	int sz = m_dealCards.size();
	for (int i = 0; i < sz; i++)
	{
		m_dealCards[i]->removeFromParent();
	}
	m_dealCards.clear();
	m_uiPlaceArea->setVisible(true);
	m_uiPlaceArea->openLastCards();
	m_touchAble = true;

}


void GameLayer::update(float dt)
{
	updatePlayTime();
	updateAutoTipTime();
	//updateScreenDirection();
}


void GameLayer::msgSwitchScreen(Ref *ref)
{
	bool isPortrait = (bool)(ref);
	if (isPortrait)
	{
		this->setPortraitStatus();
	}
	else
	{
		this->setLandscapeStatus();
	}
}

void GameLayer::msgSwitchHandTypeCallBack(Ref *ref)
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	Size handAreaSize = m_uiHandArea->getAreaSize();
	bool isLeft = (bool)(ref);
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (1)//(isLeft != m_isLeftHandType)
	{
		m_isLeftHandType = isLeft;
		m_uiHandArea->setHandType(m_isLeftHandType);
		if (m_isLeftHandType)
		{
			m_uiHandArea->setPositionX(-(winSize.width - handAreaSize.width));
			float collectPosX = winSize.width - Collect_Area_Num*cardSize.width - (Collect_Area_Num + 1)*Place_Area_Gap_X;
			if (!isPortrait)
			{
				collectPosX = winSize.width - Place_Area_Gap_X - cardSize.width;
			}
			m_uiCollectArea->setPositionX(collectPosX);
		}
		else
		{
			m_uiHandArea->setPositionX(0);
			m_uiCollectArea->setPositionX(0);
		}

		if (m_bottomMenu)
		{
			m_bottomMenu->setHandType(m_isLeftHandType);
		}
	}
}

void GameLayer::updatePlayTime()
{
	static int count = 0;
	if (m_canTime)
	{
		count++;
		if (count >= 60)
		{
			count = 0;
			m_time++;
			UserDefault::getInstance()->setIntegerForKey(strTimeOfLastGame, m_time);
			updateTimeLabel();
		}
	}
}

void GameLayer::updateAutoTipTime()
{
	const char *strIsAutoHint = "st_is_auto_hint";
	bool isAutoTip = UserDefault::getInstance()->getBoolForKey(strIsAutoHint, true);
	if (isAutoTip)
	{
		m_autoTipTime++;
		if (m_autoTipTime >= 1200)
		{
			m_autoTipTime = 0;
			newTip(false);
		}
	}
}

void GameLayer::updateTimeLabel()
{
	string timeStr = convertToTimeStyle(m_time); 
	string strTime = STLang::shared()->getValue("time");
	strTime += "  ";
	strTime += timeStr;
	strTime = STLabelBMFont::convertFirstLetterToCapital(strTime);
	m_timeLabel->setString(strTime.c_str());
}

void GameLayer::stTest_shock()
{
	vector<Card> cards;
	Card cd1, cd2;
	cd1.m_number = 1;
	cd1.m_color = BLACK_CARD;
	cd1.m_isOpen = true;
	cd2.m_number = 2;
	cd2.m_color = BLACK_CARD;
	cd2.m_isOpen = true;
	cards.push_back(cd1);
	cards.push_back(cd2);
	TransportCardQueue *stest = TransportCardQueue::create(cards, COLLECT_AREA);
	ST_RETURN_IF(!stest);
	stest->setPosition(CLDWinCenter());
	this->addChild(stest, 10000);
	STShock *sk = STShock::create(1.0f);
	ST_RETURN_IF(!sk);
	CallFunc *ca = CallFunc::create([](){
	});
	ST_RETURN_IF(!ca);
	Sequence *seq = Sequence::create(sk, ca, NULL);
	ST_RETURN_IF(!seq);
	stest->runAction(seq);

}

void GameLayer::onEnterGameLayer()
{
	CardSprite::setCardScaleAndSize();
}

void GameLayer::initBottonMenu()
{
	if (m_bottomMenu)
	{
		m_bottomMenu->removeFromParent();
		m_bottomMenu = NULL;
	}
	Size winSize = CLDWinSize();
	bool isEmpt = AllAreaIsEmpty();
	m_bottomMenu = BottomMenuNode::create(isEmpt);
	ST_RETURN_IF(!m_bottomMenu);
	this->addChild(m_bottomMenu, 10);

	m_bottomMenu->setHandType(m_isLeftHandType);
}

void GameLayer::initHeadLabel()
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	int fontsize = Game_Layer_Head_Label_Height * 0.8;
	float minWinSize = min(winSize.width, winSize.height);
	Size textSize = Size(minWinSize*0.25, fontsize);
	float posY = winSize.height - Game_Layer_Head_Label_Height*0.5 + 5;
	float labScale = 10.0f;
	char buf[64];
	string strScore = STLang::shared()->getValue("score");
	strScore += "  ";
	sprintf(buf, "%d", m_score);
	strScore += buf;
	STLabelBMFont *label = STLabelBMFont::createWithBMFont("EurostileBold.fnt", strScore);
	m_scoreLabel = STLabelBMFont::createWithBMFont("EurostileBold.fnt", strScore);
	//m_scoreLabel = LabelTTF::create(strScore.c_str(), "fonts/arial.ttf", fontsize);
	ST_RETURN_IF(!m_scoreLabel);
	m_scoreLabel->setTextSize(textSize);
	float scoreScale = m_scoreLabel->getScale();
	labScale = min(labScale, scoreScale);
	m_scoreLabel->setAnchorPoint(Vec2(0, 0.5));
	m_scoreLabel->setPosition(Vec2(winSize.width*0.05, posY));
	this->addChild(m_scoreLabel, 20);

	string timeStr = convertToTimeStyle(m_time);
	string strTime = STLang::shared()->getValue("time");
	strTime += "  ";
	strTime += timeStr;
	m_timeLabel = STLabelBMFont::createWithBMFont("EurostileBold.fnt", strTime);
	//m_timeLabel = LabelTTF::create(strTime.c_str(), "arial", fontsize);
	ST_RETURN_IF(!m_timeLabel);
	m_timeLabel->setTextSize(textSize);
	float timeScale = m_timeLabel->getScale();
	labScale = min(labScale, timeScale);
	m_timeLabel->setPosition(Vec2(winSize.width*0.5, posY));
	this->addChild(m_timeLabel, 20);

	string strMove = STLang::shared()->getValue("moves");
	strMove += "  ";
	sprintf(buf, "%d", m_moveTimes);
	strMove += buf;
	m_moveLabel = STLabelBMFont::createWithBMFont("EurostileBold.fnt", strMove);
	//m_moveLabel = LabelTTF::create(strMove.c_str(), "arial", fontsize);
	ST_RETURN_IF(!m_moveLabel);
	m_moveLabel->setTextSize(textSize);
	float moveScale = m_moveLabel->getScale();
	labScale = min(labScale, moveScale);
	m_moveLabel->setAnchorPoint(Vec2(1, 0.5));
	m_moveLabel->setPosition(Vec2(winSize.width*0.95, posY));
	this->addChild(m_moveLabel, 20);

	m_scoreLabel->setScale(labScale);
	m_timeLabel->setScale(labScale);
	m_moveLabel->setScale(labScale);


	bool isPortrait = ScreenDirectionManager::share()->getScreenType() == kScreenPortrait;
	if (!isPortrait)
	{
		m_scoreLabel->setPosition(Vec2(winSize.width*0.15, posY));
		m_timeLabel->setPosition(Vec2(winSize.width*0.5, posY));
		m_moveLabel->setPosition(Vec2(winSize.width*0.85, posY));
	}
}

void GameLayer::addAutoCollectMenu()
{
	ST_RETURN_IF(!canAutoCollect());
	if (m_autoCollectMenu)
	{
		ST_SAFE_REMOVE_FROM_PARENT(m_autoCollectMenu);
	}
	Rect capRect = Rect(65, 50, 2, 12);
	Size winSize = CLDWinSize();
	Size btSize = Size(400, 100);
	Scale9Sprite *light = Scale9Sprite::create("Pic/UI-Button-LightButton-light.png");
	ST_RETURN_IF(!light);
	light->setCapInsets(capRect);
	light->setPosition(btSize.width / 2.0, btSize.height / 2.0);
	light->setContentSize(btSize);
	Scale9Sprite *normalSp = Scale9Sprite::create("Pic/UI-Button-LightButton-bg.png");
	ST_RETURN_IF(!normalSp);
	normalSp->setCapInsets(capRect);
	normalSp->addChild(light, 100);
	normalSp->setContentSize(btSize);
	Scale9Sprite *selectSp = Scale9Sprite::create("Pic/UI-Button-LightButton-bg.png");
	ST_RETURN_IF(!selectSp);
	selectSp->setCapInsets(capRect);
	selectSp->setContentSize(btSize);

	MenuItemSprite *item = MenuItemSprite::create(normalSp, selectSp, NULL, this, menu_selector(GameLayer::menuAutoCollectCallBack));
	ST_RETURN_IF(!item);
	m_autoCollectMenu = Menu::createWithItem(item);
	m_autoCollectMenu->setPosition(CLDWinPos(0.5, 0.3));
	this->addChild(m_autoCollectMenu, 10);

	CCFadeIn *fadeIn = CCFadeIn::create(1.0f);
	ST_RETURN_IF(!fadeIn);
	CCFadeOut *fadeO = CCFadeOut::create(1.0f);
	ST_RETURN_IF(!fadeO);
	Sequence *seq = Sequence::create(fadeO, fadeIn, NULL);
	ST_RETURN_IF(!seq);
	RepeatForever *repeat = RepeatForever::create(seq);
	ST_RETURN_IF(!repeat);
	light->runAction(repeat);

	string labelStr = STLang::shared()->getValue("AUTO COMPLETE!");
	STLabelBMFont *label = STLabelBMFont::createWithBMFont("EurostileBold.fnt", labelStr.c_str());
	ST_RETURN_IF(!label);
	label->setTextSize(Size(btSize.width*0.8, btSize.height*0.6));
	label->setPosition(btSize.width / 2.0, btSize.height / 2.0);
	float labSc = label->getScale();
	if (labSc > 0.6f)
	{
		label->setScale(0.6f);
	}
	else if (labSc < 0.5f)
	{
		label->setScale(0.5f);
	}
	label->setColor(Color3B::YELLOW);
	item->addChild(label, 10);

}

void GameLayer::initGameBg()
{
	if (m_bg)
	{
		m_bg->removeFromParent();
	}
	int gameBgID = SkinManager::share()->getGameBgID();
	string bgName = SkinManager::share()->getGameBgFileNameByGameBgId(gameBgID);
	m_bg = Sprite::create(bgName);
	ST_RETURN_IF(!m_bg);
	m_bg->setPosition(CLDWinCenter());
	this->addChild(m_bg, 0);
	Size bgSize = m_bg->getContentSize();
	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		float bgWidth = bgSize.width;
		float bgHeight = bgSize.height;
		bgSize.width = bgHeight;
		bgSize.height = bgWidth;
		//m_bg->setRotation(90);
	}
	/*
	Size winSize = CLDWinSize();
	float scaleX = winSize.width / bgSize.width;
	float scaleY = winSize.height / bgSize.height;
	float maxScale = (scaleY > scaleX) ? scaleY : scaleX;
	m_bg->setScale(maxScale);*/
	fixGameBg();
}

void GameLayer::fixGameBg()
{
	int gameBgID = SkinManager::share()->getGameBgID();
	string bgName = SkinManager::share()->getGameBgFileNameByGameBgId(gameBgID);
	Sprite *bgTest = Sprite::create(bgName);
	ST_RETURN_IF(!bgTest);
	Size bgSize = m_bg->getContentSize();

	Size winSize = CLDWinSize();
	float scaleX = winSize.width / bgSize.width;
	float scaleY = winSize.height / bgSize.height;
	float maxScale = (scaleY > scaleX) ? scaleY : scaleX;
	m_bg->setScale(maxScale);
}

void GameLayer::initEachArea()
{
	m_uiCollectArea = UICollectCardArea::create(m_cardDataManager.getCollectCardArea());
	ST_RETURN_IF(!m_uiCollectArea);
	this->addChild(m_uiCollectArea, 1);

	m_uiHandArea = UIHandCardArea::create(m_cardDataManager.getHandCardArea());
	ST_RETURN_IF(!m_uiHandArea);
	this->addChild(m_uiHandArea, 1);

	m_uiPlaceArea = UIPlaceCardArea::create(m_cardDataManager.getPlaceCardArea());
	ST_RETURN_IF(!m_uiPlaceArea);
	this->addChild(m_uiPlaceArea, 1);

	m_tipManager = TipManager::create(m_uiCollectArea->getData(), m_uiHandArea->getData(), m_uiPlaceArea->getData());
	ST_RETURN_IF(!m_tipManager);

	if (m_isLeftHandType)
	{
		msgSwitchHandTypeCallBack((Ref*)m_isLeftHandType);
	}
	if (m_bottomMenu)
	{
		m_bottomMenu->removePlayEffect();
	}
	bakAllCards();

} 

void GameLayer::addTouchListerner()
{
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	ST_RETURN_IF(!listener);
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
	bool bl = m_uiCollectArea && m_uiPlaceArea && m_uiHandArea;
	m_autoTipTime = 0;
	ST_RETURN_FALSE_IF(!m_touchAble);
	Vec2 locat = touch->getLocation();// / CLDScreenFixScale();
	m_touchBeganPoint = locat;
	m_isDrag = false;
	Vec2 worldPos = convertToWorldSpace(locat);
	if (bl && ((m_transportQue = m_uiHandArea->search(worldPos)) ||
		(m_transportQue = m_uiPlaceArea->search(worldPos)) || (m_transportQue = m_uiCollectArea->search(worldPos))))
	{
		log("xxOntuchbegin,in");
		m_touchAble = false;               //点击过程中不可再次点击
		Vec2 pos = m_transportQue->getPosition();
		pos = convertToNodeSpace(pos);
		m_transportQue->setPosition(pos);
		this->addChild(m_transportQue, 1000);
		return true;
	}
	log("xxOntuchbegin,out");
	m_bottomMenu->switchShowAndHide();
	return false;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
	ST_RETURN_IF(m_transportQue->getResArea() == HAND_CLOSE_AREA);
	log("xxxonTouchMoved11");
	Vec2 locat = touch->getLocation();// / CLDScreenFixScale();
	m_touchMovePoint = locat;
	float dis = m_touchBeganPoint.getDistance(m_touchMovePoint);
	if (dis > 20.0)
	{
		m_isDrag = true;
		m_transportQue->setPosition(locat);
	}
	log("xxxonTouchMoved22");
}

void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
	log("xxxonTouchEnded");
	Vec2 locat = touch->getLocation();// / CLDScreenFixScale();
	Vec2 worldPos = convertToWorldSpace(locat);
	log("xxxonTouchEndedxx11");
	if (m_transportQue->getResArea() == HAND_CLOSE_AREA && m_uiHandArea->inColosedArea(worldPos))
	{
		///翻牌
		flipCard();
	}
	else if (m_isDrag)
	{
		///拖动放置  
		dragEndWithPos(worldPos);
	}
	else
	{
		///自动放置
		autoMove();
	}
	log("xxxonTouchEndedxx2");
}


bool GameLayer::canAutoCollect()
{
	bool isHandOneOpen = m_uiHandArea->canAutoCollect();
	bool isPlaceEmpt = m_uiPlaceArea->isEmpty();
	bool isAllOpen = m_uiPlaceArea->isAllCardsOpen();
	return isHandOneOpen && isAllOpen && !isPlaceEmpt;
}

void GameLayer::menuAutoCollectCallBack(Ref *pSender)
{
	bool canAuto = canAutoCollect();
	ST_RETURN_IF(!canAuto);
	autoCollect();
	ST_SAFE_REMOVE_FROM_PARENT(m_autoCollectMenu);
}

void GameLayer::autoCollect()
{
	m_uiPlaceArea->removeAllTipLight();
	Vec2 beginWorldPos = Vec2::ZERO;
	Vec2 endWorldPos = Vec2::ZERO;
	TipInfo tipInfo = m_tipManager->newTip();
	CardArea resArea = tipInfo.m_resArea;
	int resIdx = tipInfo.m_resQueIdx;
	CardArea desArea = tipInfo.m_desArea;
	int desIdx = tipInfo.m_desQueIdx;
	vector<Card> cards;
	if (resArea == PLACE_AREA)
	{
		beginWorldPos = m_uiPlaceArea->getTheLastCardPosOfTheQue(resIdx);
		cards = m_uiPlaceArea->removeTheLastNumCardsOfQue(resIdx, 1);
	}
	else if (resArea == HAND_OPEN_AREA)
	{
		beginWorldPos = m_uiHandArea->getOpenAreaLastCardPosition();
		cards = m_uiHandArea->removeTheLastNumCardsOfOpenQue(1);
	}
	else
	{
		return;
	}
	/*
	Card card = m_uiCollectArea->getTheNextCollectCard();
	ST_RETURN_IF(card.isInvalidCard());
	int desIdx = m_uiCollectArea->canCollectTheCard(card);
	ST_RETURN_IF(desIdx == -1);

	int resIdx = m_uiPlaceArea->searchTheCard(card);
	ST_RETURN_IF(resIdx == -1);

	Vec2 beginWorldPos = m_uiPlaceArea->getTheLastCardPosOfTheQue(resIdx);
	Vec2 endWorldPos = m_uiCollectArea->getNextPosition(desIdx);*/
	endWorldPos = m_uiCollectArea->getNextPosition(desIdx);
	Vec2 beginPos = convertToNodeSpace(beginWorldPos);
	Vec2 endPos = convertToNodeSpace(endWorldPos);
	//vector<Card> cards = m_uiPlaceArea->removeTheLastNumCardsOfQue(resIdx, 1);
	TransportCardQueue *que = TransportCardQueue::create(cards, PLACE_AREA, resIdx);
	ST_RETURN_IF(!que);
	que->setPosition(beginPos);
	que->setDestArea(COLLECT_AREA);
	que->setDestQueIndex(desIdx);
	this->addChild(que, 100);

	m_touchAble = false;       //不可点击或者回退
	m_tipping = true;          //不可提示
	m_isAutoCollecting = true;
	float dt = 0.15f;
	MoveTo *mv = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mv);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(GameLayer::autoCollectEndCallFuncN));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mv, callFunc, NULL);
	ST_RETURN_IF(!seq);
	que->runAction(seq);
}

void GameLayer::autoCollectEndCallFuncN(Node *node)
{
	//移动到这
	TransportCardQueue *que = dynamic_cast<TransportCardQueue*>(node);
	ST_RETURN_IF(!que);
	CardArea area = que->getDestArea();
	int queIdx = que->getDestQueIndex();
	CardQueue queData = que->getQueData();

	m_uiCollectArea->addCardsAtIndex(queData, queIdx, true);;
	que->removeFromParent();
	m_score += Score_Collect_Move_Success;
	char buf[64];
	string strScore = STLang::shared()->getValue("score");
	strScore += "  ";
	sprintf(buf, "%d", m_score);
	strScore += buf;
	strScore = STLabelBMFont::convertFirstLetterToCapital(strScore);
	m_scoreLabel->setString(strScore.c_str());


	m_moveTimes++;
	string strMove = STLang::shared()->getValue("moves");
	strMove += "  ";
	sprintf(buf, "%d", m_moveTimes);
	strMove += buf;
	strMove = STLabelBMFont::convertFirstLetterToCapital(strMove);
	m_moveLabel->setString(strMove.c_str());
	if (isWin())
	{
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f),
			CallFunc::create(this, callfunc_selector(GameLayer::autoCollectOverCallFunc))));
	}
	else
	{
		autoCollect();
		ClientCtrl::share()->playSound(SOUND_COLLECT_CARD);
	}
}

void GameLayer::autoCollectOverCallFunc()
{
	win();
}

void GameLayer::flipCard()
{
	bool empty = m_uiHandArea->isEmpty();
	if (empty)
	{
		m_touchAble = true;
		return;
	}
	m_moveTimes++;
	UserDefault::getInstance()->setIntegerForKey(strMovesOfLastGame, m_moveTimes);
	char buf[64];
	string strMove = STLang::shared()->getValue("moves");
	strMove += "  ";
	sprintf(buf, "%d", m_moveTimes);
	strMove += buf;
	strMove = STLabelBMFont::convertFirstLetterToCapital(strMove);
	m_moveLabel->setString(strMove.c_str());
	m_canTime = true;
	m_uiHandArea->flipCard();
}

void GameLayer::dragEndWithPos(Vec2 endTouchWorldPos)
{
	//将目的地暂时初始为始发地
	m_transportQue->setDestArea(m_transportQue->getResArea());
	m_transportQue->setDestQueIndex(m_transportQue->getResQueIndex());

	///检测收牌区
	int collectQueIdx = m_uiCollectArea->isContain(endTouchWorldPos);
	if (collectQueIdx != -1)
	{
		bool canCollect = m_uiCollectArea->canCollectTheCard(m_transportQue, collectQueIdx);
		if (canCollect)
		{
			m_transportQue->setDestArea(COLLECT_AREA);
			m_transportQue->setDestQueIndex(collectQueIdx);
		}
	}
	else
	{
		///检测置牌区
		int placeQueIdx = m_uiPlaceArea->getQueIndexByPos(endTouchWorldPos);
		log("gameLayerdrageendxx, placeQueIdx:%d", placeQueIdx);
		if (placeQueIdx >= 0 && placeQueIdx < Place_Area_Num)
		{
			log("gameLayerdrageendisnot-1, placeQueIdx:%d", placeQueIdx);
			bool touchInLast = m_uiPlaceArea->isInTheLastCardOfTheQue(placeQueIdx, endTouchWorldPos);
			bool canPlace = m_uiPlaceArea->canPlaceTheCard(m_transportQue, placeQueIdx);
			if (touchInLast && canPlace)
			{
				m_transportQue->setDestArea(PLACE_AREA);
				m_transportQue->setDestQueIndex(placeQueIdx);
			}
		}
	}
	
	log("GameLayer::dragEndWithPos,line1");
	transportMove(endTouchWorldPos);
}

void GameLayer::autoMove()
{
	//将目的地暂时初始为始发地
	m_transportQue->setDestArea(m_transportQue->getResArea());
	m_transportQue->setDestQueIndex(m_transportQue->getResQueIndex());

	///检测收牌区
	int collectQueIdx = m_uiCollectArea->canCollectTheCard(m_transportQue);
	if (collectQueIdx != -1)
	{
		m_transportQue->setDestArea(COLLECT_AREA);
		m_transportQue->setDestQueIndex(collectQueIdx);
	}
	else
	{
		///检测置牌区
		int placeQueIdx = m_uiPlaceArea->canPlaceTheCard(m_transportQue);
		if (placeQueIdx != -1)
		{
			m_transportQue->setDestArea(PLACE_AREA);
			m_transportQue->setDestQueIndex(placeQueIdx);
		}
	}

	transportMove();
}

void GameLayer::transportMove(Vec2 beginWorldPos)
{
	ST_RETURN_IF(!m_transportQue);
	bool mvSuccess = isMoveSuccess();
	if (!mvSuccess)
	{
		m_transportQue->runAction(STShock::create(Move_And_Untouchable_Time));
		ClientCtrl::share()->playSound(SOUND_MOVE_NO_EFFECT);
	}
	else
	{
		ClientCtrl::share()->playSound(SOUND_PLACE_CARD);
	}
	
	Vec2 posWorld = getTransportEndPosition();
	float distanc = posWorld.distance(beginWorldPos);
	float dt = Move_And_Untouchable_Time;//distanc / Transport_Move_Speed;
	if (m_isDrag)
	{
		dt = Move_And_Untouchable_Time / 4.0;
	}
	Vec2 destPos = convertToNodeSpace(posWorld);
	MoveTo *mv = MoveTo::create(dt, destPos);
	ST_RETURN_IF(!mv);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(GameLayer::transportArrivedDestination));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mv, callFunc, NULL);
	ST_RETURN_IF(!seq);
	m_transportQue->runAction(seq);
}

void GameLayer::transportShockOfFailMove()
{
	STShock *sk = STShock::create(Move_And_Untouchable_Time);
	ST_RETURN_IF(!sk);
	m_transportQue->runAction(sk);
}

void GameLayer::transportArrivedDestination()
{
	log("transportArrivedDestinationff");
	m_touchAble = true;
	CardArea area = m_transportQue->getDestArea();
	int queIdx = m_transportQue->getDestQueIndex();
	CardQueue queData = m_transportQue->getQueData();
	CardArea resArea = m_transportQue->getResArea();
	int resQueIdx = m_transportQue->getResQueIndex();
	
	
	if (isMoveSuccess())
	{
		if (resArea == PLACE_AREA)
		{
			m_uiPlaceArea->tryToScaleTheQue(resQueIdx);
		}
		m_canTime = true;
		m_moveTimes++;
		UserDefault::getInstance()->setIntegerForKey(strMovesOfLastGame, m_moveTimes);
		char buf[64];
		string strMove = STLang::shared()->getValue("moves");
		strMove += "  ";
		sprintf(buf, "%d", m_moveTimes);
		strMove += buf;
		strMove = STLabelBMFont::convertFirstLetterToCapital(strMove);
		m_moveLabel->setString(strMove.c_str());
		bool isOpen = resQueueActionWhenMoveSuccess();
		int offScore = changeScoreWhenMoveSuccess();

		//记录回退信息
		UndoInfo info;
		info.m_resArea = m_transportQue->getResArea();
		info.m_resQueIdx = m_transportQue->getResQueIndex();
		info.m_desArea = m_transportQue->getDestArea();;
		info.m_desQueIdx = m_transportQue->getDestQueIndex();
		info.m_number = m_transportQue->size();
		info.m_scoreOff = offScore;
		info.m_opened = isOpen;
		UndoManager::share()->pushBack(info);

		//判断是否添加自动收牌按钮
		if (canAutoCollect() && !m_autoCollectMenu)
		{
			addAutoCollectMenu();
		}
	}

	if (area == COLLECT_AREA)
	{
		bool isSuc = isMoveSuccess();
		m_uiCollectArea->addCardsAtIndex(queData, queIdx, isSuc);
		if (isWin())
		{
			win();
		}
		else if (resArea != COLLECT_AREA)
		{
			ClientCtrl::share()->playSound(SOUND_COLLECT_CARD);
		}
	}
	else if (area == PLACE_AREA)
	{
		m_uiPlaceArea->addCardsAtIndex(queData, queIdx);
	}
	else if (area == HAND_OPEN_AREA)
	{
		m_uiHandArea->addCardsAtOpenArea(queData);
	}
	else
	{
		//m_uiHandArea->addCardsAtClosedArea(queData);
	}
	bakAllCards();
	//m_transportQue->removeFromParent();
	log("transportArrivedDestinationff1111");
	ST_SAFE_REMOVE_FROM_PARENT(m_transportQue);
	log("transportArrivedDestinationff22222");
}

void GameLayer::resQueueActionByUndoInfo(UndoInfo info)
{
	CardArea resArea = info.m_resArea;
	int resQueIdx = info.m_resQueIdx;
	CardArea desArea = info.m_desArea;
	int desQueIdx = info.m_desQueIdx;
	int num = info.m_number;
	int scoreOff = info.m_scoreOff;
	bool isOpen = info.m_opened;

	if (resArea == HAND_OPEN_AREA)
	{
		m_uiHandArea->openAreaAddCardEffectWithNum(num);
	}
	else if (resArea == HAND_CLOSE_AREA)
	{
		m_uiHandArea->openAreaRemoveCardEffectWithNum(num);
	}
	else if (resArea == PLACE_AREA && isOpen)
	{
		m_uiPlaceArea->tryToCloseTheLastCardOfTheQue(resQueIdx);
	}
}

bool GameLayer::resQueueActionWhenMoveSuccess()
{
	CardArea resArea = m_transportQue->getResArea();
	int resIdx = m_transportQue->getResQueIndex();
	if (resArea == HAND_OPEN_AREA)
	{
		int num = m_transportQue->size();
		m_uiHandArea->openAreaRemoveCardEffectWithNum(num);
	}
	else if (resArea == PLACE_AREA)
	{
		return m_uiPlaceArea->tryToOpenTheLastCardOfTheQue(resIdx);
	}
	return false;
}

Vec2 GameLayer::getTransportEndPosition()
{
	CardArea area = m_transportQue->getDestArea();
	int queIdx = m_transportQue->getDestQueIndex();
	return getNextPostionOfAreaOfQueue(area, queIdx);
}

string GameLayer::convertToTimeStyle(int sec)
{
	string re = "";
	char buf[32];
	int second = sec % 60;
	sec /= 60;
	int minute = sec % 60;
	sec /= 60;
	int hour = sec;
	if (hour > 0)
	{
		sprintf(buf, "%02d:", hour);
		re += buf;
	}
	sprintf(buf, "%02d:%02d", minute, second);
	re += buf;
	return re;
}

bool GameLayer::isMoveSuccess()
{
	CardArea resArea = m_transportQue->getResArea();
	int resIdx = m_transportQue->getResQueIndex();
	CardArea destArea = m_transportQue->getDestArea();
	int destIdx = m_transportQue->getDestQueIndex();
	return (!((resArea == destArea) && (resIdx == destIdx)));
}

bool GameLayer::isMoveEffectly()
{
	ST_RETURN_FALSE_IF(!isMoveSuccess());
	CardArea resArea = m_transportQue->getResArea();
	int resIdx = m_transportQue->getResQueIndex();
	CardArea destArea = m_transportQue->getDestArea();
	int destIdx = m_transportQue->getDestQueIndex();
	if (resArea != destArea){ return true; }
	ST_RETURN_FALSE_IF(resArea == COLLECT_AREA && destArea == COLLECT_AREA);
	bool isPlaceEffectly = m_uiPlaceArea->isMoveEffectly(resIdx, destIdx);
	return isPlaceEffectly;
}

void GameLayer::changeScoreByUndoInfo(UndoInfo info)
{
	int scoreOff = info.m_scoreOff;
	m_score -= scoreOff;
	char buf[64];
	string strScore = STLang::shared()->getValue("score");
	strScore += "  ";
	sprintf(buf, "%d", m_score);
	strScore += buf;
	strScore = STLabelBMFont::convertFirstLetterToCapital(strScore);
	m_scoreLabel->setString(strScore.c_str());
	UserDefault::getInstance()->setIntegerForKey(strScoreOfLastGame, m_score);
}

int GameLayer::changeScoreWhenMoveSuccess()
{
	CardArea resArea = m_transportQue->getResArea();
	int resIdx = m_transportQue->getResQueIndex();
	CardArea destArea = m_transportQue->getDestArea();
	int destIdx = m_transportQue->getDestQueIndex();
	if (!isMoveEffectly()){ return 0; }
	int offScore = 0;
	if (destArea == COLLECT_AREA)
	{
		offScore = Score_Collect_Move_Success;
	}
	else if (resArea == COLLECT_AREA && destArea != COLLECT_AREA)
	{
		offScore = -Score_Collect_Move_Success;
	}
	else if (destArea == HAND_OPEN_AREA)
	{
		offScore = -Score_Not_Collect_Move_Success;
	}
	else
	{
		offScore = Score_Not_Collect_Move_Success;
	}
	m_score += offScore;
	char buf[64];
	string strScore = STLang::shared()->getValue("score");
	strScore += "  ";
	sprintf(buf, "%d", m_score);
	strScore += buf;
	strScore = STLabelBMFont::convertFirstLetterToCapital(strScore);
	m_scoreLabel->setString(strScore.c_str());
	UserDefault::getInstance()->setIntegerForKey(strScoreOfLastGame, m_score);
	return offScore;
}

void GameLayer::msgSetTouchEnableCallBack(Ref *ref)
{
	m_touchAble = (bool)(ref);
	bakAllCards();
	if (!m_autoCollectMenu)
	{
		addAutoCollectMenu();
	}
}
void GameLayer::msgShowPlayBoardCallBack(Ref *ref)
{
	ST_RETURN_IF(m_isAutoCollecting || !m_touchAble);
	bool canReplay = AllAreaIsEmpty();
	m_bottomMenu->tryToShowPlayBoard(!canReplay);
	/*
	ST_RETURN_IF(m_isAutoCollecting || !m_touchAble);
	Vec2 bottomPos = m_bottomMenu->getPosition();
	Size bottomSize = m_bottomMenu->getBgSize();
	bool allEmp = AllAreaIsEmpty();
	PlayBoard *board = PlayBoard::create(!allEmp);
	ST_RETURN_IF(!board);
	Size boardSize = board->getBgSize();
	Size winSize = CLDWinSize();
	float posY = bottomPos.y + boardSize.height*0.33;
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (!isPortrait)
	{
		posY = bottomPos.y + boardSize.height*0.2;
	}
	board->setPosition(Vec2(bottomPos.x, posY));
	
	board->setTag(ST_PLAY_BOARD_TAG);
	board->appearEffect();
	this->addChild(board, 100);

	ClientCtrl::share()->reportEventParam("ClickPlayButton");
	ClientCtrl::share()->playSound(SOUND_MENU_IN);
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		ClientCtrl::share()->tryToShowBanner();
	}
	*/
}

void GameLayer::showPlayBoardWithOutEffect()
{/*
	Vec2 bottomPos = m_bottomMenu->getPosition();
	bool allEmp = AllAreaIsEmpty();
	PlayBoard *board = PlayBoard::create(!allEmp);
	ST_RETURN_IF(!board);
	Size boardSize = board->getBgSize();
	Size winSize = CLDWinSize();
	float posY = bottomPos.y + boardSize.height*0.33;
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (!isPortrait)
	{
		posY = bottomPos.y + boardSize.height*0.2;
	}
	board->setPosition(Vec2(bottomPos.x, posY));
	board->setTag(ST_PLAY_BOARD_TAG);
	board->appearEffect(false);
	this->addChild(board, 100); 
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		ClientCtrl::share()->tryToShowBanner();
	}*/
}

void GameLayer::msgTipCallBack(Ref *ref)
{
	m_autoTipTime = 0;
	newTip();
}

void GameLayer::newTip(bool isManual)
{
	ST_RETURN_IF(m_isAutoCollecting);
	ST_RETURN_IF(!m_touchAble);
	ST_RETURN_IF(m_tipping);
	TipInfo tipInfo = m_tipManager->newTip();
	ST_RETURN_IF(TipManager::isInvalidTipInfo(tipInfo));

	if (isManual)
	{
		newTipWithManual(tipInfo);
	}
	else
	{
		newTipWithAuto(tipInfo);
	}

}

void GameLayer::newTipWithManual(TipInfo tipInfo)
{
	ClientCtrl::share()->reportEventParam("ClickTipButton");
	CardArea resArea = tipInfo.m_resArea;
	int resQurIdx = tipInfo.m_resQueIdx;
	int resCardIdx = tipInfo.m_resCardIdx;
	CardArea desArea = tipInfo.m_desArea;
	int desQueIdx = tipInfo.m_desQueIdx;
	vector<Card> cards;
	switch (resArea)
	{
	case HAND_CLOSE_AREA:
		m_uiHandArea->addClosedAreaTip();
		return;
		break;
	case HAND_OPEN_AREA:
	{
		CardSprite* cdSp = m_uiHandArea->getOpenAreaLastCard();
		if (cdSp)
		{
			Card cd = cdSp->getCardInfo();
			cards.push_back(cd);
		}
		break;
	}
	case PLACE_AREA:
		cards = m_uiPlaceArea->getCardsAt(resQurIdx, resCardIdx);
		break;
	case COLLECT_AREA:
	{
		CardSprite* cdSp = m_uiCollectArea->getTheLastCard(resQurIdx);
		if (cdSp)
		{
			Card cd = cdSp->getCardInfo();
			cards.push_back(cd);
		}
		break;
	}
	default:
		break;
	}
	int sz = cards.size();
	for (int i = 0; i < sz; i++)
	{
		cards[i].m_isOpen = true;
	}
	TransportCardQueue *que = TransportCardQueue::create(cards, resArea, resQurIdx);
	ST_RETURN_IF(!que);
	Vec2 beginWorldPos = getCardWorldPosition(resArea, resQurIdx, resCardIdx);
	Vec2 beginPos = this->convertToNodeSpace(beginWorldPos);
	Vec2 endWorldPos = getNextPostionOfAreaOfQueue(desArea, desQueIdx);
	Vec2 endPos = this->convertToNodeSpace(endWorldPos);
	que->setPosition(beginPos);
	que->addTipLightAfterIndex(0);
	this->addChild(que, 100);
	float dt = 0.4f;
	DelayTime *delay1 = DelayTime::create(0.05f);
	ST_RETURN_IF(!delay1);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	DelayTime *delay2 = DelayTime::create(0.05f);
	ST_RETURN_IF(!delay2);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(GameLayer::tipOverCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(delay1, mvTo, delay1, callFunc, NULL);
	ST_RETURN_IF(!seq);
	m_tipping = true;
	que->runAction(seq);
}

void GameLayer::tipOverCallFunc(Node *node)
{
	m_tipping = false;
	node->removeFromParent();
}

Vec2 GameLayer::getCardWorldPosition(CardArea area, int queIdx, int cardIdx)
{
	if (area == HAND_CLOSE_AREA)
	{
		return m_uiHandArea->getClosedAreaPosition();
	}
	else if (area == HAND_OPEN_AREA)
	{
		return m_uiHandArea->getOpenAreaLastCardPosition();
	}
	else if (area == COLLECT_AREA)
	{
		return m_uiCollectArea->getNextPosition(queIdx);
	}
	else
	{
		return m_uiPlaceArea->getCardWorldPosition(cardIdx, queIdx);
	}
}

int GameLayer::getIndexOfTheLastNumCardOfQue(CardArea area, int queIdx, int num)
{
	int sz = 0;
	if (area == HAND_CLOSE_AREA)
	{
		sz = m_uiHandArea->getClosedAreaSize();
	}
	else if (area == HAND_OPEN_AREA)
	{
		sz = m_uiHandArea->getOpenAreaSize();
	}
	else if (area == COLLECT_AREA)
	{
		sz = m_uiCollectArea->getQueSize(queIdx);
	}
	else
	{
		sz = m_uiPlaceArea->getQueSize(queIdx);
	}
	int index = sz - num;
	index = (index < 0) ? 0 : index;
	return index;
}

Vec2 GameLayer::getNextPostionOfAreaOfQueue(CardArea area, int queIdx)
{
	Vec2 rePos;
	if (area == COLLECT_AREA)
	{
		rePos = m_uiCollectArea->getNextPosition(queIdx);
	}
	else if (area == PLACE_AREA)
	{
		rePos = m_uiPlaceArea->getNextPosition(queIdx);
	}
	else if (area == HAND_OPEN_AREA)
	{
		rePos = m_uiHandArea->getNextPositionOfOpenQue();
	}
	else
	{
		rePos = m_uiHandArea->getNextPositionOfClosedQue();
	}
	return rePos;
}

void GameLayer::newTipWithAuto(TipInfo tipInfo)
{
	CardArea resArea = tipInfo.m_resArea;
	int resQurIdx = tipInfo.m_resQueIdx;
	int resCardIdx = tipInfo.m_resCardIdx;
	CardArea desArea = tipInfo.m_desArea;
	int desQueIdx = tipInfo.m_desQueIdx;
	switch (resArea)
	{
	case HAND_CLOSE_AREA:
		m_uiHandArea->addClosedAreaTip();
		return;
		break;
	case HAND_OPEN_AREA:
		m_uiHandArea->addOpenAreaTip();
		break;
	case PLACE_AREA:
		m_uiPlaceArea->addTipLightAt(resQurIdx, resCardIdx);
		break;
	default:
		break;
	}
}

void GameLayer::msgUndoCallBack(Ref *ref)
{
	ST_RETURN_IF(!m_touchAble);
	ST_RETURN_IF(m_isAutoCollecting);
	undo();
}

void GameLayer::undo()
{
	UndoInfo info = UndoManager::share()->pop();
	ST_RETURN_IF(UndoManager::share()->isInvalidUndoInfo(info));
	ClientCtrl::share()->reportEventParam("ClickUndoButton");
	m_uiPlaceArea->removeAllTipLight();
	m_autoTipTime = 0;
	m_canTime = true;
	m_moveTimes++;
	UserDefault::getInstance()->setIntegerForKey(strMovesOfLastGame, m_moveTimes);
	char buf[64];
	string strMove = STLang::shared()->getValue("moves");
	strMove += "  ";
	sprintf(buf, "%d", m_moveTimes);
	strMove += buf;
	strMove = STLabelBMFont::convertFirstLetterToCapital(strMove);
	m_moveLabel->setString(strMove.c_str());
	changeScoreByUndoInfo(info);

	int index = getIndexOfTheLastNumCardOfQue(info.m_desArea, info.m_desQueIdx, info.m_number);
	Vec2 beginWorldPos = getCardWorldPosition(info.m_desArea, info.m_desQueIdx, index);
	Vec2 endWorldPos = getNextPostionOfAreaOfQueue(info.m_resArea, info.m_resQueIdx);
	Vec2 beginPos = convertToNodeSpace(beginWorldPos);
	Vec2 endPos = convertToNodeSpace(endWorldPos);
	vector<Card> tempCards = removeTheLastNumCards(info.m_desArea, info.m_desQueIdx, info.m_number);
	vector<Card> cards;
	int tempSz = tempCards.size();
	for (int i = (tempSz - 1); i >= 0; i--)
	{
		tempCards[i].m_isOpen = true;
		cards.push_back(tempCards[i]);
	}
	TransportCardQueue *re = NULL;
	if (info.m_resArea == HAND_OPEN_AREA && info.m_desArea == HAND_CLOSE_AREA)
	{
		re = TransportCardQueue::create(tempCards, info.m_desArea);
	}
	else
	{
		re = TransportCardQueue::create(cards, info.m_desArea);
	}
	ST_RETURN_IF(!re);;
	re->setDestArea(info.m_resArea);
	re->setDestQueIndex(info.m_resQueIdx);
	re->setPosition(beginPos);
	this->addChild(re, 100);
	resQueueActionByUndoInfo(info);

	if (info.m_resArea == HAND_CLOSE_AREA)
	{
		re->setLandscapeStatus();
	}
	else if (info.m_resArea == HAND_OPEN_AREA && info.m_desArea == HAND_CLOSE_AREA)
	{
		re->setHandOpenAreaStatus();
	}

	float dt = Move_And_Untouchable_Time;
	MoveTo *mv = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mv);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(GameLayer::undoOverCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mv, callFunc, NULL);
	ST_RETURN_IF(!seq);
	m_touchAble = false;
	re->runAction(seq);

	
	ClientCtrl::share()->playSound(SOUND_UNDO);
}

void GameLayer::undoOverCallFunc(Node *node)
{
	TransportCardQueue *que = dynamic_cast<TransportCardQueue*>(node);
	ST_RETURN_IF(!que);
	m_touchAble = true;
	CardArea area = que->getDestArea();
	int queIdx = que->getDestQueIndex();
	CardQueue queData = que->getQueData();

	if (area == COLLECT_AREA)
	{
		m_uiCollectArea->addCardsAtIndex(queData, queIdx);
	}
	else if (area == PLACE_AREA)
	{
		m_uiPlaceArea->addCardsAtIndex(queData, queIdx);
	}
	else if (area == HAND_OPEN_AREA)
	{
		m_uiHandArea->addCardsAtOpenArea(queData);
	}
	else
	{
		m_uiHandArea->addCardsAtClosedArea(queData);
	}
	bakAllCards();

	if (!canAutoCollect())
	{
		ST_SAFE_REMOVE_FROM_PARENT(m_autoCollectMenu);
	}


	que->removeFromParent();
}

vector<Card> GameLayer::removeTheLastNumCards(CardArea area, int queIdx, int num)
{
	if (area == COLLECT_AREA)
	{
		return m_uiCollectArea->removeTheLastNumCardsOfQue(queIdx,num);
	}
	else if (area == HAND_OPEN_AREA)
	{
		return m_uiHandArea->removeTheLastNumCardsOfOpenQue(num);
	}
	else if (area == HAND_CLOSE_AREA)
	{
		return m_uiHandArea->removeTheLastNumCardsOfClosedQue(num);
	}
	else if (area == PLACE_AREA)
	{
		return m_uiPlaceArea->removeTheLastNumCardsOfQue(queIdx, num);
	}

}

void GameLayer::msgShowNewThemeBoardCallBack(Ref *ref)
{
	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(0.2f),
		CallFunc::create(this, callfunc_selector(GameLayer::addNewThemeTipBoardDailyCallFunc))));
}

void GameLayer::addNewThemeTipBoardDailyCallFunc()
{
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kNewThemeTipBoard);
}

void GameLayer::msgShowOptionBoardCallBack(Ref *ref)
{
	ST_RETURN_IF(m_isAutoCollecting || !m_touchAble);
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	int tp = (int)(ref);
	tryToPreGameAd(tp);
	OptionBaseBoard *board = NULL;
	switch (tp)
	{
	case kRootOptionBoard:
		board = RootOptionBoard::create();
		break;
	case kBackDropBoard:
		board = BackDropBoard::create();
		break;
	case kFrontStyleBoard:
		board = FrontStyleOptionBoard::create();
		break;
	case kBackStyleBoard:
		board = BackStyleOptionBoard::create();
		break;
	case kStatisticsBoard:
		board = StatisticsBoard::create();
		break;
	case kWinBoard:
		board = WinBoard::create();
		break;
	case kDailyChallengeBoard:
	{
		struct tm dataTm = DailyChallengeManage::share()->getDailyChallengeDefaultData();
		board = DailyChallengeBoard::create(dataTm.tm_year, dataTm.tm_mon, dataTm.tm_mday);
		break;
	}
	case kDailyChallengeHelpBoard:
		board = DailyChallengeHelpBoard::create();
		break;
	case kNetworkFailBoard:
		board = NetworkFailBoard::create();
		break;
	case kLanguageSelectBoard:
		board = LanguageSelectBoard::create();
		break;
	case kInviteDailyChallengeBoard:
		board = InviteDailyChallengeBoard::create();
		break;
	case kRateBoard:
		board = RateBoard::create();
		break;
	case kRateSuccessBoard:
		board = RateSuccessBoard::create();
		break;
	case kResetDataBoard:
		board = ResetDataBoard::create();
		break;
	case kInviteFriendBoard:
		board = InviteFriendBoard::create();
		break;
	case kInviteFriendSimpleBoard:
		board = InviteFriendSimpleBoard::create();
		break;
	case kNewThemeTipBoard:
		board = NewThemeTipBoard::create();
		break;
	case kInviteSuccessBoard:
		board = InviteSuccessBoard::create();
		break;
	default:
		break;
	}
	ST_RETURN_IF(!board);
	OptionBoardManager::share()->pushBuck(board);
	float posBeginX = -winSize.width*0.5;
	float posEndX = winSize.width*0.5;
	OptionBoardManager::share()->fixScreen(board);
	float posY = winSize.height*0.5;// cardSize.height*0.3;
	board->setPosition(Vec2(posBeginX, posY));
	this->addChild(board, 100);


	float dt = 0.3f;
	MoveTo *mvTo = MoveTo::create(dt, Vec2(posEndX, posY));
	ST_RETURN_IF(!mvTo);
	EaseBackOut *ease = EaseBackOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(GameLayer::boardShowEndCallFunc));
	ST_RETURN_IF(!callFunc);

	Sequence *seq = Sequence::create(ease, callFunc, NULL);
	ST_RETURN_IF(!seq);


	board->runAction(seq);
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		ClientCtrl::share()->tryToShowBanner();
	}
	
	ClientCtrl::share()->playSound(SOUND_MENU_IN);
}

void GameLayer::boardShowEndCallFunc(Node *node)
{
	OptionBaseBoard *board = dynamic_cast<OptionBaseBoard *>(node);
	ST_RETURN_IF(!board);

	OptionBoardType tp = board->getOptionBoardType();
	if (tp == kWinBoard)
	{
		WinBoard *winBoard = dynamic_cast<WinBoard *>(board);
		ST_RETURN_IF(!winBoard);
		winBoard->addSnowEffect();
	}
	tryToShowGameAd(tp);
}

void GameLayer::tryToPreGameAd(int tp)
{
	bool hasRate = RateBoard::isExist(); //OptionBoardManager::share()->hasTheOption(kRateBoard);
	if ((!hasRate) && (tp == kRootOptionBoard || tp == kWinBoard || tp == kDailyChallengeBoard))
	{
		bool isAutoShow = DailyChallengeBoard::getIsAutoShow();
		if (!(tp == kDailyChallengeBoard && isAutoShow))
		{
			ClientCtrl::share()->tryToPreGameAds();
		}
	}
}

void GameLayer::tryToShowGameAd(int tp)
{
	bool hasRate = RateBoard::isExist();// OptionBoardManager::share()->hasTheOption(kRateBoard);
	if ((!hasRate) && (tp == kRootOptionBoard || tp == kWinBoard || tp == kDailyChallengeBoard))
	{
		bool isAutoShow = DailyChallengeBoard::getIsAutoShow();
		if (!(tp == kDailyChallengeBoard && isAutoShow))
		{
			float dt = 0.3f;
			this->runAction(Sequence::createWithTwoActions(
				DelayTime::create(dt),
				CallFunc::create(this, callfunc_selector(GameLayer::tryToShowGameAdCallFunc))));
		}
	}
}

void GameLayer::tryToShowGameAdCallFunc()
{
	ClientCtrl::share()->tryToShowGameAds();
}

void GameLayer::msgShowOptionBoardWithOutEffectCallBack(Ref *ref)
{
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	int tp = (int)(ref);
	OptionBaseBoard *board = NULL;
	switch (tp)
	{
	case kRootOptionBoard:
		board = RootOptionBoard::create();
		break;
	case kBackDropBoard:
		board = BackDropBoard::create();
		break;
	case kFrontStyleBoard:
		board = FrontStyleOptionBoard::create();
		break;
	case kBackStyleBoard:
		board = BackStyleOptionBoard::create();
		break;
	case kStatisticsBoard:
		board = StatisticsBoard::create();
		break;
	case kWinBoard:
		board = WinBoard::create();
		break;
	case kDailyChallengeBoard:
	{
		struct tm dataTm = DailyChallengeManage::share()->getDailyChallengeDefaultData();
		board = DailyChallengeBoard::create(dataTm.tm_year, dataTm.tm_mon, dataTm.tm_mday);
		break;
	}
	case kDailyChallengeHelpBoard:
		board = DailyChallengeHelpBoard::create();
		break;
	case kNetworkFailBoard:
		board = NetworkFailBoard::create();
		break;
	case kLanguageSelectBoard:
		board = LanguageSelectBoard::create();
		break;
	case kInviteDailyChallengeBoard:
		board = InviteDailyChallengeBoard::create();
		break;
	case kRateBoard:
		board = RateBoard::create();
		break;
	case kRateSuccessBoard:
		board = RateSuccessBoard::create();
		break;
	case kResetDataBoard:
		board = ResetDataBoard::create();
		break;
	case kInviteFriendBoard:
		board = InviteFriendBoard::create();
		break;
	case kInviteFriendSimpleBoard:
		board = InviteFriendSimpleBoard::create();
		break;
	case kNewThemeTipBoard:
		board = NewThemeTipBoard::create();
		break;
	case kInviteSuccessBoard:
		board = InviteSuccessBoard::create();
		break;
	default:
		break;
	}
	ST_RETURN_IF(!board);
	float posX = winSize.width*0.5;
	OptionBoardManager::share()->fixScreen(board);
	float posY = winSize.height*0.5;// cardSize.height*0.3;
	OptionBoardManager::share()->pushBuck(board);
	board->setPosition(Vec2(posX, posY));
	this->addChild(board, 100);

	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		ClientCtrl::share()->tryToShowBanner();
	}
}


void GameLayer::resetGame()
{
	UndoManager::share()->clear();
	ST_SAFE_REMOVE_FROM_PARENT(m_uiHandArea);
	ST_SAFE_REMOVE_FROM_PARENT(m_uiPlaceArea);
	ST_SAFE_REMOVE_FROM_PARENT(m_uiCollectArea);
	ST_SAFE_REMOVE_FROM_PARENT(m_scoreLabel);
	ST_SAFE_REMOVE_FROM_PARENT(m_timeLabel);
	ST_SAFE_REMOVE_FROM_PARENT(m_moveLabel);
	ST_SAFE_REMOVE_FROM_PARENT(m_autoCollectMenu);
	m_cardDataManager.clear();
	m_score = 0;
	m_time = 0;
	m_moveTimes = 0;
	m_autoTipTime = 0;
	UserDefault::getInstance()->setIntegerForKey(strScoreOfLastGame, m_score);
	UserDefault::getInstance()->setIntegerForKey(strTimeOfLastGame, m_time);
	UserDefault::getInstance()->setIntegerForKey(strMovesOfLastGame, m_moveTimes);
	m_canTime = false;
}

void GameLayer::reportNewGameStartLevel()
{
	const char *eventId = "NormalGame";
	ClientCtrl::share()->startLevel(eventId);
}

void GameLayer::reportDailyChallengeStartLevel()
{
	string eventId = "";
	int year = DailyChallengeManage::share()->getChallengeYear();
	int mon = DailyChallengeManage::share()->getChallengeMonth();
	int day = DailyChallengeManage::share()->getChallengeDay();
	char buf[64];
	sprintf(buf, "Daily_%d_%d_%d", year, mon, day);
	eventId = buf;
	eventId = "DailyChallenge";
	ClientCtrl::share()->startLevel(eventId.c_str());
}

void GameLayer::reportEndLevel()
{
	log("xxxxGamelayerreporetend");
	bool isChallenge = UserDefault::getInstance()->getBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
	if (isChallenge)
	{
		reportDailyChallengeEndLevel();
	}
	else
	{
		reportNewGameEndLevel();
	}

}

void GameLayer::reportNewGameEndLevel()
{
	log("xxxxGamelayerreporetend,NewGame");
	string eventId = "NormalGame";

	bool allEmp = AllAreaIsEmpty();
	if (allEmp)       //win
	{
		ClientCtrl::share()->finishLevel(eventId.c_str());
	}
	else
	{
		ClientCtrl::share()->failLevel(eventId.c_str());
	}
}
void GameLayer::reportDailyChallengeEndLevel()
{
	log("xxxxGamelayerreporetend,DailyChallenge");
	string eventId = "";
	int year = DailyChallengeManage::share()->getChallengeYear();
	int mon = DailyChallengeManage::share()->getChallengeMonth();
	int day = DailyChallengeManage::share()->getChallengeDay();
	char buf[64];
	sprintf(buf, "Daily_%d_%d_%d", year, mon, day);
	eventId = buf;
	eventId = "DailyChallenge";
	bool allEmp = AllAreaIsEmpty();
	if (allEmp)       //win
	{
		ClientCtrl::share()->finishLevel(eventId.c_str());
	}
	else
	{
		ClientCtrl::share()->failLevel(eventId.c_str());
	}
}

void GameLayer::msgNewGameCallBack(Ref *ref)
{
	//统计结束状态
	reportEndLevel();

	//ClientCtrl::share()->tryToPreGameAds();
	//判断是否连胜
	int winStreakNum = UserDefault::getInstance()->getIntegerForKey(STRG_WIN_STREAK_NUM, 0);
	bool iswin = AllAreaIsEmpty();
	winStreakNum = iswin ? (winStreakNum) : 0;
	UserDefault::getInstance()->setIntegerForKey(STRG_WIN_STREAK_NUM, winStreakNum);

	resetGame();
	m_cardDataManager.setInitGameType(kNewGameInit);
	int drillNum = 0;
	do 
	{
		m_cardDataManager.init();
		//drillNum = m_cardDataManager.drill();
	} while (0);// (drillNum < 5);
	log("drillNum:%d", drillNum);

	initEachArea();
	initHeadLabel();
	addDealCardEffect();

	//开始统计
	reportNewGameStartLevel();

}

void GameLayer::msgDailyChallengeGameCallBack(Ref *ref)
{
	//统计结束状态
	reportEndLevel();

	//ClientCtrl::share()->tryToPreGameAds();
	//判断是否连胜
	int winStreakNum = UserDefault::getInstance()->getIntegerForKey(STRG_WIN_STREAK_NUM, 0);
	bool iswin = AllAreaIsEmpty();
	winStreakNum = iswin ? (winStreakNum) : 0;
	UserDefault::getInstance()->setIntegerForKey(STRG_WIN_STREAK_NUM, winStreakNum);

	resetGame();
	m_cardDataManager.setInitGameType(kChallengeInit);
	m_cardDataManager.init();
	initEachArea();
	initHeadLabel();
	addDealCardEffect();

	//开始统计
	reportDailyChallengeStartLevel();
}

void GameLayer::msgReplayCallBack(Ref *ref)
{
	//ClientCtrl::share()->tryToPreGameAds();
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum == 0);
	ClientCtrl::share()->reportEventParam("ClickReplayButton");
	resetGame();
	m_cardDataManager.setInitGameType(kReplayInit);
	m_cardDataManager.init();
	initEachArea();
	initHeadLabel();
	addDealCardEffect();
}

void GameLayer::msgUpdateUICallBack(Ref *ref)
{
	initGameBg();
	updateCard();
}

void GameLayer::msgConnectNetworkShowDailyChallengeCallBack(Ref *ref)
{
	ST_RETURN_IF(m_isAutoCollecting);
	string cfgStr = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_DAILY_CHALLENGE_PARAM);
	string value = ClientCtrl::share()->parseUMengOnlineConfig(cfgStr, "server_time");
	log("configNettime,cfgStr:%s,value:%s", cfgStr.c_str(),value.c_str());
	bool isNetTime = true;
	if (value == "0")
	{
		isNetTime = false;
	}

	if (!isNetTime)      //本地时间
	{
		time_t curTm = STTimeManager::share()->getLocalTime();
		STTimeManager::share()->setCurrentTime(curTm);
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kDailyChallengeBoard));
	//emoveSeparateTouch();
	}
	else
	{
		addSeparateTouch();
		NetTime * netTime = NetTime::create();
		ST_RETURN_IF(!netTime);
		netTime->setPosition(CLDWinCenter());
		netTime->requestNetTime(this, netTime_selector(GameLayer::onNetSuccessComplete), netTime_selector(GameLayer::onNetFailComplete));
		this->addChild(netTime, 10000);
	}

	/*
#define ST_GET_TIME     3
#if (ST_GET_TIME == 1)       
	string dataStr = ClientCtrl::share()->getNetData();
	if (dataStr == "0-0-0")
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kNetworkFailBoard));
	}
	else
	{
		time_t ttt = STTimeManager::share()->setCurrentTime(dataStr.c_str());
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kDailyChallengeBoard));
	}
	removeSeparateTouch();
#elif (ST_GET_TIME == 2)    //本地时间
	time_t curTm = STTimeManager::share()->getLocalTime();
	STTimeManager::share()->setCurrentTime(curTm);
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kDailyChallengeBoard));
	removeSeparateTouch();
#elif (ST_GET_TIME == 3)  //网络获取时间
	NetTime * netTime = NetTime::create();
	ST_RETURN_IF(!netTime);
	netTime->setPosition(CLDWinCenter());
	netTime->requestNetTime(this, netTime_selector(GameLayer::onNetSuccessComplete), netTime_selector(GameLayer::onNetFailComplete));
	this->addChild(netTime, 10000);
#endif
	*/
}

void GameLayer::onNetSuccessComplete(NetTime * pSender)
{
	STTimeManager::share()->setCurrentTime(pSender->getTime());

	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kDailyChallengeBoard));
	ST_SAFE_REMOVE_FROM_PARENT(pSender);

	removeSeparateTouch();
}


void GameLayer::onNetFailComplete(NetTime * pSender)
{
	//联网失败显示联网失败面板
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kNetworkFailBoard));
	ST_SAFE_REMOVE_FROM_PARENT(pSender);
	//this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f),
	//CallFunc::create(this, callfunc_selector(GameLayer::removeSeparateTouch))));
	removeSeparateTouch();
}

void GameLayer::msgUpdateLanguageCallBack(Ref *ref)
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	Size fontSize = Size(winSize.width*0.25, cardSize.height*0.28);

	//m_scoreLabel
	char buf[64];
	string strScore = STLang::shared()->getValue("score");
	strScore += "  ";
	sprintf(buf, "%d", m_score);
	strScore += buf;
	strScore = STLabelBMFont::convertFirstLetterToCapital(strScore);
	m_scoreLabel->setString(strScore.c_str());
	m_scoreLabel->setTextSize(fontSize);

	//m_moveLabel
	string strMove = STLang::shared()->getValue("moves");
	strMove += "  ";
	sprintf(buf, "%d", m_moveTimes);
	strMove += buf;
	strMove = STLabelBMFont::convertFirstLetterToCapital(strMove);
	m_moveLabel->setString(strMove.c_str());
	m_moveLabel->setTextSize(fontSize);

	//m_timeLabel
	string timeStr = convertToTimeStyle(m_time);
	string strTime = STLang::shared()->getValue("time");
	strTime += "  ";
	strTime += timeStr;
	strTime = STLabelBMFont::convertFirstLetterToCapital(strTime);
	m_timeLabel->setString(strTime.c_str());
	m_timeLabel->setTextSize(fontSize);

	m_bottomMenu->updateLanguage();

	addAutoCollectMenu();
}

void GameLayer::msgGameEnterBackGroundCallBack(Ref *ref)
{
	bool isEmp = AllAreaIsEmpty();
	if (!isEmp)
	{
		NotificationManage::share()->tryToPostNoWin();
	}
}

void GameLayer::msgExitGameCallBack(Ref *ref)
{
	msgGameEnterBackGroundCallBack(NULL);
	Director::getInstance()->end();
	ClientCtrl::share()->umengEnd();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameLayer::msgClickAndroidReturnButtonCallBack(Ref *ref)
{
	ST_RETURN_IF(!m_canClickReturnBt);
	m_canClickReturnBt = false;
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f),
		CallFunc::create(this, callfunc_selector(GameLayer::clickAndroidReturnButtonDelayCallFunc))));
	
}

void GameLayer::clickAndroidReturnButtonDelayCallFunc()
{
	bool bl = tryToRemoveOptionBoard();
	if (!bl)
	{
		bool bl2 = tryToRemovePlayBoard();
		if (!bl2)
		{
			log("xxx exitgame1");
			ClientCtrl::share()->showExitBoard();
			ClientCtrl::share()->reportEventParam("AdsOnExitGame");
			msgGameEnterBackGroundCallBack(NULL);
			log("xxx exitgame2");
			//NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kExitGameBoard);
		}
		
	}
	m_canClickReturnBt = true;
}

bool GameLayer::tryToRemovePlayBoard()
{
	ST_RETURN_FALSE_IF(!m_bottomMenu);
	return m_bottomMenu->tryToHidePlayBoard();
}

bool GameLayer::tryToRemoveOptionBoard()
{
	OptionBaseBoard *lastBoard = OptionBoardManager::share()->getTheLastBoard();
	if (lastBoard)
	{
		lastBoard->closeEffect();
		return true;
	}
	return false;
}

void GameLayer::addSeparateTouch()
{
	Node * spaceNode = Node::create();
	ST_RETURN_IF(!spaceNode);
	spaceNode->setContentSize(CCSizeMake(CLDWinSize().width * 3, CLDWinSize().height * 3));
	MenuItemSprite * menuSeparate = MenuItemSprite::create(
		spaceNode,
		spaceNode);
	ST_RETURN_IF(!menuSeparate);

	Menu *seperateMenu = Menu::createWithItem(menuSeparate);
	ST_RETURN_IF(!seperateMenu);
	seperateMenu->setTag(Seperate_layer_tag);
	seperateMenu->setPosition(CLDWinCenter());
	STHelper::addDarkLayer(spaceNode);
	this->addChild(seperateMenu,10);
	
	Node* node = CSLoader::createNode("Node_hourglass.csb");
	auto action = CSLoader::createTimeline("Node_hourglass.csb");
	node->runAction(action);
	node->setPosition(CLDWinCenter());
	node->setTag(Net_request_effect);
	action->gotoFrameAndPlay(0, 100, true);
	this->addChild(node, 100);
}
void GameLayer::removeSeparateTouch()
{
	Node *node = getChildByTag(Seperate_layer_tag);
	ST_SAFE_REMOVE_FROM_PARENT(node);
	Node *node2 = getChildByTag(Net_request_effect);
	ST_SAFE_REMOVE_FROM_PARENT(node2);
}



void GameLayer::updateCard()
{
	m_uiCollectArea->updateCard();
	m_uiHandArea->updateCard();
	m_uiPlaceArea->updateCard();
}


void GameLayer::bakAllCards()
{
	//ST_RETURN_IF(m_cardDataManager.getInitGameType() == kChallengeInit);
	string re = "";
	//json开头
	const char *jsonBeginStr = "{\n";
	re += jsonBeginStr;
	if (m_uiCollectArea && m_uiPlaceArea && m_uiHandArea)
	{
		//收牌区
		string collectStr = m_uiCollectArea->bakAllCards();
		re += collectStr;

		//手牌区
		string handStr = m_uiHandArea->bakAllCards();
		re += handStr;

		//置牌区
		string placeStr = m_uiPlaceArea->bakAllCards();
		re += placeStr;
	}

	//json结尾
	const char *jsonEndStr = "\n}";
	re += jsonEndStr;
	if (1)//(m_cardDataManager.getInitGameType() != kChallengeInit)
	{
		IOManager::share()->write(re, "commonBak.json", "w");       //挑战局不存储
	}
	
}

void GameLayer::tryToShowInvitePlayDailyBoard()
{
	bool isReceved = UserDefault::getInstance()->getBoolForKey(STRG_IS_NOTIFY_CHALLENGE, true); 
	time_t localTime = STTimeManager::share()->getLocalTime();
	struct tm *today = localtime(&localTime);
	char buf[64];
	sprintf(buf, "Play_Num_Of_%d_%d_%d", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday);
	int playNumToday = UserDefault::getInstance()->getIntegerForKey(buf, 0);
	int totalPlayNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(buf, playNumToday + 1);
	bool bl1 = (totalPlayNum > 0);
	bool bl2 = (playNumToday == 0);
	bool bl3 = AllAreaIsEmpty();
	if (isReceved && bl1 && bl2)
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kInviteDailyChallengeBoard);
	}
}

bool GameLayer::AllAreaIsEmpty()
{
	bool bl1  = m_uiPlaceArea->isEmpty();
	bool bl2 = m_uiCollectArea->isEmpty();
	bool bl3 = m_uiHandArea->isEmpty();
	return bl1 && bl2 && bl3;
}

bool GameLayer::isWin()
{
	bool b1 = m_uiHandArea->isEmpty();
	bool b2 = m_uiPlaceArea->isEmpty();
	bool b3 = (m_uiCollectArea->getAllCardSize() == 52);
	return (b1 && b2 && b3);
}

void GameLayer::stTest_win()
{
	win();
}

void GameLayer::win()
{
	ClientCtrl::share()->submitScore(m_score);
	ClientCtrl::share()->submitMoves(m_moveTimes);
	ClientCtrl::share()->submitTime(m_time);
	m_canTime = false;
	m_touchAble = true;
	m_tipping = false;
	m_isAutoCollecting = false;
	//判断是否连胜
	int winStreakNum = UserDefault::getInstance()->getIntegerForKey(STRG_WIN_STREAK_NUM, 0);
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	int winNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_WIN_NUM, 0);
	int bestScore = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_SCORE_OF_WIN, 0);
	int bestTime = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_TIME_OF_WIN, 0);
	int bestMoves = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_MOVES_OF_WIN, 0);
	int cumuScore = UserDefault::getInstance()->getIntegerForKey(STRG_CUMULATIVE_SCORE_OF_WIN, 0);
	m_uiCollectArea->clear();
	UndoManager::share()->clear();
	if (1)//(m_cardDataManager.getInitGameType() != kCommonInit)       //防止一直replay刷新
	{
		winNum += 1;
		winStreakNum += 1;
	}

	playNum = (playNum < winNum) ? winNum : playNum;
	if (m_score > bestScore)
	{
		bestScore = m_score;
		ClientCtrl::share()->playSound(SOUND_NEW_RECORD);
	}
	else
	{
		ClientCtrl::share()->playSound(SOUND_VICTORY);
	}
	bestScore = (m_score > bestScore) ? m_score : bestScore;
	bestTime = (bestTime == 0 || m_time < bestTime) ? m_time : bestTime;
	bestMoves = (bestMoves == 0 || m_moveTimes < bestMoves) ? m_moveTimes : bestMoves;
	cumuScore += m_score;


	//show winboard
	int todayBest = getBestScoreOfToday();
	WinBoard::setData(m_moveTimes, m_time, m_score, todayBest, bestScore);
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kWinBoard);

	if (winNum == 1)
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kRateBoard);
	}

	UserDefault::getInstance()->setIntegerForKey(STRG_WIN_STREAK_NUM, winStreakNum);
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_PLAY_NUM, playNum);
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_WIN_NUM, winNum );
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_SCORE_OF_WIN, bestScore);
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_TIME_OF_WIN, bestTime);
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_MOVES_OF_WIN, bestMoves);
	UserDefault::getInstance()->setIntegerForKey(STRG_CUMULATIVE_SCORE_OF_WIN, cumuScore);

	refreshChallengeDate();

	ST_SAFE_REMOVE_FROM_PARENT(m_autoCollectMenu);
	bakAllCards();
}

void GameLayer::refreshChallengeDate()
{
	bool isChallengeNow = UserDefault::getInstance()->getBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
	if (isChallengeNow)
	{
		int year = DailyChallengeManage::share()->getChallengeYear();
		int mon = DailyChallengeManage::share()->getChallengeMonth();
		int day = DailyChallengeManage::share()->getChallengeDay();
		char buf[64];
		sprintf(buf, "Best_Challenge_Score_Of_%d_%d", year, mon);
		int bestScoreOfMonth = UserDefault::getInstance()->getIntegerForKey(buf, 0);
		if (m_score > bestScoreOfMonth)
		{
			UserDefault::getInstance()->setIntegerForKey(buf, m_score);
			sprintf(buf, "Best_Day_%d_%d", year, mon);
			UserDefault::getInstance()->setIntegerForKey(buf, day);
		}
		
	}
}

int GameLayer::getBestScoreOfToday()
{
	time_t todayTm = time(NULL);
	struct tm *today = localtime(&todayTm);
	char buf[128];
	int year = today->tm_year + 1900;
	int mon = today->tm_mon + 1;
	int day = today->tm_mday;
	sprintf(buf, "Best_Score_%d_%d_%d", year, mon, day);
	int todayBest = UserDefault::getInstance()->getIntegerForKey(buf, 0);
	if (todayBest < m_score)
	{
		todayBest = m_score;
		UserDefault::getInstance()->setIntegerForKey(buf, todayBest);
	}
	return todayBest;
}
