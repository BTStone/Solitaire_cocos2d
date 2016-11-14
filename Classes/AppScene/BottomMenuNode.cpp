#include "BottomMenuNode.h"
#include "OptionBaseBoard.h"
#include "CardSprite.h"
#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"
#include "DailyChallengeManager.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"


BottomMenuNode* BottomMenuNode::create(bool withEffect /* = true */)
{
	BottomMenuNode *re = new BottomMenuNode();
	if (re && re->init(withEffect))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	return NULL;
}

bool BottomMenuNode::init(bool withEffect)
{
	if (!Node::init())
	{
		return false;
	}
	m_withEffect = withEffect;
	m_isShow = true;
	m_orginBottomBoardPos = Vec2::ZERO;
	initRootCSBNode();
	handleButton();

	tryToShowMoreGameIcon();
	updateLanguage();
	fixScreen();
	return true;
}

void BottomMenuNode::initRootCSBNode()
{
	Size winSize = CLDWinSize();

	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/MainScene_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/MainScene_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setPosition(Vec2(0, 0));
	this->addChild(m_rootCSBNode);

	//隐藏PlayBoard和隔绝层
	CSBHelper::setChildVisible(m_rootCSBNode, "PlayBoard", false);
	CSBHelper::setChildVisible(m_rootCSBNode, "ButtonSeparateTouch", false);

	Node *bottomNode = ui::Helper::seekNodeByName(m_rootCSBNode, "BottomMenuBoard");
	if (bottomNode)
	{
		m_orginBottomBoardPos = bottomNode->getPosition();
	}
	
}

