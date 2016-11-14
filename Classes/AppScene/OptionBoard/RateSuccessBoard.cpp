#include "RateSuccessBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool RateSuccessBoard::init()
{
	if (!OptionBaseBoard::init(kRateSuccessBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}


void RateSuccessBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/SceneOK_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/SceneOK_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void RateSuccessBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(RateSuccessBoard::btnCloseCallBack, this));

}

void RateSuccessBoard::updateLanguage()
{
	//ok menu
	string okStr = STLang::shared()->getValue("OK");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextClose", okStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	// info
	string infoStr = STLang::shared()->getValue("Congratulation! Check new card back in option");
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.8;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInfo", infoStr, maxWidth, TextHAlignment::LEFT);
}


void RateSuccessBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(RateSuccessBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}
void RateSuccessBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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