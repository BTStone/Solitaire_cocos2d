#include "RootOptionBoard.h"
#include "SkinManager.h"
#include "CardSprite.h"
#include "ClientCtrl.h"
#include "ScreenDirectionManager.h"
#include "InviteFriendBoard.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

const char *strIsAutoHint = "st_is_auto_hint";
const char *strIsDraw3 = "st_is_draw_3";
const char *strIsLeftHand = "st_is_left_hand";
RootOptionBoard::RootOptionBoard()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RootOptionBoard::msgUpdateUICallBack), ST_UPDATE_AFTER_SELECT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RootOptionBoard::msgUpdateLanguageCallBack), ST_UPDATE_LANGUAGE, NULL);

}
RootOptionBoard::~RootOptionBoard()
{
	NotificationCenter::getInstance()->removeAllObservers(this);

}

RootOptionBoard* RootOptionBoard::create()
{
	RootOptionBoard *re = new RootOptionBoard();
	if (re && re->init())
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool RootOptionBoard::init()
{
	if (!OptionBaseBoard::init(kRootOptionBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();


	updateCheckBoxState();
	updateTheme();
	updateScreenSignPosition();
	msgUpdateLanguageCallBack(NULL);

	return true;
}

void RootOptionBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Options_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Options_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void RootOptionBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "BtnBackDrop", CC_CALLBACK_2(RootOptionBoard::btnBackDropCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnFrontStyle", CC_CALLBACK_2(RootOptionBoard::btnFrontStyleCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnBackStyle", CC_CALLBACK_2(RootOptionBoard::btnBackStyleCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnLanguage", CC_CALLBACK_2(RootOptionBoard::btnLanguageCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnContactUs", CC_CALLBACK_2(RootOptionBoard::btnContactUsCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnInvite", CC_CALLBACK_2(RootOptionBoard::btnInviteCallBack, this));

	CSBHelper::registerButton(m_rootCSBNode, "BtnAuto", CC_CALLBACK_2(RootOptionBoard::btnAutoScreenCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnPortrait", CC_CALLBACK_2(RootOptionBoard::btnPortraitScreenCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnLandscape", CC_CALLBACK_2(RootOptionBoard::btnLandscapeScreenCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(RootOptionBoard::btnCloseCallBack, this));


	//check box
	CSBHelper::registerCheckBox(m_rootCSBNode, "CheckBoxHint", this, checkboxselectedeventselector(RootOptionBoard::boxSwitchHintCallBack));
	CSBHelper::registerCheckBox(m_rootCSBNode, "CheckBoxSound", this, checkboxselectedeventselector(RootOptionBoard::boxSwitchSoundCallBack));
	CSBHelper::registerCheckBox(m_rootCSBNode, "CheckBoxDraw3", this, checkboxselectedeventselector(RootOptionBoard::boxSwitchDrawCallBack));
	CSBHelper::registerCheckBox(m_rootCSBNode, "CheckBoxLeftHand", this, checkboxselectedeventselector(RootOptionBoard::boxSwitchLeftHandCallBack));
}

void RootOptionBoard::updateCheckBoxState()
{
	bool isHint = UserDefault::getInstance()->getBoolForKey(strIsAutoHint, true);
	bool isSoundOn = ClientCtrl::share()->isSoundOn();
	bool isDraw3 = UserDefault::getInstance()->getBoolForKey(strIsDraw3, false);
	bool isLeft = UserDefault::getInstance()->getBoolForKey(strIsLeftHand, false);

	CSBHelper::setCheckBoxSelected(m_rootCSBNode, "CheckBoxHint", isHint);
	CSBHelper::setCheckBoxSelected(m_rootCSBNode, "CheckBoxSound", isSoundOn);
	CSBHelper::setCheckBoxSelected(m_rootCSBNode, "CheckBoxDraw3", isDraw3);
	CSBHelper::setCheckBoxSelected(m_rootCSBNode, "CheckBoxLeftHand", isLeft);
}

void RootOptionBoard::updateTheme()
{
	int gameBgID = SkinManager::share()->getGameBgID();
	int cardBgID = SkinManager::share()->getCardBackID();
	int cardFrontID = SkinManager::share()->getCardFaceID();

	string gameBgName = SkinManager::share()->getGameBgFileNameByGameBgId(gameBgID);
	string cardBgName = SkinManager::share()->getCardBgFileNameByCardBgId(cardBgID);
	string frontName = String::createWithFormat("Pic/Card-Style-%02d.png", cardFrontID)->getCString();

	CSBHelper::loadImageViewTexture(m_rootCSBNode, "GameBgShow", gameBgName);
	CSBHelper::loadImageViewTexture(m_rootCSBNode, "CardBackShow", cardBgName);
	CSBHelper::loadImageViewTexture(m_rootCSBNode, "CardFrontShow", frontName);
}

void RootOptionBoard::updateScreenSignPosition()
{
	int dic = ScreenDirectionManager::share()->getSelectDic();
	Node *autoSign = ui::Helper::seekNodeByName(m_rootCSBNode, "ScreenSignAuto");
	Node *portSign = ui::Helper::seekNodeByName(m_rootCSBNode, "ScreenSignPort");
	Node *landSign = ui::Helper::seekNodeByName(m_rootCSBNode, "ScreenSignLand");
	ST_RETURN_IF(!(autoSign && portSign &&landSign));
	autoSign->setVisible(false);
	portSign->setVisible(false);
	landSign->setVisible(false);
	Vec2 pos = Vec2::ZERO;
	if (dic == 0)
	{
		autoSign->setVisible(true);
	}
	else if (dic == 1)
	{
		portSign->setVisible(true);
	}
	else if (dic == -1)
	{
		landSign->setVisible(true);
	}
}


void RootOptionBoard::switchHintCallBack(Ref *ref)
{
	bool isAuto = UserDefault::getInstance()->getBoolForKey(strIsAutoHint, true);
	UserDefault::getInstance()->setBoolForKey(strIsAutoHint, !isAuto);
	if (isAuto)
	{
		ClientCtrl::share()->reportEventParam("ClickManualButton");
	}
	else
	{
		ClientCtrl::share()->reportEventParam("ClickAutoButton");
	}
}
void RootOptionBoard::switchSoundCallBack(Ref *ref)
{
	bool onSound = ClientCtrl::share()->isSoundOn();
	ClientCtrl::share()->soundSwitch();
	if (onSound)
	{
		ClientCtrl::share()->reportEventParam("SoundOff");
	}
}
void RootOptionBoard::switchDrawCallBack(Ref *ref)
{
	bool isDraw3 = UserDefault::getInstance()->getBoolForKey(strIsDraw3, false);
	UserDefault::getInstance()->setBoolForKey(strIsDraw3, !isDraw3);
}

void RootOptionBoard::switchLeftHandCallBack(Ref *ref)
{
	bool isLeft = UserDefault::getInstance()->getBoolForKey(strIsLeftHand, false);
	UserDefault::getInstance()->setBoolForKey(strIsLeftHand, !isLeft);
	NotificationCenter::getInstance()->postNotification(ST_SWITCH_HAND_TYPE, (Ref*)(!isLeft));
	if (!isLeft)
	{
		ClientCtrl::share()->reportEventParam("LeftHandOn");
	}
}

void RootOptionBoard::msgUpdateUICallBack(Ref *ref)
{
	updateTheme();
}

void RootOptionBoard::msgUpdateLanguageCallBack(Ref *ref)
{
	int menuNum = 15;
	const char *strKey[] = { "hint", "sound", "draw 3", "LeftHand", "backdrop", "front style",
		"back style", "selectLanguage", "ContactUs", "Orientation", "Invite Friends","auto","Portrait","Landscape", "options" };

	const char *csbNames[] = { "TextHint", "TextSound", "TextDraw3", "TextLeftHand", "TextBackDrop", "TextFrontStyle",
		"TextBackStyle", "TextLanguage", "TextContactUs", "TextScreen", "TextInvite", "TextAuto", "TextPortrait", "TextLandscape", "TextTitle" };

	for (int i = 0; i < menuNum; i++)
	{
		string str = STLang::shared()->getValue(strKey[i]);
		if (i == 14)//title
		{
			str = STLabelBMFont::convertAllLetterToCapital(str);
			CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
		}
		else
		{
			str = STLabelBMFont::convertFirstLetterToCapital(str);
			CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
		}
	}
}

void RootOptionBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(RootOptionBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}

void RootOptionBoard::boxSwitchHintCallBack(Ref *ref, CheckBoxEventType tp)
{
	switchHintCallBack(NULL);
	
}


void RootOptionBoard::boxSwitchSoundCallBack(Ref *ref, CheckBoxEventType tp)
{
	switchSoundCallBack(NULL);
	
}


void RootOptionBoard::boxSwitchDrawCallBack(Ref *ref, CheckBoxEventType tp)
{
	switchDrawCallBack(NULL);
	
}

void RootOptionBoard::boxSwitchLeftHandCallBack(Ref *ref, CheckBoxEventType tp)
{
	switchLeftHandCallBack(NULL);
	
}


void RootOptionBoard::btnBackDropCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kBackDropBoard));
		ClientCtrl::share()->reportEventParam("ClickBackDropButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void RootOptionBoard::btnFrontStyleCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kFrontStyleBoard));
		ClientCtrl::share()->reportEventParam("ClickCardFrontButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void RootOptionBoard::btnBackStyleCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kBackStyleBoard));
		ClientCtrl::share()->reportEventParam("ClickCardBackButton");
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void RootOptionBoard::btnLanguageCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)(kLanguageSelectBoard));
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void RootOptionBoard::btnContactUsCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		ClientCtrl::share()->contactUs();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void RootOptionBoard::btnInviteCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		InviteFriendBoard::setResString("RootOptionBoard");
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kInviteFriendBoard);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void RootOptionBoard::btnAutoScreenCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button *bt = dynamic_cast<Button*>(pSender);
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		ScreenDirectionManager::share()->setSelectDic(0);
		updateScreenSignPosition();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void RootOptionBoard::btnPortraitScreenCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button *bt = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		ScreenDirectionManager::share()->setSelectDic(1);
		updateScreenSignPosition();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void RootOptionBoard::btnLandscapeScreenCallBack(Ref *pSender, Widget::TouchEventType type)
{
	Button *bt = dynamic_cast<Button*>(pSender);
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		ScreenDirectionManager::share()->setSelectDic(-1);
		updateScreenSignPosition();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void RootOptionBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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