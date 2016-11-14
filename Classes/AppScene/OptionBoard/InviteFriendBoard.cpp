#include "InviteFriendBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

string InviteFriendBoard::m_resString = "";

bool InviteFriendBoard::init()
{
	if (!OptionBaseBoard::init(kInviteFriendBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	return true;
}


void InviteFriendBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/InviteFriends_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/InviteFriends_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void InviteFriendBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(InviteFriendBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonInvite", CC_CALLBACK_2(InviteFriendBoard::btnInviteCallBack, this));

}

void InviteFriendBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("Invite Friends");
	titleStr = STLabelBMFont::convertAllLetterToCapital(titleStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//invite menu
	string inviteStr = STLang::shared()->getValue("Invite Button");
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInvite", inviteStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//invite info
	string infoStr = STLang::shared()->getValue("Invite Friends to unlock new Backdrop style?");
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.95;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInviteInfo", infoStr, maxWidth, TextHAlignment::LEFT);
}


void InviteFriendBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(InviteFriendBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}
void InviteFriendBoard::btnInviteCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		closeEffect();
		ClientCtrl::share()->reportEventParam("InviteFriend", m_resString.c_str());
		ClientCtrl::share()->googleInvite();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void InviteFriendBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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
void InviteFriendBoard::setResString(string resStr)
{
	m_resString = resStr;
}