#include "ResetDataBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool ResetDataBoard::init()
{
	if (!OptionBaseBoard::init(kResetDataBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}


void ResetDataBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/StatisticReset_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/StatisticReset_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void ResetDataBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(ResetDataBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonReset", CC_CALLBACK_2(ResetDataBoard::btnResetCallBack, this));

}

void ResetDataBoard::updateLanguage()
{
	//CANCEL menu
	string cancelStr = STLang::shared()->getValue("CANCEL");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextClose", cancelStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//ok menu
	string okStr = STLang::shared()->getValue("RESET");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextReset", okStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//invite info
	string infoStr = STLang::shared()->getValue("Are you sure you want to reset statistics?");
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.9;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInfo", infoStr, maxWidth, TextHAlignment::LEFT);
}


void ResetDataBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(ResetDataBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}
void ResetDataBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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


void ResetDataBoard::btnResetCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		closeEffect();
		NotificationCenter::getInstance()->postNotification(ST_RESET_STA_DATA);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
