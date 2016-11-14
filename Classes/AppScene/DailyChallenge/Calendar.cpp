#include "Calendar.h"
#include "DailyChallengeManager.h"
#include "DailyChallengeBoard.h"
#include"cocostudio/CocoStudio.h"
#include "CSBHelper.h"
#include "ClientCtrl.h"

int Calendar::m_curYear = 2016;
int Calendar::m_curMonth = 4;

Calendar::Calendar()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Calendar::msgSelectCalendarDayCallBack), ST_SELECT_CALENDAR_DAY, NULL);

}
Calendar::~Calendar()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Calendar* Calendar::create(int year, int mon, int day)
{
	Calendar *re = new Calendar();
	if (re && re->init(year, mon,day))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	return NULL;
}


int Calendar::getCurYear()
{
	return m_curYear;
}

int Calendar::getCurMonth()
{
	return m_curMonth;
}

bool Calendar::init(int year, int mon, int day)
{
	if (!Node::init())
	{
		return false;
	}
	m_curYear = year;
	m_curMonth = mon;
	m_originDay = day;
	m_selectDay = m_originDay;
	m_calendarSize = Size(500, 440);
	m_maxLookMonCount = 1;
	m_preMonCount = m_maxLookMonCount;
	m_rootCSBNode = NULL;
	initRootCSBNode();
	handleButton();
	initPositionList();
	refresh();
	updateWeekLabel();
	return true;
}

void Calendar::initRootCSBNode()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	string csbName = "cocostudio/port/Calendar_port.csb";
	if (!isPort)
	{
		csbName = "cocostudio/land/Calendar_land.csb";
	}
	m_rootCSBNode = CSLoader::createNode(csbName);
	ST_RETURN_IF(!m_rootCSBNode);
	m_rootCSBNode->setPosition(Vec2::ZERO);
	this->addChild(m_rootCSBNode);

	///隐藏背景
	Node *bg = ui::Helper::seekNodeByName(m_rootCSBNode, "Bg");
	if (bg)
	{
		bg->setVisible(false);
	}
}

void Calendar::handleButton()
{
	CSBHelper::registerButton(m_rootCSBNode, "BtnPre", CC_CALLBACK_2(Calendar::btnLeftCallBack, this));
	CSBHelper::registerButton(m_rootCSBNode, "BtnNext", CC_CALLBACK_2(Calendar::btnRightCallBack, this));
}

void Calendar::addSuccessEffect()
{
	int day = DailyChallengeManage::share()->getChallengeDay();
	int sz = m_calendarItems.size();
	ST_RETURN_IF(day < 0 || day > sz);

	Node *bg = ui::Helper::seekNodeByName(m_rootCSBNode, "Bg");
	ST_RETURN_IF(!bg);
	Size bgSize = bg->getContentSize();
    m_calendarItems[day - 1]->hideCrown();
	Node *effectNode = Node::create();
	ST_RETURN_IF(!effectNode);
	effectNode->setScale(1.5f);
	effectNode->setPosition(Vec2(bgSize.width*0.5, bgSize.height*0.5));
	this->addChild(effectNode, 20);

	//光
	Sprite *light = Sprite::create("Pic/Img_RingLight.png");
	ST_RETURN_IF(!light);
	effectNode->addChild(light);
	RepeatForever *rep1 = RepeatForever::create(RotateBy::create(2.0f, 360));
	ST_RETURN_IF(!rep1);
	light->runAction(rep1);

	//皇冠
	string crownName = m_calendarItems[day - 1]->getCrownPicName();
	Sprite *crown = Sprite::create(crownName);
	ST_RETURN_IF(!crown);
	crown->setAnchorPoint(Vec2(0.5, 0.45));
	effectNode->addChild(crown);

	//静止展示
	DelayTime *delay = DelayTime::create(1.0f);
	ST_RETURN_IF(!delay);

	//缩小并移动
	float dt = 0.5f;
	ScaleTo *caleTo = ScaleTo::create(dt, 0.5f);
	ST_RETURN_IF(!caleTo);
	Vec2 endPos = m_calendarItems[day - 1]->getPosition();
	MoveTo *mvTo = MoveTo::create(dt, endPos);
	ST_RETURN_IF(!mvTo);
	Spawn *spw = Spawn::create(caleTo, mvTo, NULL);
	ST_RETURN_IF(!spw);

	//结束回调
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(Calendar::successEffectEndCallFunc));
	ST_RETURN_IF(!callFunc);

	Sequence *seq = Sequence::create(delay, spw, callFunc, NULL);
	ST_RETURN_IF(!seq);
	effectNode->runAction(seq);

}

void Calendar::successEffectEndCallFunc(Node *node)
{
	node->removeFromParent();
	int day = DailyChallengeManage::share()->getChallengeDay();
	int sz = m_calendarItems.size();
	ST_RETURN_IF(day < 0 || day > sz);
	m_calendarItems[day - 1]->refreshStatus();
}

void Calendar::refresh()
{
	ST_RETURN_IF(!m_rootCSBNode);
	clearCalendar();
	refreshSelectDay();
	int wDay = getWeekDayOfDate(m_curYear, m_curMonth, 1);                 //获取本月1号是周几
	int totalDay = getTotalDayOfMonth(m_curYear, m_curMonth);             //获取本月总共多少天
	float itemScale = 0.4f;
	Node *node = ui::Helper::seekNodeByName(m_rootCSBNode, "Item1");
	if (node)
	{
		itemScale = node->getScale();
	}
	for (int i = 0; i < totalDay; i++)
	{
		int day = i + 1;
		
		int idx = wDay + i;
		CalendarItem *item = CalendarItem::create(day);
		ST_RETURN_IF(!item); 
		Size tSiz = item->getSize();
		item->setScale(itemScale);
		item->setPosition(m_positionList[idx]);
		item->setTag(day);
		m_rootCSBNode->addChild(item);
		m_calendarItems.push_back(item);

		if (m_selectDay == day)
		{
			item->select();
		}
	}
	refreshCurMonthLabel();
	refreshArrowMenu();
}

