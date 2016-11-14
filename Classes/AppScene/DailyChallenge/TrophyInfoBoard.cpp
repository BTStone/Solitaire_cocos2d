#include "TrophyInfoBoard.h"
#include "Calendar.h"
#include "DailyChallengeManager.h"

TrophyInfoBoard* TrophyInfoBoard::create(TrophyType tp, int year, int mon)
{
	TrophyInfoBoard *re = new TrophyInfoBoard();
	if (re && re->init(tp, year, mon))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	return NULL;
}

bool TrophyInfoBoard::init(TrophyType tp, int year, int mon)
{
	if (!Node::init())
	{
		return false;
	}
	m_type = tp;
	m_year = year;
	m_month = mon;

	STHelper::addDarkLayer(this);
	initBg();
	initTrophy();
	initSolvedState();
	initSuccessInfo();
	initSeperateMenu();
	return true;
}

void TrophyInfoBoard::initBg()
{
	Scale9Sprite *bg = Scale9Sprite::create("Pic/UI-Box-TipBox-Bg.png");
	ST_RETURN_IF(!bg);
	Size bgSize = Size(230, 110);
	bg->setContentSize(bgSize);
	this->addChild(bg, 0);
}

void TrophyInfoBoard::initTrophy()
{
	string picName = "";
	if (m_type == kCopperTrophy)
	{
		picName = "Pic/UI-Icon-lv1Medal-x96.png";
	}
	else if (m_type == kSilverTrophy)
	{
		picName = "Pic/UI-Icon-lv2Medal-x96.png";
	}
	else if (m_type == kGoldenTrophy)
	{
		picName = "Pic/UI-Icon-lv3Medal-x96.png";
	}
	Sprite *trophySp = Sprite::create(picName);
	ST_RETURN_IF(!trophySp);
	trophySp->setScale(0.5f);
	trophySp->setPosition(-70, 5);
	this->addChild(trophySp, 1);
}

void TrophyInfoBoard::initSolvedState()
{
	int monthNum = Calendar::getTotalDayOfMonth(m_year, m_month);
	int sucNum = DailyChallengeManage::share()->getSuccessNumOfMon(m_year, m_month);
	string solveStr = "unsolved";
	int num = 0;
	if (m_type == kCopperTrophy)
	{
		num = 10;
	}
	else if (m_type == kSilverTrophy)
	{
		num = 20;
	}
	else if (m_type == kGoldenTrophy)
	{
		num = monthNum;
	}
	if (sucNum >= num)
	{
		solveStr = "solved";
	}
	STLabelBMFont *solveLab = STLabelBMFont::createWithBMFont("EurostileBold.fnt", solveStr);
	ST_RETURN_IF(!solveLab);
	solveLab->setScale(0.4f); 
	solveLab->setColor(Color3B(0,180,0));
	solveLab->setPosition(30, 25);
	this->addChild(solveLab, 1);

	if (solveStr == "unsolved")
	{
		solveLab->setColor(Color3B::RED);
	}
}

void TrophyInfoBoard::initSuccessInfo()
{
	int monthNum = Calendar::getTotalDayOfMonth(m_year, m_month);
	int sucNum = DailyChallengeManage::share()->getSuccessNumOfMon(m_year, m_month);
	int num = 0;
	if (m_type == kCopperTrophy)
	{
		num = 10;
	}
	else if (m_type == kSilverTrophy)
	{
		num = 20;
	}
	else if (m_type == kGoldenTrophy)
	{
		num = monthNum;
	}
	char buf[64];
	sprintf(buf, "%d / %d", sucNum, num);
	STLabelBMFont *infoLab = STLabelBMFont::createWithBMFont("EurostileBold.fnt", buf);
	ST_RETURN_IF(!infoLab);
	infoLab->setScale(0.45f);
	infoLab->setColor(Color3B(50, 53, 57));
	infoLab->setPosition(30, -15);
	this->addChild(infoLab, 1);
}

void TrophyInfoBoard::initSeperateMenu()
{
	Node * spaceNode = Node::create();
	ST_RETURN_IF(!spaceNode);
	spaceNode->setContentSize(CCSizeMake(CLDWinSize().width * 3, CLDWinSize().height * 3));
	MenuItemSprite * menuSeparate = MenuItemSprite::create(
		spaceNode,
		spaceNode,
		NULL,
		this,
		menu_selector(TrophyInfoBoard::menuRemoveSelfCallBack));
	ST_RETURN_IF(!menuSeparate);

	Menu * pMenu = Menu::createWithItem(menuSeparate);
	ST_RETURN_IF(!pMenu);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 100);
}

void TrophyInfoBoard::menuRemoveSelfCallBack(Ref *ref)
{
	this->removeFromParent();
}