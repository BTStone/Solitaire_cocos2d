#ifndef __ST_NOTIFICATION_MANAGE_H__
#define __ST_NOTIFICATION_MANAGE_H__

#include "STBase.h"

class NotificationManage
{
public:
	static NotificationManage * share();

	bool tryToPostDailyChallenge();          //尝试推送每日挑战
	bool tryToPostThreeDaysLeave();          //尝试推送三天未在线
	bool tryToPostNoWin();                   //尝试推送未完成牌局
protected:
	static NotificationManage *m_shared;
};


#endif
