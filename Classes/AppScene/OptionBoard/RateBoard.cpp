#include "RateBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool RateBoard::m_isExist = false;
bool RateBoard::init()
{
	if (!OptionBaseBoard::init(kRateBoard))
	{
		return false;
	}
	m_isExist = true;
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}
void RateBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Rate_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Rate_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void RateBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(RateBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRate1", CC_CALLBACK_2(RateBoard::btnRateCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRate2", CC_CALLBACK_2(RateBoard::btnRateCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRate3", CC_CALLBACK_2(RateBoard::btnRateCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRate4", CC_CALLBACK_2(RateBoard::btnRateCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRate5", CC_CALLBACK_2(RateBoard::btnRateCallBack, this));

}

void RateBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("Enjoying Solitaire?");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));


	//invite info
	string infoStr = STLang::shared()->getValue("Please let us know what you think. You will unlock card back style as a gift.");
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.9;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextRateInfo", infoStr, maxWidth, TextHAlignment::LEFT);


	//tap star info
	string tapStr = STLang::shared()->getValue("tap star to rate");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTapStar", tapStr, maxWidth, TextHAlignment::LEFT);
}


void RateBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(RateBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

	ClientCtrl::share()->reportEventParam("RateUsCannel");
	m_isExist = false;

}


int RateBoard::getNumFromString(string str)
{
	int len = str.size();
	if (len <= 0){ return 1; }
	string re = "";
	for (int i = 0; i < len; i++)
	{
		char ch = str[i];
		if (ch >= '0' && ch <= '9')
		{
			re += ch;
		}
	}
	int num = atoi(re.c_str());
	return num;
}

void RateBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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


void RateBoard::btnRateCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button *bt = (Button*)(pSender);
	ST_RETURN_IF(!bt);
	string name = bt->getName();
	int idx = getNumFromString(name) - 1;
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		closeEffect();
		string parmStr = "FiveStars";
		if (idx < 3)
		{
			parmStr = "UnderFiveStars";
		}
		ClientCtrl::share()->rateApp(idx + 1);
		ClientCtrl::share()->reportEventParam("RateUsDone", parmStr.c_str());
		UserDefault::getInstance()->setBoolForKey(STRG_HAS_ALREADY_RATE_US, true);
		break;
	}

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

