#include "BackDropBoard.h"
#include "SkinManager.h"
#include "LockManager.h"
#include "ClientCtrl.h"
#include "CardSprite.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

//======================================================BackDropBoard begin========================================

BackDropBoard::BackDropBoard()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BackDropBoard::msgInviteSuccessCallBack), ST_INVITE_SUCCESS, NULL);

}
BackDropBoard::~BackDropBoard()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool BackDropBoard::init()
{
	if (!OptionBaseBoard::init(kBackDropBoard))
	{
		return false;
	}
	initRootCSBNode();

	handleButton();
	updateLanguage();

	refreshLockAndNew();

	int gameBgId = SkinManager::share()->getGameBgID();
	select(gameBgId - 1);
	return true;
}

void BackDropBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Backdrop_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Backdrop_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
	hideAllLock();
	hideAllNewSign();
}

void BackDropBoard::refreshLockAndNew()
{
	int num = 9;
	for (int i = 0; i < num; i++)
	{
		bool isNew = LockManager::share()->isBgIsNew(i + 1);
		if (isNew)
		{
			showNewSignAt(i);
		}

		bool isLock = LockManager::share()->isBgLock(i + 1);
		if (isLock)
		{
			showLockSignAt(i);
		}
	}

}

void BackDropBoard::setAllBgNotNew()
{
	//都不是最新的
	int sz = 9;
	for (int i = 0; i < sz; i++)
	{
		LockManager::share()->setBgIsNew(i + 1, false);
	}
}

void BackDropBoard::hideAllLock()
{
	int num = 9;
	for (int i = 0; i < num; i++)
	{
		CSBHelper::setChildVisible(m_rootCSBNode, String::createWithFormat("LockNode%d", i + 1)->getCString(), false);
	}
}

void BackDropBoard::hideAllNewSign()
{
	int num = 9;
	for (int i = 0; i < num; i++)
	{
		CSBHelper::setChildVisible(m_rootCSBNode, String::createWithFormat("ImgNew%d", i + 1)->getCString(), false);
	}
}

void BackDropBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(BackDropBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem1", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem2", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem3", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem4", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem5", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem6", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem7", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem8", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem9", CC_CALLBACK_2(BackDropBoard::selectBgCallBack, this));

}



void BackDropBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(BackDropBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

void BackDropBoard::updateLanguage()
{
	string str = STLang::shared()->getValue("backdrop");
	str = STLabelBMFont::convertAllLetterToCapital(str);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
}

int BackDropBoard::getNumFromString(string str)
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

void BackDropBoard::selectBgCallBack(Ref *pSender, Widget::TouchEventType type)
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
	{
		select(num - 1);
		char buf[64];
		sprintf(buf, "GameBg%d", num);
		ClientCtrl::share()->reportEventParam("SelectGameBG", buf);
		bool isNew = LockManager::share()->isBgIsNew(num);
		if (isNew)
		{
			hideAllNewSign();
			setAllBgNotNew();
		}
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void BackDropBoard::msgInviteSuccessCallBack(Ref *ref)
{
	int num = 9;
	for (int i = 0; i < num; i++)
	{
		bool isNew = LockManager::share()->isBgIsNew(i + 1);
		if (isNew)
		{
			showNewSignAt(i);
		}
		
	}

	//删除lock标志
	hideAllLock();
}



void BackDropBoard::select(int idx)
{
	bool isLock = LockManager::share()->isBgLock(idx + 1);
	if (isLock)
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kInviteFriendSimpleBoard));
	}
	else
	{
		SkinManager::share()->selectGameBg(idx + 1);
		resetInUseSignPos(idx + 1);
		NotificationCenter::getInstance()->postNotification(ST_UPDATE_AFTER_SELECT);
	}

	
}

void BackDropBoard::resetInUseSignPos(int selectNum)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("ButtonItem%d", selectNum)->getCString());
	ST_RETURN_IF(!itemNode);
	Node *useNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgInUse");
	ST_RETURN_IF(!useNode);
	useNode->setPosition(itemNode->getPosition());

}

void BackDropBoard::showNewSignAt(int idx)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("ImgNew%d", idx + 1)->getCString());
	ST_RETURN_IF(!itemNode);
	itemNode->setVisible(true);
}

void BackDropBoard::showLockSignAt(int idx)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("LockNode%d", idx + 1)->getCString());
	ST_RETURN_IF(!itemNode);
	itemNode->setVisible(true);
}

void BackDropBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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

//======================================================BackDropBoard end========================================