void Calendar::updateWeekLabel()
{
	const char *keyStr[] = { "Sun.", "Mon.", "Tues.", "Wed.", "Thur.", "Fri.", "Sat." };
	const char *csbNames[] = { "TextSun", "TextMon", "TextTue", "TextWed", "TextThu", "TextFri", "TextSat" };
	int num = 7;

	for (int i = 0; i < num; i++)
	{
		string str = STLang::shared()->getValue(keyStr[i]);
		CSBHelper::updateFntTextToLabel(m_rootCSBNode, csbNames[i], str);
	}
}

void Calendar::refreshArrowMenu()
{
	Node *leftArrow = ui::Helper::seekNodeByName(m_rootCSBNode, "BtnPre");
	ST_RETURN_IF(!leftArrow);
	leftArrow->setVisible(true);

	Node *rightArrow = ui::Helper::seekNodeByName(m_rootCSBNode, "BtnNext");
	ST_RETURN_IF(!rightArrow);
	rightArrow->setVisible(true);

	time_t curTm = STTimeManager::share()->getCurrentTime();
	struct tm *nowTm = localtime(&curTm);

	int nowMon = nowTm->tm_mon + 1;
	int nowYear = nowTm->tm_year + 1900;

	bool isBeforeNowMon =  (m_curYear < nowYear || (m_curYear == nowYear && m_curMonth < nowMon));
	if (!isBeforeNowMon)
	{
		rightArrow->setVisible(false);
	}
	else 
	{
		leftArrow->setVisible(false);
	}
}

void Calendar::refreshSelectDay()
{
	int day = getTheLastNotSuccessDayOfMon(m_curYear, m_curMonth);
	m_selectDay = day;

}

int Calendar::getTheLastNotSuccessDayOfMon(int year, int mon)
{
	int totalDay = getTotalDayOfMonth(year, mon);


	int beginDay = totalDay;

	time_t nowTime = STTimeManager::share()->getCurrentTime();
	struct tm *today = localtime(&nowTime);
	m_selectDay = m_originDay;

	int nowMon = today->tm_mon + 1;
	int nowYear = today->tm_year + 1900;
	int nowDay = today->tm_mday;

	if (year == nowYear && mon == nowMon)
	{
		beginDay = nowDay;
	}

	for (int i = beginDay; i > 0; i--)
	{
		bool isSuc = DailyChallengeManage::share()->isChallengeSuccess(year, mon, i);
		if (!isSuc)
		{
			return i;
		}
	}
	return beginDay;
}



void Calendar::refreshCurMonthLabel()
{
	const char *months[] = { "January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December" };
	string monStr = STLang::shared()->getValue(months[m_curMonth-1]);
	monStr = STLabelBMFont::convertFirstLetterToCapital(monStr);
	string dayStr = String::createWithFormat("%d", m_selectDay)->getCString();
	string str = monStr + " " + dayStr;
	CSBHelper::updateFntTextToLabel(m_rootCSBNode, "TextDate", str, -1.0f, TextHAlignment::LEFT, Color3B(1, 1, 1), -1.0f, Color4B(0, 0, 0, 100));
}

int Calendar::getTotalDayOfMonth(int year, int mon)
{
	int d;
	int day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (2 == mon)
	{
		d = (((0 == year % 4) && (0 != year % 100) || (0 == year % 400)) ? 29 : 28);
	}
	else
	{
		d = day[mon - 1];

	}
	return d;
}

int Calendar::getWeekDayOfDate(int year, int month, int day)
{
	struct tm date = { 0 };
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = 1;
	date.tm_hour = 10;
	time_t dateTime = mktime(&date);
	struct tm *dt = localtime(&dateTime);
	int re = dt->tm_wday;
	return re;
}

void Calendar::clearCalendar()
{
	int sz = m_calendarItems.size();
	for (int i = 0; i < sz; i++)
	{
		ST_SAFE_REMOVE_FROM_PARENT(m_calendarItems[i]);
	}
	m_calendarItems.clear();
}


void Calendar::initPositionList()
{
	int num = 42;
	for (int i = 0; i < num; i++)
	{
		string itemName = String::createWithFormat("Item%d", i + 1)->getCString();
		Node *item = ui::Helper::seekNodeByName(m_rootCSBNode, itemName);
		if (item)
		{
			item->setVisible(false);
			Vec2 pos = item->getPosition();
			m_positionList.push_back(pos);
		}
	}
}


void Calendar::btnRightCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		if (1)//(m_preMonCount < m_maxLookMonCount)
		{
			m_preMonCount++;
			m_curMonth++;
			if (m_curMonth > 12)
			{
				m_curMonth = 1;
				m_curYear++;
			}
			refresh();
		}
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void Calendar::btnLeftCallBack(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		if (1)//(m_preMonCount > 0)
		{
			m_preMonCount--;
			m_curMonth--;
			if (m_curMonth < 1)
			{
				m_curMonth = 12;
				m_curYear--;
			}
			refresh();
		}
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void Calendar::msgSelectCalendarDayCallBack(Ref *ref)
{
	m_selectDay = (int)(ref);
	int sz = m_calendarItems.size();
	for (int i = 0; i < sz; i++)
	{
		m_calendarItems[i]->unselect();
	}
	refreshCurMonthLabel();
}
