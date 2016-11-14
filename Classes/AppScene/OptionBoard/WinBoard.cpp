#include "WinBoard.h"
#include "DailyChallengeManager.h"
#include "ClientCtrl.h"
#include "InviteFriendBoard.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

int WinBoard::m_moves = 0;
int WinBoard::m_time = 0;
int WinBoard::m_score = 0;
int WinBoard::m_todayBest = 0;
int WinBoard::m_record = 0;

void WinBoard::setData(int moves, int time, int score, int todayBest, int record)
{
	m_moves = moves;
	m_time = time;
	m_score = score;
	m_todayBest = todayBest;
	m_record = record;
}

WinBoard* WinBoard::create()
{
	WinBoard *re = new WinBoard();
	if (re && re->init())
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool WinBoard::init()
{
	if (!OptionBaseBoard::init(kWinBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}
void WinBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Win_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Win_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void WinBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(WinBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonInvite", CC_CALLBACK_2(WinBoard::btnInviteCallBack, this));

	//ok 或者 new Game
	bool isChallenge = UserDefault::getInstance()->getBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
	if (!isChallenge)
	{
		CSBHelper::registerButton(m_rootCSBNode, "ButtonNewGame", CC_CALLBACK_2(WinBoard::btnNewGameCallBack, this));
		ClientCtrl::share()->reportEventParam("WinNumNewGame");
	}
	else
	{
		CSBHelper::registerButton(m_rootCSBNode, "ButtonNewGame", CC_CALLBACK_2(WinBoard::btnCloseCallBack, this));
		ClientCtrl::share()->reportEventParam("WinNumDailyChallenge");
	}
}

void WinBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("GAME SCORE");
	titleStr = STLabelBMFont::convertAllLetterToCapital(titleStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//invite menu 
	string inviteStr = STLang::shared()->getValue("Invite Button");
	inviteStr = STLabelBMFont::convertAllLetterToCapital(inviteStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInvite", inviteStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//ok 或者 new Game
	string okStr = STLang::shared()->getValue("OK");
	bool isChallenge = UserDefault::getInstance()->getBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
	if (!isChallenge)
	{
		okStr = STLang::shared()->getValue("NEW GAME");
	}
	okStr = STLabelBMFont::convertAllLetterToCapital(okStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextNewGame", okStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//update key
	updateContentKey();

	//value
	updateContentValue();

}


void WinBoard::updateContentKey()
{
	int num = 5;
	const char *keyNames[] = { "moves", "time", "score", "Today Best :", "Record :" };

	const char *csbNames[] = {"TextMoves", "TextTime", "TextScore", "TextTodayBest", "TextRecord"};

	for (int i = 0; i < num; i++)
	{
		string str = STLang::shared()->getValue(keyNames[i]);
		str = STLabelBMFont::convertFirstLetterToCapital(str);
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
	}
}
void WinBoard::updateContentValue()
{
	int num = 5;
	//数据
	int datas[5] = { 0 };
	datas[0] = m_moves;
	datas[1] = m_time;
	datas[2] = m_score;
	datas[3] = m_todayBest;
	datas[4] = m_record;

	const char *csbNames[] = { "ValueMoves", "ValueTime", "ValueScore", "ValueTodayBest", "ValueRecord" };

	for (int i = 0; i < num; i++)
	{
		string str = "";
		if (i != 1)
		{
			char buf[64];
			sprintf(buf, "%d", datas[i]);
			str += buf;
		}
		else
		{
			str = convertToTimeStyle(datas[i]);
		}
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str); 
	}
}


void WinBoard::addSnowEffect()
{
	/*
	Size winSize = CLDWinSize();
	Vec2 headPos = m_headBg->getPosition();
	Size headSize = m_headBg->getContentSize();
	ParticleSystemQuad *particleSystem = ParticleSystemQuad::create("effect/zhihua.plist");
	particleSystem->setPosition(headPos + Vec2(0, headSize.height));
	particleSystem->setPosition(Vec2(0, winSize.height*0.5));
	particleSystem->setAutoRemoveOnFinish(true);
	particleSystem->setScale(1.0f);
	this->addChild(particleSystem, 100);
	*/
}


void WinBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(WinBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);
}

string WinBoard::convertToTimeStyle(int sec)
{
	if (sec == 0)
	{
		return "-:-";
	}
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


void WinBoard::removeSelfCallFunc()
{
	OptionBaseBoard::removeSelfCallFunc();
	bool isChallenge = UserDefault::getInstance()->getBoolForKey(STRG_CURRENT_GAME_IS_CHALLENGE, false);
	int year = DailyChallengeManage::share()->getChallengeYear();
	int mon = DailyChallengeManage::share()->getChallengeMonth();
	int day = DailyChallengeManage::share()->getChallengeDay();
	bool isSuc = DailyChallengeManage::share()->isChallengeSuccess(year, mon, day);
	if (isChallenge && !isSuc)
	{
		DailyChallengeManage::share()->setJustNowSuccess(true);             //记录刚刚挑战成功
		DailyChallengeManage::share()->success(year, mon, day);
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kDailyChallengeBoard);
		int challengeWinNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_CHALLENGE_SUCCESS_NUM, 0);
		UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_CHALLENGE_SUCCESS_NUM, challengeWinNum + 1);
	}

}

void WinBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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

void WinBoard::btnNewGameCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		closeEffect();
		NotificationCenter::getInstance()->postNotification(ST_NEW_GAME);
		ClientCtrl::share()->reportEventParam("ClickWinBoardNewGameButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void WinBoard::btnInviteCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		InviteFriendBoard::setResString("WinBoard");
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kInviteFriendBoard);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
