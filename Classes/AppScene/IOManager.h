/*�洢����ȡ����*/
#ifndef __ST_IO_MANAGER_H__
#define __ST_IO_MANAGER_H__

#include "STBase.h"
#include "Card.h"

class IOManager
{
public:
	static IOManager* share();
	void write(string contentStr, string fileName , string typeStr = "a");      //�洢����
	vector<Card> readCardByKey(const char *fileName, const char *key);
	string convertCardToString(Card pCard);
protected:
	static IOManager *m_shared;
};


#endif