#ifndef __ST_NOTIFICATION_MANAGE_H__
#define __ST_NOTIFICATION_MANAGE_H__

#include "STBase.h"

class NotificationManage
{
public:
	static NotificationManage * share();

	bool tryToPostDailyChallenge();          //��������ÿ����ս
	bool tryToPostThreeDaysLeave();          //������������δ����
	bool tryToPostNoWin();                   //��������δ����ƾ�
protected:
	static NotificationManage *m_shared;
};


#endif
