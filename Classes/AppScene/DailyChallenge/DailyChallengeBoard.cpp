#include "DailyChallengeBoard.h"
#include "ClientCtrl.h"
#include "DailyChallengeManager.h"
#include "CardSprite.h"
#include "TrophyInfoBoard.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool DailyChallengeBoard::m_isAutoShow = false;
DailyChallengeBoard::DailyChallengeBoard()
{
	
}

DailyChallengeBoard::~DailyChallengeBoard()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}


DailyChallengeBoard* DailyChallengeBoard::create(int year, int mon, int day)
{
	DailyChallengeBoard *re = new DailyChallengeBoard();
	if (re && re->init(year, mon, day))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	return NULL;
}


bool DailyChallengeBoard::init(int year, int mon, int day)
{
	if (!OptionBaseBoard::init(kDailyChallengeBoard))
	{
		return false;
	}
	m_isAutoShow = false;
	initRootCSBNode();
	initCalendar(year, mon, day);
	handleButton();
	updateLanguage();   //放在refresh之前
	updateUIState();
	
	bool justNowSuc = DailyChallengeManage::share()->getJustNowSuccess();  //是否是刚刚成功一个新日期
	if (justNowSuc)
	{
		DailyChallengeManage::share()->setJustNowSuccess(false);
		addSuccessEffect();
		m_isAutoShow = true;
	}
	refresh();

	//必须放在日历添加之后（initCalendar之后）
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DailyChallengeBoard::msgSelectCalendarDayCallBack), ST_SELECT_CALENDAR_DAY, NULL);

	return true;
}

void DailyChallengeBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/DailyChallenge_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/DailyChallenge_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}


