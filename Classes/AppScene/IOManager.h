/*´æ´¢¡¢¶ÁÈ¡²Ù×÷*/
#ifndef __ST_IO_MANAGER_H__
#define __ST_IO_MANAGER_H__

#include "STBase.h"
#include "Card.h"

class IOManager
{
public:
	static IOManager* share();
	void write(string contentStr, string fileName , string typeStr = "a");      //´æ´¢²Ù×÷
	vector<Card> readCardByKey(const char *fileName, const char *key);
	string convertCardToString(Card pCard);
protected:
	static IOManager *m_shared;
};


#endif