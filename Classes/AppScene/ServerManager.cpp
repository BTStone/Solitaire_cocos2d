#include "ServerManager.h"

const char split_card_iner = '-';      //����ֽ����Ϣ�ڲ��ָ�������2-1
const char split_card = ' ';           //����ֽ����Ϣ�ڲ��ָ�������2-1
const char split_queue = '|';          //�������м�ķָ���

ServerManager* ServerManager::m_shared = NULL;

void ServerManager::stTest_ss()
{
	const char *s = "3-10 |1-1 2-7 |0-7  2-8     0-1       |1-12 1-6 0-2 0-10 |";
	vector<Card> cards = getCardsOfPlaceQue(s, 2);
	int len = cards.size();
	for (int i = 0; i < len; i++)
	{
		Card cd = cards[i];
	}
}

ServerManager* ServerManager::share()
{
	if (!m_shared)
	{
		m_shared = new ServerManager();
	}
	return m_shared;
}

vector<Card> ServerManager::getCardsOfPlaceQue(const char *serverStr, int queIdx)
{
	string queStr = getQueInfo(serverStr, queIdx);
	return parseQueInfo(queStr.c_str());
}

vector<Card> ServerManager::parseQueInfo(const char *queStr)
{
	vector<Card> re; 
	if (!queStr){ return re; }
	int len = strlen(queStr);
	string temp = "";
	for (int i = 0; i < len; i++)
	{
		if (queStr[i] == split_card && strlen(temp.c_str())>2)
		{
			Card cd = parseToCard(temp.c_str());
			temp = "";
			if (!cd.isInvalidCard())
			{
				re.push_back(cd);
			}
			
		}
		else if (queStr[i] != split_card)
		{
			temp += queStr[i];
		}
	}
	Card cd = parseToCard(temp.c_str());
	if (!cd.isInvalidCard())
	{
		re.push_back(cd);
	}
	return re;
}

bool ServerManager::containSpace(const char *str)
{
	ST_RETURN_FALSE_IF((!str) || strlen(str) == 0);
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ')
		{
			return true;
		}
	}
	return false;
}

string ServerManager::getQueInfo(const char *serverStr, int queIdx)
{
	string re = "";
	int queueSplitNum = 0;        //���м�ķָ�������
	if (!serverStr){ return re; }
	int len = strlen(serverStr);
	for (int i = 0; i < len; i++)
	{
		if (serverStr[i] == split_queue)
		{
			queueSplitNum++;
		}
		else if (queIdx == queueSplitNum)
		{
			re += serverStr[i];
		}
	}
	return re;
}



Card ServerManager::parseToCard(const char *cardStr)
{
	/*��������Ϣ��ʽ
	���� = 0;
	���� = 1;
	÷�� = 2;
	���� = 3;

	���ϵ��£����� "3-10 |1-1 2-7 |0-7 2-8 0-1 |1-12 1-6 0-2 0-10 "
	*/
	const char *expStr = "3-1";   //ʾ����ʽ
	Card re = INVALID_CARD;
	if ((!cardStr) || strlen(cardStr) < strlen(expStr)){ return re; }

	int len = strlen(cardStr);
	string temp = "";
	for (int i = 0; i < len; i++)
	{
		if (cardStr[i] >= '0' && cardStr[i] <= '9')
		{
			temp += cardStr[i];
		}
		else if (cardStr[i] == split_card_iner)
		{
			re.m_color = (CardColor)(atoi(temp.c_str()));
			temp = "";
		}
	}
	re.m_number = atoi(temp.c_str());
	re.m_isOpen = false;           //Ĭ�ϴ���δ��״̬

	return re;
}