#include "InviteFriendSimpleBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool InviteFriendSimpleBoard::init()
{
	if (!OptionBaseBoard::init(kInviteFriendSimpleBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}


void InviteFriendSimpleBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/InviteFriends02_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/InviteFriends02_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void InviteFriendSimpleBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(InviteFriendSimpleBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonInvite", CC_CALLBACK_2(InviteFriendSimpleBoard::btnInviteCallBack, this));

}

void InviteFriendSimpleBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("daily challenge");
	titleStr = STLabelBMFont::convertAllLetterToCapital(titleStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//CANCEL menu
	string cancelStr = STLang::shared()->getValue("CANCEL");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextClose", cancelStr);

	//ok menu
	string okStr = STLang::shared()->getValue("Invite Button");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInvite", okStr);

	//invite info
	string infoStr = STLang::shared()->getValue("Invite Friends to unlock new Backdrop style?");
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.9;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInviteInfo", infoStr, maxWidth, TextHAlignment::LEFT);
}


void InviteFriendSimpleBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(InviteFriendSimpleBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}
void InviteFriendSimpleBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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


void InviteFriendSimpleBoard::btnInviteCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		closeEffect();
		ClientCtrl::share()->reportEventParam("InviteFriend", "BackDropBoard");
		ClientCtrl::share()->googleInvite();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
