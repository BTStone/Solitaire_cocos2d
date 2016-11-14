#include "LanguageSelectBoard.h"
#include "ClientCtrl.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"

LanguageSelectBoard::LanguageSelectBoard()
{
	
}
LanguageSelectBoard::~LanguageSelectBoard()
{
	
}

LanguageSelectBoard* LanguageSelectBoard::create()
{
	LanguageSelectBoard *re = new LanguageSelectBoard();
	if (re && re->init())
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool LanguageSelectBoard::init()
{
	if (!OptionBaseBoard::init(kLanguageSelectBoard))
	{
		return false;
	}
	initRootCSBNode();
	handleButton();
	updateLanguage();
	updateSelectSign();
	return true;
}
void LanguageSelectBoard::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Language_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Language_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_rootCSBNode);
}

void LanguageSelectBoard::handleButton()
{
	//button
	CSBHelper::registerButton(m_rootCSBNode, "ButtonClose", CC_CALLBACK_2(LanguageSelectBoard::btnCloseCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect1", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect2", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect3", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect4", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect5", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect6", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect7", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect8", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect9", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect10", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnSelect11", CC_CALLBACK_2(LanguageSelectBoard::btnSelectCallBack, this));

}

void LanguageSelectBoard::updateLanguage()
{
	//title
	updateTitle();

	updateContent();
}

void LanguageSelectBoard::updateTitle()
{
	string titleStr = STLang::shared()->getValue("selectLanguage");
	titleStr = STLabelBMFont::convertAllLetterToCapital(titleStr);
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextTitle", titleStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
}

void LanguageSelectBoard::updateContent()
{
	const char *csbNames[] = { "Text1", "Text2", "Text3", "Text4", "Text5", "Text6", "Text7", "Text8", "Text9", "Text10", "Text11" };
	int menuNum = 11;
	for (int i = 0; i < menuNum; i++)
	{
		char buf[64];
		sprintf(buf, "language_%d", i + 1);
		//Draw label
		string doneLabelStr = STLang::shared()->getLanguageName(buf);
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], doneLabelStr, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
	}
}


void LanguageSelectBoard::updateSelectSign()
{
	int curIdx = STLang::shared()->getCurLangIdx();
	select(curIdx);
}



void LanguageSelectBoard::select(int langIdx)
{
	setSelectSignPos(langIdx);

	//存储选择的语言，并更新游戏
	STLang::shared()->selectLanguage(langIdx);

	//更新标题
	updateTitle();
}


void LanguageSelectBoard::closeEffect()
{
	OptionBaseBoard::closeEffect();
	Size winSize = CLDWinSize();
	float dt = 0.2f;
	Vec2 endPos = this->getPosition() + Vec2(winSize.width*1.0, 0);
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	EaseElasticOut *ease = EaseElasticOut::create(mvTo);
	ST_RETURN_IF(!ease);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(LanguageSelectBoard::removeSelfCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
	ClientCtrl::share()->playSound(SOUND_MENU_OUT);
}

void LanguageSelectBoard::setSelectSignPos(int langIdx)
{
	string name = String::createWithFormat("BtnSelect%d", langIdx + 1)->getCString();
	Node *selectBt = ui::Helper::seekNodeByName(m_rootCSBNode, name);
	ST_RETURN_IF(!selectBt);
	
	Node *signNode = ui::Helper::seekNodeByName(m_rootCSBNode, "ImgSelectedSign");
	ST_RETURN_IF(!signNode);

	signNode->setPosition(Vec2(signNode->getPositionX(), selectBt->getPositionY()));
}

int LanguageSelectBoard::getNumFromString(string str)
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

void LanguageSelectBoard::btnCloseCallBack(Ref *pSender, Widget::TouchEventType type)
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
void LanguageSelectBoard::btnSelectCallBack(Ref *pSender, Widget::TouchEventType type)
{
	const char *langList[] = { "ChineseSimplified", "English", "ChineseTraditional", "French",
		"German", "Korean", "Portuguese", "Russian", "Spanish", "Japanese", "Indonesian" };

	Button *bt = (Button*)(pSender);
	ST_RETURN_IF(!bt);
	string name = bt->getName();
	int idx = getNumFromString(name) - 1;
	
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		UserDefault::getInstance()->setBoolForKey(ST_HAS_ALREADY_SELECT_LANGUAGE, true);
		select(idx);
		ClientCtrl::share()->reportEventParam("SelectLanguage", langList[idx]);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}