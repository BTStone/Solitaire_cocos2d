#ifndef __ST_SOLITAIRE_OPTION_BOARD_MANAGER_H__
#define __ST_SOLITAIRE_OPTION_BOARD_MANAGER_H__


#include "OptionBaseBoard.h"

class OptionBoardManager
{
public:
	static OptionBoardManager* share();
	void pushBuck(OptionBaseBoard *board);
	void pop();
	void refreshBoards();
	OptionBaseBoard* getTheLastBoard();

	int size();

	void fixScreen(OptionBaseBoard *board);
	//float getOffsetY(OptionBaseBoard *board);
protected:
	static OptionBoardManager *m_shared;
	vector<OptionBaseBoard *> m_uiBoards;
};


#endif
