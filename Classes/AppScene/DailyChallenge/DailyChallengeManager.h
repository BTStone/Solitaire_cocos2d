#ifndef __ST_DAILY_CHALLENGE_MANAGER_H__
#define __ST_DAILY_CHALLENGE_MANAGER_H__

#include "STBase.h"

class DailyChallengeManage
{
public:
	static DailyChallengeManage* share();
	void init();

	int convertDataToIndex(int year, int mon, int day);              //������ת�����ƾֿ���±�
	string getCardInfoByData(int year, int mon, int day);
	void success(int year, int mon, int day);
	bool isChallengeSuccess(int year, int mon, int day);
	int getBestDayOfMon(int year, int mon);
	int getSuccessNumOfMon(int year, int mon);
	void setChallengeData(int year, int mon, int day);

	struct tm getDailyChallengeDefaultData();                             //��ȡ�����û��ͨ��������

	void stTest_reset();
protected:
	void setChallengeYear(int year);
	void setChallengeMonth(int mon);
	void setChallengeDay(int day);
	CC_SYNTHESIZE(bool, m_justNowSuccess, JustNowSuccess);
	CC_SYNTHESIZE_READONLY(int, m_challengeYear, ChallengeYear);
	CC_SYNTHESIZE_READONLY(int, m_challengeMonth, ChallengeMonth);
	CC_SYNTHESIZE_READONLY(int, m_challengeDay, ChallengeDay);
protected:
	static DailyChallengeManage *m_shared;
};


#endif
