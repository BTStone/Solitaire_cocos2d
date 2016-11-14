#include "PlayBoard.h"
#include "OptionBaseBoard.h"
#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"

bool PlayBoard::m_isShowing = false;

PlayBoard* PlayBoard::create(bool canReplay /* = true */)
{
	PlayBoard *re = new PlayBoard();
	if (re && re->init(canReplay))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	return NULL;
}

bool PlayBoard::init(bool canReplay)
{
	if (!Node::init())
	{
		return false;
	}
	m_canReplay = canReplay;
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	m_canReplay = canReplay && (playNum > 0);

	m_isShowing = true;

	initBg();
	addSeperateMenu();
	initMenu();
	ClientCtrl::share()->tryToPreGameAds();
	return true;
}

void PlayBoard::initBg()
{
	Size winSize = CLDWinSize();/*
	if (winSize.width > winSize.height)
	{
		winSize.width = winSize.height*1.0;
	}
	*/
	m_bg = Scale9Sprite::create("Pic/UI-Box-MenuBox-Bg.png");
	ST_RETURN_IF(!m_bg);
	float bgWidth = winSize.width*0.7;
	//float bgHeight = winSize.height*0.4;
	float bgHeight = winSize.height*0.3;

	bool isPortrait = ScreenDirectionManager::share()->getScreenType() == kScreenPortrait;
	if (!isPortrait)
	{
		//bgHeight = winSize.height*0.7;
		bgHeight = winSize.width*0.3;
		bgWidth = winSize.height*0.7;
	}

	bgHeight = 450;
	bgWidth = 500;


	bgHeight = 540;
	bgWidth = 540;

	m_bg->setContentSize(Size(bgWidth, bgHeight));
	//m_bg->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_bg, 0);
}

void PlayBoard::initMenu()
{
	SEL_MenuHandler handles[] = {
		menu_selector(PlayBoard::menuNewGameCallBack),
		menu_selector(PlayBoard::menuReplayCallBack),
		menu_selector(PlayBoard::menuStatisticCallBack),
		menu_selector(PlayBoard::menuScoreCallBack),
		menu_selector(PlayBoard::menuSevenDaysCallBack)
	};
	const char *normalPics[] = { 
		"Pic/UI-Button-Blue.png",
		"Pic/UI-Button-DarkBlue.png",
		"Pic/UI-Button-yellow.png",
		"Pic/UI-Button-Orange.png",
		"Pic/UI-Button-Blue.png"
	};
	const char *selectPics[] = {
		"Pic/UI-Button-Blue-Press.png",
		"Pic/UI-Button-DarkBlue-press.png",
		"Pic/UI-Button-yellow-press.png",
		"Pic/UI-Button-Orange-Press.png",
		"Pic/UI-Button-Blue-Press.png"
	};
	const char *names[] = {
		"NEW GAME",
		"REPLAY",
		"STATISTICS",
		"SCORES",
		"7 DAYS"
	};

	int menuNum = 4;
	Size bgSize = getBgSize();
	//Size realSize = Size(bgSize.width, bgSize.height*0.88);   //去除箭头部分以及上下部分
	float offY = 105;
	float beginY = 175;
	//float beginY = bgSize.height*0.39;
	//float offY = bgSize.height*0.18;
	for (int i = 0; i < menuNum; i++)
	{
		string normalStr = normalPics[i];
		string selectStr = selectPics[i];
		SEL_MenuHandler selector= handles[i];
		if (i == 1 && !m_canReplay)
		{
			normalStr = "Pic/UI-Button-Grey-Press.png";
			selectStr = "Pic/UI-Button-Grey-Press.png";
			selector = NULL;
		}
		Size winSize = CLDWinSize();
		Scale9Sprite *s1 = Scale9Sprite::create(normalStr);
		ST_RETURN_IF(!s1);
		//Size spSize = Size(bgSize.width*0.9, offY*0.85);修改大小
		Size spSize = Size(450, 100);// Size(bgSize.width*0.9, offY*0.9);
		s1->setContentSize(spSize);
		Scale9Sprite *s2 = Scale9Sprite::create(selectStr);
		ST_RETURN_IF(!s2);
		s2->setContentSize(spSize);
		MenuItemSprite * item = NULL;
		if (!selector)
		{
			item = MenuItemSprite::create(s1, s2);
		}
		else
		{
			item = MenuItemSprite::create(s1, s2, NULL, this, selector);
		}
		ST_RETURN_IF(!item);

		Menu *menu = Menu::create(item, NULL);
		ST_RETURN_IF(!menu);
		menu->setPosition(Vec2(0, beginY - i * offY));
		this->addChild(menu, 1);
		menu->setVisible(false);
		menu->setTag(100 + i);

		string labelStr = STLang::shared()->getValue(names[i]);
		STLabelBMFont *label = STLabelBMFont::createWithBMFont("EurostileBold.fnt", labelStr, true, true);
		//LabelTTF *label = LabelTTF::create(names[i], "arial", spSize.height*0.6);
		ST_RETURN_IF(!label);
		label->setTextSize(Size(spSize.width*0.95, spSize.height*0.6));
		float sccale = label->getScale();
		if (sccale > 0.7f)
		{
			label->setScale(0.7f);
		}
		
		label->setPosition(menu->getPosition() + Vec2(0, spSize.height*0));
		this->addChild(label, 1);
		label->setTag(200 + i);
		label->setVisible(false);
	}
	addMenuAppearEffect();
}

