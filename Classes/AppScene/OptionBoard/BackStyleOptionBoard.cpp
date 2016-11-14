#include "BackStyleOptionBoard.h"
#include "SkinManager.h"
#include "LockManager.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

BackStyleOptionBoard::BackStyleOptionBoard()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BackStyleOptionBoard::msgRateSuccessCallBack), ST_RATE_SUCCESS, NULL);

}
BackStyleOptionBoard::~BackStyleOptionBoard()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool BackStyleOptionBoard::init()
{
	if (!OptionBaseBoard::init(kBackStyleBoard))
	{
		return false;
	}

	initRootCSBNode();

	handleButton();
	updateLanguage();

	refreshLockAndNew();
	int bgId = SkinManager::share()->getCardBackID();
	select(bgId - 1);
	return true;
}


void BackStyleOptionBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/CardBackStyle_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/CardBackStyle_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
	hideAllLock();
	hideAllNewSign();
}

void BackStyleOptionBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(BackStyleOptionBoard::btnCloseCallBack, this));


	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem1", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem2", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem3", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem4", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem5", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem6", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem7", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem8", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem9", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonItem10", CC_CALLBACK_2(BackStyleOptionBoard::selectBgCallBack, this));

}




void BackStyleOptionBoard::refreshLockAndNew()
{
	int num = 10;
	for (int i = 0; i < num; i++)
	{
		bool isNew = LockManager::share()->isBackStyleIsNew(i + 1);
		if (isNew)
		{
			showNewSignAt(i);
		}

		bool isLock = LockManager::share()->isBackStyleLock(i + 1);
		if (isLock)
		{
			showLockSignAt(i);
		}
	}

}


void BackStyleOptionBoard::resetInUseSignPos(int selectNum)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("ButtonItem%d", selectNum)->getCString());
	ST_RETURN_IF(!itemNode);
	Node *useNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgInUse");
	ST_RETURN_IF(!useNode);
	useNode->setPosition(itemNode->getPosition());

}


void BackStyleOptionBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(BackStyleOptionBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

int BackStyleOptionBoard::getNumFromString(string str)
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
void BackStyleOptionBoard::updateLanguage()
{
	string str = STLang::shared()->getValue("card back style");
	str = STLabelBMFont::convertAllLetterToCapital(str);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
}
void BackStyleOptionBoard::selectBgCallBack(Ref *pSender, Widget::TouchEventType type)
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
		sprintf(buf, "CardBack%d", num);
		ClientCtrl::share()->reportEventParam("SelectCardBack", buf);
		bool isNew = LockManager::share()->isBackStyleIsNew(num);
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

void BackStyleOptionBoard::setAllBgNotNew()
{
	//都不是最新的
	int sz = 10;
	for (int i = 0; i < sz; i++)
	{
		LockManager::share()->setBackStyleIsNew(i + 1, false);
	}
}

void BackStyleOptionBoard::select(int idx)
{
	bool isLock = LockManager::share()->isBackStyleLock(idx + 1);
	if (isLock)
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kRateBoard));
	}
	else
	{
		SkinManager::share()->selectCardBack(idx + 1);
		resetInUseSignPos(idx + 1);
		NotificationCenter::getInstance()->postNotification(ST_UPDATE_AFTER_SELECT);
	}
}

void BackStyleOptionBoard::msgRateSuccessCallBack(Ref *ref)
{
	int num = 10;
	for (int i = 0; i < num; i++)
	{
		bool isNew = LockManager::share()->isBackStyleIsNew(i + 1);
		if (isNew)
		{
			showNewSignAt(i);
		}

	}

	//删除lock标志
	hideAllLock();

}


void BackStyleOptionBoard::hideAllNewSign()
{
	int num = 10;
	for (int i = 0; i < num; i++)
	{
		CSBHelper::setChildVisible(m_rootCSBNode, String::createWithFormat("ImgNew%d", i + 1)->getCString(), false);
	}
}

void BackStyleOptionBoard::hideAllLock()
{
	int num = 10;
	for (int i = 0; i < num; i++)
	{
		CSBHelper::setChildVisible(m_rootCSBNode, String::createWithFormat("LockNode%d", i + 1)->getCString(), false);
	}
}


void BackStyleOptionBoard::showNewSignAt(int idx)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("ImgNew%d", idx + 1)->getCString());
	ST_RETURN_IF(!itemNode);
	itemNode->setVisible(true);
}

void BackStyleOptionBoard::showLockSignAt(int idx)
{
	Node *itemNode = ui::Helper::seekNodeByName(m_rootCSBNode, String::createWithFormat("LockNode%d", idx + 1)->getCString());
	ST_RETURN_IF(!itemNode);
	itemNode->setVisible(true);
}
void BackStyleOptionBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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