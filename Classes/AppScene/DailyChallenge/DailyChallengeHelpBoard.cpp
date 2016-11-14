#include "DailyChallengeHelpBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"


bool DailyChallengeHelpBoard::init()
{
	if (!OptionBaseBoard::init(kDailyChallengeHelpBoard))
	{
		return false;
	}
	m_curIdx = UserDefault::getInstance()->getIntegerForKey(STRG_LAST_CHALLENGE_HELP_IDX, 0);
	m_curIdx = (m_curIdx < 0 || m_curIdx>3) ? 0 : m_curIdx;
	initRootCSBNode();
	handleButton();
	updateLanguage();
	refresh();
	return true;
}


void DailyChallengeHelpBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/DailyChallengeHelp_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/DailyChallengeHelp_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void DailyChallengeHelpBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(DailyChallengeHelpBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonLeft", CC_CALLBACK_2(DailyChallengeHelpBoard::btnLeftCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "ButtonRight", CC_CALLBACK_2(DailyChallengeHelpBoard::btnRightCallBack, this));

}

void DailyChallengeHelpBoard::updateLanguage()
{
	//title
	string titleStr = STLang::shared()->getValue("daily information");
	titleStr = STLabelBMFont::convertAllLetterToCapital(titleStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));

	//invite infoint infoNum = 4;
	const char *infos[] = { "[000000]Every day a new special deal unlocks for you to play.[-]",
		"[000000]Beating any deal on the challenge will earn you a [ffaa00]crown[-] for that day.[-]",
		"[000000]But only one [ffaa00]platinum crown[-] will be left for the best day.[-]",
		"[000000]The more days of the month you play and win.the better your [ffaa00]trophy[-] will be.[-]"
	};
	string infoStr = STLang::shared()->getValue(infos[m_curIdx]);
	infoStr = STLabelBMFont::filterColorSign(infoStr);
	Size bgSize = getBgSize();
	float maxWidth = bgSize.width*0.9;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextInfo", infoStr, maxWidth, TextHAlignment::LEFT);
}

void DailyChallengeHelpBoard::updateCenterShow()
{
	Node *centerNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgCenterShow");
	ImageView *imgCenter = dynamic_cast<ImageView*>(centerNode);
	if (imgCenter)
	{
		string picName = String::createWithFormat("Pic/Img_DaliyGuide%02d.png", m_curIdx + 1)->getCString();
		imgCenter->loadTexture(picName);
	}
}

void DailyChallengeHelpBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(DailyChallengeHelpBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);

}
void DailyChallengeHelpBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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


void DailyChallengeHelpBoard::btnLeftCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		m_curIdx--;
		int minIdx = 0;
		int maxIdx = 3;
		if (m_curIdx < minIdx)
		{
			m_curIdx = maxIdx;
		}
		UserDefault::getInstance()->setIntegerForKey(STRG_LAST_CHALLENGE_HELP_IDX, m_curIdx);
		refresh();
		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void DailyChallengeHelpBoard::btnRightCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		m_curIdx++;
		int minIdx = 0;
		int maxIdx = 3;
		if (m_curIdx > maxIdx)
		{
			m_curIdx = minIdx;
		}
		UserDefault::getInstance()->setIntegerForKey(STRG_LAST_CHALLENGE_HELP_IDX, m_curIdx);
		refresh();
		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void DailyChallengeHelpBoard::refresh()
{
	updateLanguage();
	updateCenterShow();
}