void BottomMenuNode::handleButton()
{
	CSBHelper::registerButton(m_rootCSBNode, "ButtonOptions", CC_CALLBACK_2(BottomMenuNode::btnOptionCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonDaily", CC_CALLBACK_2(BottomMenuNode::btnDailyCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonPlay", CC_CALLBACK_2(BottomMenuNode::btnPlayCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonHint", CC_CALLBACK_2(BottomMenuNode::btnHintCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonUndo", CC_CALLBACK_2(BottomMenuNode::btnUndoCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "MoreGameIcon", CC_CALLBACK_2(BottomMenuNode::btnMoreGameIconCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonSeparateTouch", CC_CALLBACK_2(BottomMenuNode::btnHidePlayBoardCallBack, this));

	//playBoard
	CSBHelper::registerButton(m_rootCSBNode, "ButtonNewGame", CC_CALLBACK_2(BottomMenuNode::btnNewGameCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonReplay", CC_CALLBACK_2(BottomMenuNode::btnReplayCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonStatistics", CC_CALLBACK_2(BottomMenuNode::btnStatisticCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonScore", CC_CALLBACK_2(BottomMenuNode::btnScoreCallBack, this));
}



void BottomMenuNode::updateLanguage()
{
	const char *names[] = {
		"NEW GAME",
		"REPLAY",
		"STATISTICS",
		"SCORES",
		"7 DAYS"
	};
	int menuNum = 9;
	const char *strKey[] = { "options", "daily", "play", "hint", "undo", "NEW GAME", "REPLAY", "STATISTICS", "SCORES","7 DAYS" };
	const char *csbNames[] = { "TextOptions", "TextDaily", "TextPlay", "TextHint", "TextUndo", "TextNewGame", "TextReplay", "TextStatistics", "TextScore"};
	for (int i = 0; i < menuNum; i++)
	{
		string str = STLang::shared()->getValue(strKey[i]);
		str = STLabelBMFont::convertAllLetterToCapital(str);
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str);
	}
}

void BottomMenuNode::fixScreen()
{
	Size winSize = CLDWinSize();


	//bottom menu
	Node *bottomMenu = ui::Helper::seekNodeByName(m_rootCSBNode, "BottomMenuBoard");
	Node *bottomMenuBg = ui::Helper::seekNodeByName(m_rootCSBNode, "BottomMenuBoardBg");
	if (bottomMenu && bottomMenuBg)
	{
		Size sz = bottomMenuBg->getContentSize();
		this->setScale(winSize.width / sz.width);
	}


	//game head bg
	Node *headBg = ui::Helper::seekNodeByName(m_rootCSBNode, "GameHeadBg");
	ImageView *headBgView = dynamic_cast<ImageView*>(headBg);
	if (headBgView)
	{
		Size sz = headBgView->getContentSize();
		Size toSize = Size(winSize.width / this->getScale(), sz.height / this->getScale());
		headBgView->setContentSize(toSize);
		Vec2 worldPos = Vec2(winSize.width*0.5, winSize.height);
		headBgView->setAnchorPoint(Vec2(0.5, 1));
		headBgView->setPosition(headBgView->getParent()->convertToNodeSpace(worldPos));
	}
}


void BottomMenuNode::tryToShowDailyPointSign()
{
	time_t nowTime = STTimeManager::share()->getLocalTime();
	struct tm *todayTime = localtime(&nowTime);
	int year = todayTime->tm_year + 1900;
	int mon = todayTime->tm_mon + 1;
	int day = todayTime->tm_mday;
	bool isSuc = DailyChallengeManage::share()->isChallengeSuccess(year, mon, day);
	if (!isSuc)
	{
		CSBHelper::setChildVisible(m_rootCSBNode, "DailyPointSign", true);
	}
	else
	{
		CSBHelper::setChildVisible(m_rootCSBNode, "DailyPointSign", false);
	}
}

void BottomMenuNode::tryToShowMoreGameIcon()
{
	Node *moreIcon = ui::Helper::seekNodeByName(m_rootCSBNode, "MoreGameIcon");
	ST_RETURN_IF(!moreIcon);
	moreIcon->setVisible(false);

	bool isShow = ClientCtrl::share()->isDevAdOn();        //后台是否打开
	ST_RETURN_IF(!isShow);

	string cfgStr = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_DEV_AD_CTRL);
	string value = ClientCtrl::share()->parseUMengOnlineConfig(cfgStr, "dev");
	log("BottomMenuNode,tryToAddMoreGameIcon, vale:%s", value.c_str());
	vector<string> packages = ClientCtrl::share()->splitStringByChar(value);
	bool allInstall = ClientCtrl::share()->isAllAppInstall(packages);
	ST_RETURN_IF(allInstall);

	moreIcon->setVisible(true);

	//light 
	Sprite *lightSp = Sprite::create("Pic/Img-Light-Ad.png");
	ST_RETURN_IF(!lightSp);
	lightSp->setPosition(Vec2(moreIcon->getContentSize().width*0.5, moreIcon->getContentSize().height*0.5));
	moreIcon->addChild(lightSp, 10);

#define AD_BUTTON_ACTION 2
#if(AD_BUTTON_ACTION == 1)
	//light fade action
	float fadeDt = 0.5f;
	FadeIn *fIn = FadeIn::create(fadeDt);
	FadeOut *fOut = FadeOut::create(fadeDt);
	Sequence *seq = Sequence::create(fIn, fOut, NULL);
	RepeatForever *action = RepeatForever::create(seq);
	lightSp->runAction(action);
#elif(AD_BUTTON_ACTION == 2)
	//light hide ,button scale
	float scaleDt = 0.3f;
	float delayDt = 60.0f;      //静止时间
	float minScale = moreIcon->getScale() * 0.8;
	float maxScale = moreIcon->getScale() * 1.0f;
	//lightSp->setVisible(false);
	moreIcon->setScale(minScale);
	ScaleTo *expandScale = ScaleTo::create(scaleDt, maxScale);
	ScaleTo *narrowScale = ScaleTo::create(scaleDt, minScale);
	Sequence *seq = Sequence::create(expandScale, narrowScale, NULL);
	Repeat *repeat = Repeat::create(seq, 5);
	DelayTime *delay = DelayTime::create(delayDt);
	Sequence *se = Sequence::create(repeat, delay, NULL);

	RepeatForever *action = RepeatForever::create(se);
	moreIcon->runAction(action);
#elif(AD_BUTTON_ACTION == 3)


#endif
	/*
	bool isPort = ClientCtrl::share()->isPortrait();
	ST_RETURN_IF(isPort);
	bool isShow = ClientCtrl::share()->isDevAdOn();        //后台是否打开
	ST_RETURN_IF(!isShow);

	string cfgStr = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_DEV_AD_CTRL);
	string value = ClientCtrl::share()->parseUMengOnlineConfig(cfgStr, "dev");
	log("BottomMenuNode,tryToAddMoreGameIcon, vale:%s", value.c_str());
	vector<string> packages = ClientCtrl::share()->splitStringByChar(value);
	bool allInstall = ClientCtrl::share()->isAllAppInstall(packages);
	ST_RETURN_IF(allInstall);

	int allIconNum = 21;
	int addNum = UserDefault::getInstance()->getIntegerForKey(STRG_ADD_MORE_GAME_ICON_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_ADD_MORE_GAME_ICON_NUM, addNum + 1);
	int picIdx = rand() % allIconNum + 1;
	char buf[32];
	sprintf(buf, "Tip-Button-%02d.png", picIdx);
	string iconPath = "Pic/MoreGameIcon/";
	string picName = iconPath + buf;

	Size winSize = CLDWinSize();
	Button *bt = Button::create(picName);
	ST_RETURN_IF(!bt);
	bt->setPosition(CLDWinPos(0.2, 0.3));
	m_notArrowNode->addChild(bt, 5);
	bt->setName(More_Game_Icon);
	bt->setTag(10000 + picIdx);
	bt->addTouchEventListener(CC_CALLBACK_2(BottomMenuNode::btMoreGameIconCallBack, this));

	//light 
	Sprite *lightSp = Sprite::create("Pic/Img-Light-Ad.png");
	ST_RETURN_IF(!lightSp);
	lightSp->setPosition(Vec2(bt->getContentSize().width*0.5, bt->getContentSize().height*0.5));
	bt->addChild(lightSp, 10);

#define AD_BUTTON_ACTION 2
#if(AD_BUTTON_ACTION == 1)
	//light fade action
	float fadeDt = 0.5f;
	FadeIn *fIn = FadeIn::create(fadeDt);
	FadeOut *fOut = FadeOut::create(fadeDt);
	Sequence *seq = Sequence::create(fIn, fOut, NULL);
	RepeatForever *action = RepeatForever::create(seq);
	lightSp->runAction(action);
#elif(AD_BUTTON_ACTION == 2)
	//light hide ,button scale
	float scaleDt = 0.3f;
	float delayDt = 60.0f;      //静止时间
	float minScale = m_menuWidth / bt->getContentSize().width * 0.8;
	float maxScale = m_menuWidth / bt->getContentSize().width * 1.0f;
	//lightSp->setVisible(false);
	bt->setScale(minScale);
	ScaleTo *expandScale = ScaleTo::create(scaleDt, maxScale);
	ScaleTo *narrowScale = ScaleTo::create(scaleDt, minScale);
	Sequence *seq = Sequence::create(expandScale, narrowScale, NULL);
	Repeat *repeat = Repeat::create(seq, 5);
	DelayTime *delay = DelayTime::create(delayDt);
	Sequence *se = Sequence::create(repeat, delay, NULL);

	RepeatForever *action = RepeatForever::create(se);
	bt->runAction(action);
#elif(AD_BUTTON_ACTION == 3)


#endif
	Size bgSize = m_menuBg->getContentSize();
	Vec2 bgPos = m_menuBg->getPosition();
	float posY = bgPos.y + bgSize.height*0.5 - m_menuWidth*0.5;
	float posX = m_menuWidth / 2.0 + 2*m_menuWidth - winSize.width*0.5;
	bt->setPosition(Vec2(posX, posY));
	*/
}

void BottomMenuNode::tryToShowPlayBoard(bool canReplay)
{
	//显示隔绝层
	CSBHelper::setChildVisible(m_rootCSBNode, "ButtonSeparateTouch", true);

	Node *board = ui::Helper::seekNodeByName(m_rootCSBNode, "PlayBoard");
	ST_RETURN_IF(!board);
	board->setScale(0.1f);
	board->setVisible(true);

	updateReplayState(canReplay);
	ClientCtrl::share()->tryToPreGameAds();
	float dt = 0.4f;
	ScaleTo *scaleTo = ScaleTo::create(dt, 1.0f, 1.0f);
	ST_RETURN_IF(!scaleTo);
	EaseElasticOut *ease = EaseElasticOut::create(scaleTo);
	ST_RETURN_IF(!ease);
	DelayTime *delay = DelayTime::create(0.6f);
	ST_RETURN_IF(!delay);
	CallFunc *callFunc = CallFunc::create([](){
		ClientCtrl::share()->tryToShowGameAds();
	});
	Sequence *seq = Sequence::create(ease, delay, callFunc, NULL);
	ST_RETURN_IF(!seq);
	board->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_IN);

	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		ClientCtrl::share()->tryToShowBanner();
	}
}

void BottomMenuNode::updateReplayState(bool canReplay)
{
	Node *replayNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonReplay");
	Button *replayBt = dynamic_cast<Button*>(replayNode);
	if (replayBt)
	{
		if (!canReplay)
		{
			replayBt->setBright(false);
			replayBt->setTouchEnabled(false);
		}
		else
		{
			replayBt->setBright(true);
			replayBt->setTouchEnabled(true);
		}
	}

}

bool BottomMenuNode::tryToHidePlayBoard()
{
	//隐藏隔绝层
	CSBHelper::setChildVisible(m_rootCSBNode, "ButtonSeparateTouch", false);

	Node *board = ui::Helper::seekNodeByName(m_rootCSBNode, "PlayBoard");
	ST_RETURN_FALSE_IF(!board);
	ST_RETURN_FALSE_IF(!board->isVisible());     //必须

	board->setVisible(true);

	float dt = 0.2f;
	ScaleTo *scaleTo = ScaleTo::create(dt, 0);
	ST_RETURN_FALSE_IF(!scaleTo);
	EaseElasticOut *ease = EaseElasticOut::create(scaleTo);
	ST_RETURN_FALSE_IF(!ease);
	CallFuncN *callFunc = CallFuncN::create([](Node *node){
		node->setVisible(false);
	});
	ST_RETURN_FALSE_IF(!callFunc);
	Sequence *seq = Sequence::create(ease, callFunc, NULL);
	ST_RETURN_FALSE_IF(!seq);
	board->runAction(seq);

	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

	return true;
}

void BottomMenuNode::btnMoreGameIconCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		ClientCtrl::share()->moreGameIcon();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void BottomMenuNode::btnPlayCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		//删除闪烁特效
		removePlayEffect();
		NotificationCenter::getInstance()->postNotification(ST_SHOW_PLAY_BOARD);
		ClientCtrl::share()->reportEventParam("ClickPlayButton");
		
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void BottomMenuNode::btnOptionCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kRootOptionBoard));
		ClientCtrl::share()->reportEventParam("ClickSetButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void BottomMenuNode::btnDailyCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_CONNECT_NETWORK);
		ClientCtrl::share()->reportEventParam("ClickDailyChallengeButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void BottomMenuNode::btnHintCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_TIP_FOR_GAME);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void BottomMenuNode::btnUndoCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_UNDO_FOR_GAME);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void BottomMenuNode::btnHidePlayBoardCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		tryToHidePlayBoard();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void BottomMenuNode::btnNewGameCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		tryToHidePlayBoard();
		NotificationCenter::getInstance()->postNotification(ST_NEW_GAME);
		ClientCtrl::share()->reportEventParam("ClickPlayBoardNewGameButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void BottomMenuNode::btnReplayCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		tryToHidePlayBoard();
		NotificationCenter::getInstance()->postNotification(ST_REPLAY_GAME);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}



}


void BottomMenuNode::btnStatisticCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		tryToHidePlayBoard();
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kStatisticsBoard);
		ClientCtrl::share()->reportEventParam("ClickStatisticsButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}



}


void BottomMenuNode::btnScoreCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		tryToHidePlayBoard();
		ClientCtrl::share()->reportEventParam("ClickScoreButton");
		ClientCtrl::share()->googleRank();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}



}

