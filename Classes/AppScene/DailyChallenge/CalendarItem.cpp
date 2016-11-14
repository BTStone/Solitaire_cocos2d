#include "CalendarItem.h"
#include "Calendar.h"
#include "DailyChallengeManager.h"
#include "ClientCtrl.h"

CalendarItem* CalendarItem::create(int day)
{
	CalendarItem *re = new CalendarItem();
	if (re && re->init(day))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	return NULL;
}

bool CalendarItem::init(int day)
{
	if (!Node::init())
	{
		return false;
	}

	m_day = day;

	initContent();
	initCrown();

	refreshStatus();
	return true;
}

void CalendarItem::refreshStatus()
{
	int curYear = Calendar::getCurYear();                    //当前日历的年份
	int curMon = Calendar::getCurMonth();                    //当前日历的月份
	bool isSuc = DailyChallengeManage::share()->isChallengeSuccess(curYear, curMon, m_day);
	if (isSuc)
	{
		m_crownSprite->setVisible(true);
		m_dayLabel->setVisible(false);
	}
	else
	{
		m_crownSprite->setVisible(false);
	}
	if (isAfterToday())
	{
		afterToday();
	}
	addTodayEffect();
}

void CalendarItem::hideCrown()
{
	m_crownSprite->setVisible(false);
}

void CalendarItem::showDayLabel()
{
	m_dayLabel->setVisible(true);
}
void CalendarItem::hideDayLabel()
{
	m_dayLabel->setVisible(false);
}

float CalendarItem::getCrownScale()
{
	return m_crownSprite->getScale();
}

bool CalendarItem::isToday()
{
	int year = Calendar::getCurYear();
	int mon = Calendar::getCurMonth();
	int day = m_day;
	time_t todayTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&todayTime);
	int curYear = today->tm_year + 1900;
	int curMon = today->tm_mon + 1;
	int curDay = today->tm_mday;

	return (year == curYear) && (mon == curMon) && (day == curDay);
}

bool CalendarItem::isTodaySuccess()
{
	time_t todayTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&todayTime);
	int curYear = today->tm_year + 1900;
	int curMon = today->tm_mon + 1;
	int curDay = today->tm_mday;

	bool re = DailyChallengeManage::share()->isChallengeSuccess(curYear, curMon, curDay);
	return re;
}

bool CalendarItem::isYesterday()
{
	int year = Calendar::getCurYear();
	int mon = Calendar::getCurMonth();
	int day = m_day;
	time_t todayTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&todayTime);
	int curYear = today->tm_year + 1900;
	int curMon = today->tm_mon + 1;
	int curDay = today->tm_mday;

	int yesdYear = curYear;
	int yesdMon = curMon;
	int yesdDay = curDay;
	yesdDay--;
	if (yesdDay <= 0)
	{
		yesdMon--;
		if (yesdMon <= 0)
		{
			yesdMon = 12;
			yesdYear--;
		}
		int totalDay = Calendar::getTotalDayOfMonth(yesdYear, yesdMon);
		yesdDay = totalDay;
	}

	return (year == yesdYear) && (mon == yesdMon) && (day == yesdDay);
}

bool CalendarItem::isTheDayBeforeYesterday()
{
	int year = Calendar::getCurYear();
	int mon = Calendar::getCurMonth();
	int day = m_day;
	time_t todayTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&todayTime);
	int curYear = today->tm_year + 1900;
	int curMon = today->tm_mon + 1;
	int curDay = today->tm_mday;

	int yesdYear = curYear;
	int yesdMon = curMon;
	int yesdDay = curDay;

	yesdDay-=2;
	if (yesdDay <= 0)
	{
		yesdMon--;
		if (yesdMon <= 0)
		{
			yesdMon = 12;
			yesdYear--;
		}
		int totalDay = Calendar::getTotalDayOfMonth(yesdYear, yesdMon);
		yesdDay = totalDay;
	}

	return (year == yesdYear) && (mon == yesdMon) && (day == yesdDay);
}

bool CalendarItem::isAfterToday()
{
	int year = Calendar::getCurYear();
	int mon = Calendar::getCurMonth();
	int day = m_day;
	time_t todayTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&todayTime);
	int curYear = today->tm_year + 1900;
	int curMon = today->tm_mon + 1;
	int curDay = today->tm_mday;
	if (year > curYear)
	{
		return true;
	}
	else if (year == curYear && mon > curMon)
	{
		return true;
	}
	else if (year == curYear && mon == curMon && day > curDay)
	{
		return true;
	}
	return false;

}

