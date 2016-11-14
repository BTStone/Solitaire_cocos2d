#ifndef __ST_NEW_GAME_MANAGER_H__
#define __ST_NEW_GAME_MANAGER_H__

#include "STBase.h"

class NewGameManager
{
public:
	static NewGameManager* share();
	string getNewGameInfo();
protected:
	static NewGameManager *m_shared;
};

#endif