void PlayBoard::addMenuAppearEffect()
{
	int menuNum = 4;
	for (int i = 0; i < menuNum; i++)
	{
		Node *menuNode = this->getChildByTag(100 + i);
		Node *labelNode = this->getChildByTag(200 + i);
		ST_RETURN_IF((!menuNode) || (!labelNode));
		labelNode->setPosition(menuNode->getPosition());
		labelNode->setScale(0.05f);
		float dt = 0.2f;
		float delayDt = 0.2f + i*0.1f;
		DelayTime *delay1 = DelayTime::create(delayDt);
		ST_RETURN_IF(!delay1);
		CallFuncN *callFunc1 = CallFuncN::create(this, callfuncN_selector(PlayBoard::showSelfCallFunc));
		ST_RETURN_IF(!callFunc1);
		Sequence *seq1 = Sequence::create(delay1, callFunc1, NULL);
		ST_RETURN_IF(!seq1);
		menuNode->runAction(seq1);

		DelayTime *delay2 = DelayTime::create(delayDt);
		ST_RETURN_IF(!delay2);
		CallFuncN *callFunc2 = CallFuncN::create(this, callfuncN_selector(PlayBoard::showSelfCallFunc));
		ST_RETURN_IF(!callFunc2);
		ScaleTo *scaleTo2 = ScaleTo::create(dt, 0.7f, 0.7f);
		ST_RETURN_IF(!scaleTo2);
		EaseElasticOut *ease2 = EaseElasticOut::create(scaleTo2);
		ST_RETURN_IF(!ease2);
		Sequence *seq2 = NULL;
		if (i == (menuNum - 1))
		{
			CallFunc *endCall = CallFunc::create(this, callfunc_selector(PlayBoard::tryToShowGameAdCallFunc));
			ST_RETURN_IF(!endCall);
			seq2 = Sequence::create(delay1, callFunc2, ease2, endCall,NULL);
		}
		else
		{
			seq2 = Sequence::create(delay1, callFunc2, ease2, NULL);
		}
		
		ST_RETURN_IF(!seq2);
		labelNode->runAction(seq2);
	}
	
}


void PlayBoard::showSelfCallFunc(Node *node)
{
	ST_RETURN_IF(!node);
	node->setVisible(true);
}

