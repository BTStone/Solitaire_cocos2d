#ifndef __ST_SERVER_MANAGER_H__
#define __ST_SERVER_MANAGER_H__

#include "STBase.h"
#include "Card.h"

class ServerManager
{
public:
	static ServerManager *share();
	Card parseToCard(const char *cardStr);                   //��������������ֽ���ַ�����Ϣת����Card����,��3-8ת����÷��8
	vector<Card> getCardsOfPlaceQue(const char *serverStr, int queIdx);           //��ȡ������ĳһ���е���Ϣ,queIdx��0��ʼ
	vector<Card> parseQueInfo(const char *queStr);
	string getQueInfo(const char *serverStr, int queIdx);
	bool containSpace(const char *str);                      //�����ո�
	void stTest_ss();
protected:
	static ServerManager *m_shared;
};

#endif
