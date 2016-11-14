#ifndef __ST_TIME_MANAGER_H__
#define __ST_TIME_MANAGER_H__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class STTimeManager
{
public:
	static STTimeManager * share();
	void init();
	time_t getLocalTime();
	long getDiffTime(struct tm preTm, struct tm lastTm);         //��ȡ����ʱ���ʱ���룩

	void setCurrentTime(time_t tt);
	time_t setCurrentTime(const char *dataStr);
	CC_SYNTHESIZE_READONLY(time_t, m_currentTime, CurrentTime);//����ʱ��
protected:
	std::string removeFirstZero(string str);
protected:
	static STTimeManager *m_shared;
};

#endif