void PlayBoard::addSeperateMenu()
{
	//添加隔绝点击菜单，并且使点击时候关闭面板
	Node * spaceNode = Node::create();
	ST_RETURN_IF(!spaceNode);
	spaceNode->setContentSize(CCSizeMake(CLDWinSize().width * 3, CLDWinSize().height * 3));
	MenuItemSprite * menuSeparate = MenuItemSprite::create(
		spaceNode,
		spaceNode,
		NULL,
		this,
		menu_selector(PlayBoard::closeCallBack));
	ST_RETURN_IF(!menuSeparate);

	Menu * pMenu = Menu::createWithItem(menuSeparate);
	ST_RETURN_IF(!pMenu);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);
}

void PlayBoard::closeCallBack(Ref *ref)
{
	disappearEffect(); 
}

Size PlayBoard::getBgSize()
{
	return m_bg->getContentSize()*m_bg->getScale();
}

void PlayBoard::appearEffect(bool isEffect)
{
	Size bgSize = getBgSize();
	if (!isEffect)
	{
		this->setPosition(this->getPosition() + Vec2(0, bgSize.height*0.4));
		return;
	}
	this->setScale(0);
	float dt = 0.1f;
	ScaleTo *scaleTo = ScaleTo::create(dt, 0.8f);
	ST_RETURN_IF(!scaleTo);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(PlayBoard::addEaseEffectCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(scaleTo, callFunc, NULL);
	ST_RETURN_IF(!seq);

	this->runAction(seq);
	MoveBy *mvBy = MoveBy::create(dt, Vec2(0, bgSize.height*0.4));
	ST_RETURN_IF(!mvBy);
	this->runAction(mvBy);
}

void PlayBoard::addEaseEffectCallFunc(Node *node)
{
	if (node)
	{
		float dt = 0.4f;
		ScaleTo *scaleTo = ScaleTo::create(dt, 1.0f, 1.0f);
		ST_RETURN_IF(!scaleTo);
		EaseElasticOut *ease = EaseElasticOut::create(scaleTo);
		ST_RETURN_IF(!ease);
		this->runAction(ease);
	}
}

void PlayBoard::tryToShowGameAdCallFunc()
{
	ClientCtrl::share()->tryToShowGameAds();
}

void PlayBoard::disappearEffect()
{
	float dt = 0.2f;
	ScaleTo *scaleTo = ScaleTo::create(dt, 0);
	ST_RETURN_IF(!scaleTo);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(PlayBoard::removeSelf));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(scaleTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	Size bgSize = getBgSize();
	MoveBy *mvBy = MoveBy::create(dt, Vec2(0, -bgSize.height*0.5));
	ST_RETURN_IF(!mvBy);
	this->runAction(mvBy);

	m_isShowing = false;
	ClientCtrl::share()->tryToHideBanner();
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);
}


void PlayBoard::removeSelf()
{
	this->removeFromParent();
}

void PlayBoard::menuNewGameCallBack(Ref *ref)
{
	disappearEffect();
	NotificationCenter::getInstance()->postNotification(ST_NEW_GAME);

	ClientCtrl::share()->reportEventParam("ClickPlayBoardNewGameButton");
}
void PlayBoard::menuReplayCallBack(Ref *ref)
{
	disappearEffect();
	NotificationCenter::getInstance()->postNotification(ST_REPLAY_GAME);

}
void PlayBoard::menuStatisticCallBack(Ref *ref)
{
	disappearEffect();
	NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)kStatisticsBoard);
	ClientCtrl::share()->reportEventParam("ClickStatisticsButton");
}
void PlayBoard::menuScoreCallBack(Ref *ref)
{
	disappearEffect();
	ClientCtrl::share()->reportEventParam("ClickScoreButton");
	ClientCtrl::share()->googleRank();
}
void PlayBoard::menuSevenDaysCallBack(Ref *ref)
{
	disappearEffect();
}

bool PlayBoard::isShowing()
{
	return m_isShowing;
}
