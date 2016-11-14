#include "StartLayer.h"
#include "GameLayer.h"
#include "ClientCtrl.h"
#include "SkinManager.h"

const char *strInstallYear = "st_install_year";
const char *strInstallMonth = "st_install_month";
const char *strinstallDay = "st_install_day";

StartLayer::StartLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartLayer::msgSwitchScreen), ST_SWITCH_SCREEN, NULL);
}


StartLayer::~StartLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool StartLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (!isPortrait)
	{
		setFrameAndDesignSizeWhenLandscape();
	}

	m_cardNode = NULL;
	m_appName = NULL;
	m_light = NULL;
	m_logo = NULL;
	initLogo();
	ClientCtrl::share()->hideBanner();
	//ClientCtrl::share()->showBanner();
	return true;
}

void StartLayer::loadPic()
{
	char bgName[64];
	int num = SkinManager::share()->getGameBgFileNames().size();
	for (int i = 1; i <= num; i++)
	{
		string bgName = SkinManager::share()->getGameBgFileNameByGameBgId(i);
		Sprite* custom_button = Sprite::create(bgName);
		ST_RETURN_IF(!custom_button);
		custom_button->setScale(0.1f);
		custom_button->setTag(i);
		custom_button->setPosition(Vec2(2000, 2000));
		this->addChild(custom_button);
	}
}

void StartLayer::initLogo()
{
	m_logo = Sprite::create("Pic/CompanyLogo.jpg");
	ST_RETURN_IF(!m_logo);
	m_logo->setPosition(CLDWinCenter());
	this->addChild(m_logo);
	Size winSize = CLDWinSize();
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Size logoSize = m_logo->getContentSize();
	float widthScale = winSize.width / logoSize.width;
	float heightScale = winSize.height / logoSize.height;
	float logoScale = min(widthScale, heightScale);
	m_logo->setScale(logoScale);
	
	float dt = 0.7f;
	DelayTime *delay = DelayTime::create(dt);
	ST_RETURN_IF(!delay);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(StartLayer::logoShowOverCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(delay, callFunc, NULL);
	ST_RETURN_IF(!seq);
	m_logo->runAction(seq);
}

void StartLayer::addCardShowEffect()
{
	log("addCardShowEffect");
	Size winSize = CLDWinSize();
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	m_cardNode = Node::create();
	ST_RETURN_IF(!m_cardNode);
	m_cardNode->setPosition(CLDWinPos(0.5, 0.6));
	this->addChild(m_cardNode);
	
	int colorNum = 4;
	char *picNames[] = { "Pic/Card-A-Spades.png", "Pic/Card-A-Hearts.png", "Pic/Card-A-Clubs.png", "Pic/Card-A-Diamonds.png" };
	for (int i = 0; i < colorNum; i++)
	{
		Sprite *cd = Sprite::create(picNames[i]);
		ST_RETURN_IF(!cd);
		Size cdSize = cd->getContentSize();
		float offsetX = cdSize.width*0.1;
		float beginX = -1.5 * offsetX;
		cd->setAnchorPoint(Vec2(0.5, 0));
		cd->setPosition(Vec2(beginX + offsetX*i, 0));
		m_cardNode->addChild(cd);

		float dt = 0.3f;
		float offsetSingle = 20.0f;
		float beginSingle = -1.5 * offsetSingle;
		RotateTo *rotateTo = RotateTo::create(dt, beginSingle+offsetSingle*i);
		ST_RETURN_IF(!rotateTo);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(StartLayer::addAppNameShowEffect));
		ST_RETURN_IF(!callFunc);
		Sequence *seq = NULL;
		if (i < colorNum - 1)
		{
			seq = Sequence::create(rotateTo, NULL);
		}
		else
		{
			seq = Sequence::create(rotateTo, callFunc, NULL);
		}
		ST_RETURN_IF(!seq);
		cd->runAction(seq);
	}
}