void DailyChallengeBoard::handleButton()
{

	//button
	CSBHelper::registerButton(m_rootCSBNode, "BtnHelp", CC_CALLBACK_2(DailyChallengeBoard::btnHelpCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnClose", CC_CALLBACK_2(DailyChallengeBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnPlay", CC_CALLBACK_2(DailyChallengeBoard::btnPlayCallBack, this));

	//check box
	CSBHelper::registerCheckBox(m_rootCSBNode, "CheckBox", this, checkboxselectedeventselector(DailyChallengeBoard::checkBoxSwitchCallBack));
}

void DailyChallengeBoard::initCalendar(int year, int mon, int day)
{
	bool justNowSuc = DailyChallengeManage::share()->getJustNowSuccess();  //是否是刚刚成功一个新日期
	if (justNowSuc)
	{
		year = DailyChallengeManage::share()->getChallengeYear();
		mon = DailyChallengeManage::share()->getChallengeMonth();
	}

	Node *areaNode = ui::Helper::seekNodeByName(m_rootCSBNode, "CalendarArea");
	ST_RETURN_IF(!areaNode);
	Vec2 pos = areaNode->getPosition();

	m_calendar = Calendar::create(year, mon, day);
	m_calendar->setPosition(pos);
	areaNode->getParent()->addChild(m_calendar, areaNode->getLocalZOrder());

}
void DailyChallengeBoard::setIsAutoShow(bool isAutoShow)
{
	m_isAutoShow = isAutoShow;
}

bool DailyChallengeBoard::getIsAutoShow()
{
	return m_isAutoShow;
}


void DailyChallengeBoard::addSuccessEffect()
{
	m_calendar->addSuccessEffect();
	refresh();
}

void DailyChallengeBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(DailyChallengeBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

void DailyChallengeBoard::tryToShowRightSign()
{
	Node *levelSign = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgLevelSelect");
	ST_RETURN_IF(!levelSign);
	levelSign->setVisible(false);
	int sucNum = DailyChallengeManage::share()->getSuccessNumOfMon(m_calendar->getCurYear(), m_calendar->getCurMonth());
	//sucNum = 30;
	int totalDays = Calendar::getTotalDayOfMonth(m_calendar->getCurYear(), m_calendar->getCurMonth());
	ST_RETURN_IF(sucNum < 10);
	Vec2 signPos = levelSign->getPosition();
	if (sucNum >= totalDays)
	{
		Node *imgLevel = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgLevel3");
		if (imgLevel)
		{
			signPos = imgLevel->getPosition() + Vec2(0, -15);
		}
	}
	else if (sucNum >= 20)
	{
		Node *imgLevel = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgLevel2");
		if (imgLevel)
		{
			signPos = imgLevel->getPosition() + Vec2(0, -15);
		}
	}
	else if (sucNum >= 10)
	{
		Node *imgLevel = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgLevel1");
		if (imgLevel)
		{
			signPos = imgLevel->getPosition() + Vec2(0, -15);
		}
	}
	levelSign->setVisible(true);
	levelSign->setPosition(signPos);
}

void DailyChallengeBoard::checkBoxSwitchCallBack(cocos2d::Ref *pSender, CheckBoxEventType type)
{
	switch (type) {
	case cocos2d::ui::CHECKBOX_STATE_EVENT_SELECTED:
		//选中
		UserDefault::getInstance()->setBoolForKey(STRG_IS_NOTIFY_CHALLENGE, true);
		ClientCtrl::share()->reportEventParam("ReceiveNotifyButtonOn");
		break;

	case cocos2d::ui::CHECKBOX_STATE_EVENT_UNSELECTED:
		//未选中
		UserDefault::getInstance()->setBoolForKey(STRG_IS_NOTIFY_CHALLENGE, false);
		ClientCtrl::share()->reportEventParam("ReceiveNotifyButtonOff");
		break;
	default:
		break;
	}

}

void DailyChallengeBoard::menuTrophyCliekCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button* bt = dynamic_cast<Button*>(pSender);
	ST_RETURN_IF(!bt);
	int btTag = bt->getTag();
	Vec2 pos = bt->getPosition();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		addTrophyInfoBoard(btTag,pos);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void DailyChallengeBoard::addTrophyInfoBoard(int btTag, Vec2 pos)
{
	TrophyType tp = kCopperTrophy;
	if (btTag == Daily_Copper_Tag)
	{
		tp = kCopperTrophy;
	}
	else if (btTag == Daily_Silver_Tag)
	{
		tp = kSilverTrophy;
	}
	else
	{
		tp = kGoldenTrophy;
	}
	int curYear = Calendar::getCurYear();
	int curMon = Calendar::getCurMonth();
	TrophyInfoBoard* board = TrophyInfoBoard::create(tp, curYear, curMon);
	ST_RETURN_IF(!board);
	board->setPosition(pos + Vec2(0, 100));
	this->addChild(board, 100);
}

void DailyChallengeBoard::showTrophyInfoBoard()
{
	/*
	Size bgSize = m_bg->getContentSize();
	Node *node = Node::create();
	ST_RETURN_IF(!node);
	node->setPosition(m_progress->getPosition());
	node->setTag(400);
	this->addChild(node, 100);

	Size infoBgSize = Size(400, 100);
	Scale9Sprite *bg = Scale9Sprite::create("Pic/UI-Box-MenuBox-Bg2.png");
	ST_RETURN_IF(!bg);
	bg->setContentSize(infoBgSize);
	bg->setPosition(Vec2(0, infoBgSize.height*0.5));
	//bg->setAnchorPoint(Vec2(0.5, 0));
	//bg->setColor(Color3B::BLACK);
	node->addChild(bg, 0);

	int sucNum = DailyChallengeManage::share()->getSuccessNumOfMon(m_calendar->getCurYear(), m_calendar->getCurMonth());
	string infoStr = STLang::shared()->getValue("this month");
	infoStr += ":";
	infoStr += String::createWithFormat("%d", sucNum)->getCString();
	STLabelBMFont *infoLab = STLabelBMFont::createWithBMFont("EurostileBold.fnt", infoStr, true);
	ST_RETURN_IF(!infoLab);
	infoLab->setTextSize(Size(infoBgSize.width*0.9, infoBgSize.height*0.5));
	infoLab->setPosition(Vec2(0, infoBgSize.height*0.65));
	infoLab->setColor(Color3B(50, 53, 57));
	node->addChild(infoLab, 1);

	STLabelBMFont *numLab = STLabelBMFont::createWithBMFont("EurostileBold.fnt", String::createWithFormat("%d", sucNum)->getCString());
	ST_RETURN_IF(!numLab);
	numLab->setTextSize(Size(infoBgSize.width*0.9, infoBgSize.height*0.5));
	numLab->setPosition(Vec2(0, infoBgSize.height*0.4));
	//node->addChild(numLab, 1);

	STHelper::addDarkLayer(node);
	//添加隔绝点击菜单，并且使点击时候关闭面板
	Node * spaceNode = Node::create();
	ST_RETURN_IF(!spaceNode);
	spaceNode->setContentSize(CCSizeMake(CLDWinSize().width * 3, CLDWinSize().height * 3));
	MenuItemSprite * menuSeparate = MenuItemSprite::create(
		spaceNode,
		spaceNode,
		NULL,
		this,
		menu_selector(DailyChallengeBoard::closeTrophyInfoBoardCallBack));
	ST_RETURN_IF(!menuSeparate);

	Menu * pMenu = Menu::createWithItem(menuSeparate);
	ST_RETURN_IF(!pMenu);
	pMenu->setPosition(Vec2::ZERO);
	node->addChild(pMenu);
	*/
}

void DailyChallengeBoard::closeTrophyInfoBoardCallBack(Ref *ref)
{
	Node *node = getChildByTag(400);
	ST_SAFE_REMOVE_FROM_PARENT(node);
}

void DailyChallengeBoard::btnHelpCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		//每日挑战帮助面板
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kDailyChallengeHelpBoard);
		ClientCtrl::share()->reportEventParam("ClickDailyChallengeHelpButton");
	break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void DailyChallengeBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		closeEffect();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void DailyChallengeBoard::btnPlayCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		//每日挑战
		closeEffect();

		//记录挑战的日期
		DailyChallengeManage::share()->setChallengeData(m_calendar->getCurYear(), m_calendar->getCurMonth(), m_calendar->getSelectDay());

		float dt = 0.16f;
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(dt),
			CallFunc::create(this, callfunc_selector(DailyChallengeBoard::startDailyChallengeGame))));
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void DailyChallengeBoard::startDailyChallengeGame()
{
	NotificationCenter::getInstance()->postNotification(ST_DAILY_CHALLENGE_GAME_START);

	ClientCtrl::share()->reportEventParam("ClickDailyChallengePlayButton");
}

