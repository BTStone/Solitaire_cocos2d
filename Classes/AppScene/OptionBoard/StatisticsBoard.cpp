#include "StatisticsBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"


StatisticsBoard::StatisticsBoard()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StatisticsBoard::msgResetDataCallBack), ST_RESET_STA_DATA, NULL);
}

StatisticsBoard::~StatisticsBoard()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool StatisticsBoard::init()
{
	if (!OptionBaseBoard::init(kStatisticsBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}

void StatisticsBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Statistic_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Statistic_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void StatisticsBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(StatisticsBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonOk", CC_CALLBACK_2(StatisticsBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonReset", CC_CALLBACK_2(StatisticsBoard::btnResetCallBack, this));

}

void StatisticsBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("STATISTICS");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//reset and ok 
	string resetStr = STLang::shared()->getValue("RESET");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextReset", resetStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	string okStr = STLang::shared()->getValue("OK");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextOk", okStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//all statistics
	string allStr = STLang::shared()->getValue("ALL STATISTICS");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextAllStatistics", allStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//update key
	updateContentKey();

	//value
	updateContentValue();

}

void StatisticsBoard::updateContentKey()
{
	const char *keyNames[] = {
		"Games Played :", "Games Won :", "Winning Streak:", "Win % :",
		"Best Moves :", "Best Time :", "Best Score :", "Cumulative Score :"
	};
	const char *csbNames[] = {
		"TextGamePlayed", "TextGameWon", "TextWinStreak", "TextWinPercent",
		"TextBestMoves", "TextBestTime", "TextBestScore", "TextCumulativeScore"
	};
	int keyNum = 8;
	for (int i = 0; i < keyNum; i++)
	{
		string str = STLang::shared()->getValue(keyNames[i]);
		str = STLabelBMFont::convertFirstLetterToCapital(str);
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
	}
}
void StatisticsBoard::updateContentValue()
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	int winNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_WIN_NUM, 0);
	int winStreakNum = UserDefault::getInstance()->getIntegerForKey(STRG_WIN_STREAK_NUM, 0);
	float winPercent = (playNum == 0) ? 0 : winNum * 100.0 / playNum;
	int bestMoves = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_MOVES_OF_WIN, 0);
	int bestTime = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_TIME_OF_WIN, 0);
	int bestScore = UserDefault::getInstance()->getIntegerForKey(STRG_BEST_SCORE_OF_WIN, 0);
	int cumuScore = UserDefault::getInstance()->getIntegerForKey(STRG_CUMULATIVE_SCORE_OF_WIN, 0);

	int num = 8;
	float info[8] = { 0 };
	info[0] = playNum;
	info[1] = winNum;
	info[2] = winStreakNum;
	info[3] = winPercent;
	info[4] = bestMoves;
	info[5] = bestTime;
	info[6] = bestScore;
	info[7] = cumuScore;


	const char *csbNames[] = {
		"ValueGamePlayed", "ValueGameWon", "ValueWinStreak", "ValueWinPercent",
		"ValueBestMoves", "ValueBestTime", "ValueBestScore", "ValueCumulativeScore"
	};

	int BeginTag = 100;
	for (int i = 0; i < num; i++)
	{
		string str = String::createWithFormat("%d", (int)info[i])->getCString();
		if (i == 3)         //win streak
		{
			str = String::createWithFormat("%.2f%%", info[i])->getCString();
		}
		else if (i == 5)   //best time
		{
			str = convertToTimeStyle(info[i]);
		}
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str);
	}
}


void StatisticsBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(StatisticsBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

string StatisticsBoard::convertToTimeStyle(int sec)
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


void StatisticsBoard::msgResetDataCallBack(Ref *ref)
{
	resetData();
}

void StatisticsBoard::resetData()
{
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_TOTAL_WIN_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_WIN_STREAK_NUM, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_MOVES_OF_WIN, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_TIME_OF_WIN, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_BEST_SCORE_OF_WIN, 0);
	UserDefault::getInstance()->setIntegerForKey(STRG_CUMULATIVE_SCORE_OF_WIN, 0);
	updateLanguage();
}

void StatisticsBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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

void StatisticsBoard::btnResetCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kResetDataBoard));
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}