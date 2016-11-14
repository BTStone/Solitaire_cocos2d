#include "FrontStyleOptionBoard.h"
#include "SkinManager.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

bool FrontStyleOptionBoard::init()
{
	if (!OptionBaseBoard::init(kFrontStyleBoard))
	{
		return false;
	}
	initRootCSBNode();

	handleButton();
	updateLanguage();

	int bgId = SkinManager::share()->getCardFaceID();
	select(bgId - 1);
	return true;
}

void FrontStyleOptionBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/CardFrontStyle_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/CardFrontStyle_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void FrontStyleOptionBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(FrontStyleOptionBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem1", CC_CALLBACK_2(FrontStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem2", CC_CALLBACK_2(FrontStyleOptionBoard::selectBgCallBack, this));

}

void FrontStyleOptionBoard::updateLanguage()
{
	string str = STLang::shared()->getValue("card front style");
	str = STLabelBMFont::convertAllLetterToCapital(str);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
}
void FrontStyleOptionBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(FrontStyleOptionBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

int FrontStyleOptionBoard::getNumFromString(string str)
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


void FrontStyleOptionBoard::selectBgCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button *bt = (Button*)(pSender);
	string name = bt->getName();

	int num = getNumFromString(name);
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		select(num - 1);

		char buf[64];
		sprintf(buf, "CardFront%d", num);
		ClientCtrl::share()->reportEventParam("SelectCardFront", buf);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void FrontStyleOptionBoard::resetInUseSignPos(int selectNum)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("ButtonItem%d", selectNum)->getCString());
	ST_RETURN_IF(!itemNode);
	Node *useNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgInUse");
	ST_RETURN_IF(!useNode);
	useNode->setPosition(itemNode->getPosition());

}
void FrontStyleOptionBoard::select(int idx)
{
	SkinManager::share()->selectCardFace(idx + 1);

	resetInUseSignPos(idx + 1);

	NotificationCenter::getInstance()->postNotification(ST_UPDATE_AFTER_SELECT);
}


void FrontStyleOptionBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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