void DailyChallengeBoard::msgSelectCalendarDayCallBack(Ref *ref)
{
	refresh();
}

void DailyChallengeBoard::refresh()
{
	int curYear = m_calendar->getCurYear();
	int curMon = m_calendar->getCurMonth();
	int selectDay = m_calendar->getSelectDay();
	//m_solvedStatus
	bool isSuc = DailyChallengeManage::share()->isChallengeSuccess(curYear, curMon, selectDay);
	string solvedKeyStr = "solved";
	if (!isSuc)
	{
		solvedKeyStr = "unsolved";
	}
	string solvedStr = STLang::shared()->getValue(solvedKeyStr.c_str());
	solvedStr = STLabelBMFont::convertFirstLetterToCapital(solvedStr);
	Color3B solvedColor = Color3B::WHITE;
	if (!isSuc)
	{
		solvedColor = Color3B(255, 113, 50);
	}
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextSolved", solvedStr, -1.0f, TextHAlignment::LEFT, solvedColor);

	//进度条
	int totalDay = Calendar::getTotalDayOfMonth(curYear, curMon);
	int sucNum = DailyChallengeManage::share()->getSuccessNumOfMon(curYear, curMon);
	float sale = sucNum / (float)totalDay * 1.0;
	float percnt = sale * 100;
	Node *sliderNode = ui::Helper::seekNodeByName(m_rootCSBNode, "Slider");
	Slider *slider = dynamic_cast<Slider*>(sliderNode);
	if (slider)
	{
		slider->setPercent(percnt);
	}

	//总共多少天
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextLevel3", String::createWithFormat("%d", totalDay)->getCString());

	refreshCrownOfSolved(isSuc);
	tryToShowRightSign();
	refreshPlayMenu(isSuc);
}

void DailyChallengeBoard::updateLanguage()
{
	int menuNum = 4;
	const char *strKey[] = { "daily challenge", "solved", "play", "Receive Notification" };

	const char *csbNames[] = { "TextTitle", "TextSolved", "TextPlay", "TextNotifyInfo" };

	for (int i = 0; i < menuNum; i++)
	{
		string str = STLang::shared()->getValue(strKey[i]);
		if (i == 0 || i == 2)//title, str
		{
			str = STLabelBMFont::convertAllLetterToCapital(str);
			CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
		}
		else
		{
			str = STLabelBMFont::convertFirstLetterToCapital(str);
			CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
		}
	}
}

void DailyChallengeBoard::updateUIState()
{
	//check box state
	bool isSelect = UserDefault::getInstance()->getBoolForKey(STRG_IS_NOTIFY_CHALLENGE, true);
	CSBHelper::setCheckBoxSelected(m_rootCSBNode, "CheckBox", isSelect);
}

void DailyChallengeBoard::refreshPlayMenu(bool isSuc)
{
	//play button show
	string keyStr = "play";
	string normalPic = "Pic/UI-Button-Green.png";
	string selectPic = "Pic/UI-Button-Green-Press.png";
	if (isSuc)
	{
		keyStr = "REPLAY";
		normalPic = "Pic/UI-Button-Orange.png";
		selectPic = "Pic/UI-Button-Orange-Press.png";
	}
	Node *playNode = ui::Helper::seekNodeByName(m_rootCSBNode, "BtnPlay");
	Button *playBt = dynamic_cast<Button*>(playNode);
	if (playBt)
	{
		//playBt->loadTextureNormal(normalPic);
		//playBt->loadTexturePressed(selectPic);
	}
	
	//text play
	string playStr = STLang::shared()->getValue(keyStr.c_str());
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextPlay", playStr);
}

void DailyChallengeBoard::refreshCrownOfSolved(bool isSolved)
{
	string picName = "";
	if (!isSolved)
	{
		picName = "Pic/UI-Icon-GetTodayCrown-UnGet.png";
	}
	else
	{
		picName = "Pic/UI-Icon-Crown-lv2-x128.png";
	}
	CSBHelper::loadImageViewTexture(m_rootCSBNode, "SolvedSign", picName);
}
