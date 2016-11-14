#include "NotificationManage.h"
#include "ClientCtrl.h"


NotificationManage* NotificationManage::m_shared = NULL;
NotificationManage* NotificationManage::share()
{
	if (!m_shared)
	{
		m_shared = new NotificationManage();
	}
	return m_shared;
}

bool NotificationManage::tryToPostDailyChallenge()
{
	time_t localTm = STTimeManager::share()->getLocalTime();
	struct tm *nowTm = localtime(&localTm);
	int curYear = nowTm->tm_year + 1900;
	int curMon = nowTm->tm_mon + 1;
	int curDay = nowTm->tm_mday;
	char buf[128];
	sprintf(buf, "Is_notification_daily_challenge_%d_%d_%d", curYear, curMon, curDay);
	bool hasPost = UserDefault::getInstance()->getBoolForKey(buf, false);
	ST_RETURN_FALSE_IF(hasPost);

	UserDefault::getInstance()->setBoolForKey(buf, true);

	int hour = 11;       //第二天的几点开始

	int curHour = nowTm->tm_hour;
	int curMinute = nowTm->tm_min;
	int curSec = nowTm->tm_sec;
	int offHour = hour + 24 - curHour - 1;
	int offMin = 60 - curMinute - 1;
	int offSec = 60 - curSec;

	int totalOffSec = offHour * 60 * 60 + offMin * 60 + offSec;

	int delayMs = totalOffSec * 1000;         //毫秒
	string titleStr = "Solitaire";
	string contStr = "New Daily Challenge is ready.";
	contStr = STLang::shared()->getValue(contStr.c_str());
	ClientCtrl::share()->setNotification(titleStr, contStr, delayMs, Notification_Daily_Challenge_Id);
	ClientCtrl::share()->reportEventParam("SystemNotify", "NotifyDailyChallenge");
	return true;
}

bool NotificationManage::tryToPostThreeDaysLeave()
{
	int delayDay = 3;     //3天
	int delayMs = delayDay * 24 * 60 * 60 * 1000;         //毫秒
	string titleStr = "Solitaire";
	string contStr = "Free and no ads. Enjoy it please!!";
	contStr = STLang::shared()->getValue(contStr.c_str());
	ClientCtrl::share()->setNotification(titleStr, contStr, delayMs, Notification_Three_Days_Leave);
	ClientCtrl::share()->reportEventParam("SystemNotify", "NotifyThreeDaysLeave");

	return true;
}

bool NotificationManage::tryToPostNoWin()
{
	int delayHour = 1;     //3天
	int delayMs = delayHour * 60 * 60 * 1000;         //毫秒
	//delayMs = 10000;
	string titleStr = "Solitaire";
	string contStr = "You are running an unfinished game.";
	contStr = STLang::shared()->getValue(contStr.c_str());
	//ClientCtrl::share()->cancelNotification(Notification_No_Win);
	ClientCtrl::share()->setNotification(titleStr, contStr, delayMs, Notification_No_Win);
	ClientCtrl::share()->reportEventParam("SystemNotify","NotifyNoFinishGame");
	return true;
}
