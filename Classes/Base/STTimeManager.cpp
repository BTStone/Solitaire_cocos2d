#include "STTimeManager.h"
#include "ClientCtrl.h"

STTimeManager * STTimeManager::m_shared = NULL;
STTimeManager * STTimeManager::share()
{
	if (!m_shared)
	{
		m_shared = new STTimeManager();
		m_shared->init();
	}

	return m_shared;
}

void STTimeManager::init()
{
	m_currentTime = time(NULL);
}

time_t STTimeManager::getLocalTime()
{
	time_t localTime = time(NULL);
	return localTime;
}

long STTimeManager::getDiffTime(struct tm preTm, struct tm lastTm)
{
	time_t preTime = mktime(&preTm);
	time_t lastTime = mktime(&lastTm);

	return (long)(lastTime - preTime);
}

void STTimeManager::setCurrentTime(time_t tt)
{
	m_currentTime = tt;
}

time_t STTimeManager::setCurrentTime(const char *dataStr)
{
	int len = strlen(dataStr);
	char sepreateCh = '-'; //·Ö¸ô·û
	int year = 2016;
	int mon = 1;
	int day = 1;
	int sepreateChCount = 0;
	string str = "";
	for (int i = 0; i < len; i++)
	{
		char ch = dataStr[i];
		if (ch == sepreateCh)
		{
			sepreateChCount++;
			if (sepreateChCount == 1)
			{
				year = atoi(str.c_str());
			}
			else if (sepreateChCount == 2)
			{
				str = removeFirstZero(str);
				mon = atoi(str.c_str());
			}
			str = "";
		}
		else
		{
			str += ch;
		}
	}
	str = removeFirstZero(str);
	day = atoi(str.c_str());
	struct tm ttt = { 0 };
	ttt.tm_year = year - 1900;
	ttt.tm_mon = mon - 1;
	ttt.tm_mday = day;
	ttt.tm_hour = 10;
	m_currentTime = mktime(&ttt);
	return m_currentTime;
}

string STTimeManager::removeFirstZero(string str)
{
	int len = str.size();
	string re = "";
	bool isBegin = false;
	for (int i = 0; i < len; i++)
	{
		char ch = str[i];
		if (isBegin || ch != '0')
		{
			isBegin = true;
			re += ch;
		}
	}
	return re;
}