void StartLayer::addAppNameShowEffect()
{
	Size winSize = CLDWinSize();
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	m_light = Sprite::create("Pic/UI-Login-PokerAMLight.png");
	ST_RETURN_IF(!m_light);
	Size lightSize = m_light->getContentSize();
	//light->setContentSize(lightSize);
	m_light->setScale(0);
	m_light->setPosition(CLDWinPos(0.5, 0.4));
	this->addChild(m_light);

	m_appName = Sprite::create("Pic/UI-Login-LogoText.png");
	ST_RETURN_IF(!m_appName);
	m_appName->setPosition(CLDWinPos(0.5, 0.4));
	m_appName->setScale(0);
	this->addChild(m_appName);
	float appNameMaxScale = 1.0f;
	
	Size appNameSize = m_appName->getContentSize() * m_appName->getScale();
	float dt1 = 0.2f;
	DelayTime *delay1 = DelayTime::create(dt1);
	ST_RETURN_IF(!delay1);
	ScaleTo *lightScaleTo1 = ScaleTo::create(0.15f, appNameSize.width*1.2 / lightSize.width, 0.2f);
	ST_RETURN_IF(!lightScaleTo1);
	ScaleTo *lightScaleTo2 = ScaleTo::create(0.15f, 0.05f, 0.05f);
	ST_RETURN_IF(!lightScaleTo2);
	Sequence *seq1 = Sequence::create(delay1, lightScaleTo1, lightScaleTo2, NULL);
	ST_RETURN_IF(!seq1);
	m_light->runAction(seq1);

	float dt2 = 0.28f;
	DelayTime *delay2 = DelayTime::create(dt2);
	ST_RETURN_IF(!delay2);
	ScaleTo *appScaleTo = ScaleTo::create(0.3f, appNameMaxScale, appNameMaxScale);
	ST_RETURN_IF(!appScaleTo);
	DelayTime *delay3 = DelayTime::create(0.1f);
	ST_RETURN_IF(!delay3);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(StartLayer::appNameShowOverCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq2 = Sequence::create(delay2, appScaleTo, delay3, callFunc, NULL);
	ST_RETURN_IF(!seq2);
	m_appName->runAction(seq2);

}

void StartLayer::logoShowOverCallFunc()
{
	ST_SAFE_REMOVE_FROM_PARENT(m_logo);
	addCardShowEffect();
}

void StartLayer::cardShowOverCallFunc()
{

}

void StartLayer::appNameShowOverCallFunc()
{
	loadPic();
	Director::getInstance()->replaceScene(GameLayer::scene());
}

void StartLayer::recordInstallData()
{
	int enterNum = UserDefault::getInstance()->getIntegerForKey(STRG_ENTER_GAME_NUM, 0);
	if (enterNum == 0)
	{

		//UserDefault::getInstance()->setIntegerForKey(strInstallYear, )
	}
}

void StartLayer::msgSwitchScreen(Ref *ref)
{
	bool isPortrait = (bool)(ref);
	if (isPortrait)
	{
		this->setPortraitStatus();
	}
	else
	{
		this->setLandscapeStatus();
	}
}


void StartLayer::setFrameAndDesignSizeWhenLandscape()
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();
	Size frameSize = glview->getFrameSize();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	frameSize.width = ClientCtrl::share()->getScreenWidth();
	frameSize.height = ClientCtrl::share()->getScreenHeight();
#endif
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	glview->setFrameSize(bigNum, smallNum);

	Size normalFrameSize = Size(smallNum, bigNum);
	Size resSize = Size(720, 1280);
	glview->setDesignResolutionSize(resSize.width,
		resSize.width * normalFrameSize.height / normalFrameSize.width, kResolutionShowAll);

	Size designSize = glview->getDesignResolutionSize();

	if (designSize.width < designSize.height)
	{
		glview->setDesignResolutionSize(designSize.height, designSize.width, kResolutionShowAll);
	}
	else
	{
		glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	}
}

void StartLayer::setFrameAndDesignSizeWhenPortrait()
{
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();

	Size frameSize = glview->getFrameSize();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	frameSize.width = ClientCtrl::share()->getScreenWidth();
	frameSize.height = ClientCtrl::share()->getScreenHeight();
#endif
	float bigNum = max(frameSize.width, frameSize.height);
	float smallNum = min(frameSize.width, frameSize.height);
	glview->setFrameSize(smallNum, bigNum);

	Size normalFrameSize = Size(smallNum, bigNum);
	Size resSize = Size(720, 1280);
	glview->setDesignResolutionSize(resSize.width,
		resSize.width * normalFrameSize.height / normalFrameSize.width, kResolutionShowAll);

	Size designSize = glview->getDesignResolutionSize();
	if (designSize.width > designSize.height)
	{
		glview->setDesignResolutionSize(designSize.height, designSize.width, kResolutionShowAll);
	}
	else
	{
		glview->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
	}
}

void StartLayer::setPortraitStatus()
{

	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(0.1f),
		CallFunc::create(this, callfunc_selector(StartLayer::setPortraitStatusDelay))));


}

void StartLayer::setLandscapeStatus()
{

	this->runAction(Sequence::createWithTwoActions(
		DelayTime::create(0.1f),
		CallFunc::create(this, callfunc_selector(StartLayer::setLandscapeStatusDelay))));


}

void StartLayer::setLandscapeStatusDelay()
{
	setFrameAndDesignSizeWhenLandscape();
	resetPosition();
}

void StartLayer::setPortraitStatusDelay()
{
	setFrameAndDesignSizeWhenPortrait();
	resetPosition();
}


void StartLayer::resetPosition()
{
	if (m_logo)
	{
		m_logo->setPosition(CLDWinCenter());
	}
	if (m_cardNode)
	{
		m_cardNode->setPosition(CLDWinPos(0.5, 0.6));
	}
	if (m_light)
	{
		m_light->setPosition(CLDWinPos(0.5, 0.4));
	}
	if (m_appName)
	{
		m_appName->setPosition(CLDWinPos(0.5, 0.4));
	}
}
