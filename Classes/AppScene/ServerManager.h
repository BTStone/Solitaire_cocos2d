#ifndef __ST_SERVER_MANAGER_H__
#define __ST_SERVER_MANAGER_H__

#include "STBase.h"
#include "Card.h"

class ServerManager
{
public:
	static ServerManager *share();
	Card parseToCard(const char *cardStr);                   //将服务器传来的纸牌字符串信息转换成Card类型,如3-8转换成梅花8
	vector<Card> getCardsOfPlaceQue(const char *serverStr, int queIdx);           //获取置牌区某一队列的信息,queIdx从0开始
	vector<Card> parseQueInfo(const char *queStr);
	string getQueInfo(const char *serverStr, int queIdx);
	bool containSpace(const char *str);                      //包含空格
	void stTest_ss();
protected:
	static ServerManager *m_shared;
};

#endif