void CalendarItem::initContent()
{
	///响应点击的按钮
	m_button = Button::create("Pic/UI-Daliy-bg.png", "Pic/UI-Daliy-bg.png");
	ST_RETURN_IF(!m_button);
	m_button->addTouchEventListener(CC_CALLBACK_2(CalendarItem::menuSelectCallBack, this));
	this->addChild(m_button, 0);
	
	
	m_todaySpriteBg = Sprite::create("Pic/UI-Daliy-bg2.png");
	ST_RETURN_IF(!m_todaySpriteBg);
	m_todaySpriteBg->setVisible(false);
	this->addChild(m_todaySpriteBg, 0);

	///m_selectSpriteBg  普通背景
	m_selectSpriteBg = Sprite::create("Pic/UI-Daliy-Done-bg.png");
	ST_RETURN_IF(!m_selectSpriteBg);
	m_selectSpriteBg->setVisible(false);
	this->addChild(m_selectSpriteBg, 0);

	///m_dayLabel        日期的label
	m_dayLabel = STLabelBMFont::createWithBMFont("EurostileBold.fnt", String::createWithFormat("%d", m_day)->getCString(), true);
	ST_RETURN_IF(!m_dayLabel);
	float labelScale = 0.4f;
	m_dayLabel->setScale(labelScale);
	m_dayLabel->setPosition(Vec2(-3 * labelScale, 0));
	m_dayLabel->setColor(Color3B::WHITE);
	
	this->addChild(m_dayLabel, 5);

}

void CalendarItem::initCrown()
{
	int curYear = Calendar::getCurYear();                    //当前日历的年份
	int curMon = Calendar::getCurMonth();                    //当前日历的月份
	int bestDay = DailyChallengeManage::share()->getBestDayOfMon(curYear, curMon);//
	string crownPic = "Pic/UI-Icon-Crown-lv2-x128.png";

	if (m_day == bestDay)
	{
		crownPic = "Pic/UI-Icon-Crown-lv3-x128.png";
	}
	m_crownPicName = crownPic;
	///m_crownSprite  皇冠
	m_crownSprite = Sprite::create(crownPic);
	m_crownSprite->setAnchorPoint(Vec2(0.5, 0.5));
	ST_RETURN_IF(!m_crownSprite);
	m_crownSprite->setScale(0.5f);
	this->addChild(m_crownSprite, 10);
}

Size CalendarItem::getSize()
{
	return m_selectSpriteBg->getContentSize();
}

void CalendarItem::menuSelectCallBack(Ref *pSender, Widget::TouchEventType type)
{/*
	Button *bt = (Button*)(pSender);
	int tag = bt->getTag();*/
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		select();
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void CalendarItem::select()
{
	NotificationCenter::getInstance()->postNotification(ST_SELECT_CALENDAR_DAY, (Ref*)m_day);
	m_selectSpriteBg->setVisible(true);

	bool isTody = isToday();
	bool isYesday = isYesterday();
	bool isBeforeYesday = isTheDayBeforeYesterday();
	if (isTody)
	{

	}
	else if (isYesday)
	{
		ClientCtrl::share()->reportEventParam("ClickYesterdayButton");
	}
	else if (isBeforeYesday)
	{
		ClientCtrl::share()->reportEventParam("ClickTheDayBeforeYesterdayButton");
	}
	else
	{
		ClientCtrl::share()->reportEventParam("ClickBeforeYesterdayBeforeButton");
	}
}

void CalendarItem::unselect()
{
	m_selectSpriteBg->setVisible(false);
}

void CalendarItem::afterToday()
{
	m_button->setVisible(false);
	m_crownSprite->setVisible(false);
	m_selectSpriteBg->setVisible(false);
	m_dayLabel->setColor(Color3B(4, 57, 95));
}

void CalendarItem::addTodayEffect()
{
	bool isTody = isToday();
	ST_RETURN_IF(!isTody);
	bool isTodaySuc = isTodaySuccess();
	ST_RETURN_IF(isTodaySuc);
	m_todaySpriteBg->setVisible(true);

	Sprite *sp2 = Sprite::create("Pic/UI-Daliy-bg2.png");
	ST_RETURN_IF(!sp2);
	sp2->setOpacity(50);
	this->addChild(sp2, -1);

	ScaleTo *scaleTo = ScaleTo::create(1.0f, 2.0f);
	ST_RETURN_IF(!scaleTo);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(CalendarItem::todayEffctScaleEndCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(scaleTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	RepeatForever *ret = RepeatForever::create(seq);
	ST_RETURN_IF(!ret);
	sp2->runAction(ret);
}

void CalendarItem::todayEffctScaleEndCallFunc(Node *node)
{
	node->setScale(1.0f);
}