void BottomMenuNode::addPlayEffect()
{
	Node *playNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonPlay");
	ST_RETURN_IF(!playNode);

	//添加play闪烁
	Sprite *lightSp = Sprite::create("Pic/UI-Icon-Games-whiteshadow.png");
	ST_RETURN_IF(!lightSp);
	lightSp->setScale(playNode->getScale());
	lightSp->setPosition(playNode->getPosition());
	lightSp->setTag(Play_Light_Tag);
	playNode->getParent()->addChild(lightSp, playNode->getLocalZOrder() - 1);

	lightSp->setOpacity(0);
	float dt = 0.5f;
	CCFadeIn *fadeIn = CCFadeIn::create(dt);
	ST_RETURN_IF(!fadeIn);
	CCFadeOut *fadeO = CCFadeOut::create(dt);
	ST_RETURN_IF(!fadeO);
	Sequence *seq = Sequence::create(fadeIn, fadeO, NULL);
	ST_RETURN_IF(!seq);
	RepeatForever *repeat = RepeatForever::create(seq);
	ST_RETURN_IF(!repeat);
	lightSp->runAction(repeat);
}

void BottomMenuNode::removePlayEffect()
{
	Node *node = ui::Helper::seekNodeByTag(m_rootCSBNode, Play_Light_Tag);
	ST_RETURN_IF(!node);
	ST_SAFE_REMOVE_FROM_PARENT(node);
}




void BottomMenuNode::show()
{
	m_isShow = true;

	float dt = 0.2f;
	Node *bottomNode = ui::Helper::seekNodeByName(m_rootCSBNode, "BottomMenuBoard");
	ST_RETURN_IF(!bottomNode);
	bottomNode->stopAllActions();

	bottomNode->runAction(MoveTo::create(dt, m_orginBottomBoardPos));
}

void BottomMenuNode::hide()
{
	m_isShow = false;

	float dt = 0.2f;
	float mvOff = 400.0f;      //与hide一致
	Vec2 endPos = Vec2(0, 0);

	Node *bottomNode = ui::Helper::seekNodeByName(m_rootCSBNode, "BottomMenuBoard");
	ST_RETURN_IF(!bottomNode);
	bottomNode->stopAllActions();

	endPos.x = bottomNode->getPositionX();
	endPos.y = m_orginBottomBoardPos.y - mvOff;

	bottomNode->runAction(MoveTo::create(dt, endPos));
}

void BottomMenuNode::switchShowAndHide()
{
	log("switchShowAndHide");
	if (m_isShow)
	{
		hide();
	}
	else
	{
		show();
	}
}

void BottomMenuNode::setHandType(bool isLeft)
{
	Node *optionPosNode = ui::Helper::seekNodeByName(m_rootCSBNode, "OptionPosNode");
	Node *dailyPosNode = ui::Helper::seekNodeByName(m_rootCSBNode, "DailyPosNode");
	//Node *playNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonPlay");
	Node *hintPosNode = ui::Helper::seekNodeByName(m_rootCSBNode, "HintPosNode");
	Node *undoPosNode = ui::Helper::seekNodeByName(m_rootCSBNode, "UndoPosNode");
	ST_RETURN_IF(!(optionPosNode && dailyPosNode && hintPosNode && undoPosNode));


	Node *optionNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonOptions");
	Node *dailyNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonDaily");
	//Node *playNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonPlay");
	Node *hintNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonHint");
	Node *undoNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ButtonUndo");
	ST_RETURN_IF(!(optionNode && dailyNode && hintNode && undoNode));

	Vec2 optionPos = optionPosNode->getPosition();
	Vec2 dailyPos = dailyPosNode->getPosition();
	Vec2 hintPos = hintPosNode->getPosition();
	Vec2 undoPos = undoPosNode->getPosition();

	if (isLeft)
	{
		optionNode->setPosition(undoPos);
		dailyNode->setPosition(hintPos);
		hintNode->setPosition(dailyPos);
		undoNode->setPosition(optionPos);
	}
	else
	{
		optionNode->setPosition(optionPos);
		dailyNode->setPosition(dailyPos);
		hintNode->setPosition(hintPos);
		undoNode->setPosition(undoPos);
	}
}
