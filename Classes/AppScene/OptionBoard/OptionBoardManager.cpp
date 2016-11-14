#include "OptionBoardManager.h"
#include "ClientCtrl.h"


OptionBoardManager* OptionBoardManager::m_shared = NULL;

OptionBoardManager* OptionBoardManager::share()
{
	if (!m_shared)
	{
		m_shared = new OptionBoardManager();
	}
	return m_shared;
}


void OptionBoardManager::pushBuck(OptionBaseBoard *board)
{
	m_uiBoards.push_back(board);
}

void OptionBoardManager::pop()
{
	m_uiBoards.pop_back();
}

void OptionBoardManager::refreshBoards()
{
	int sz = m_uiBoards.size();
	ST_RETURN_IF(sz <= 0);
	vector<OptionBoardType> tps;
	for (int i = 0; i < sz; i++)
	{
		OptionBoardType tp = m_uiBoards[i]->getOptionBoardType();
		tps.push_back(tp);
		m_uiBoards[i]->removeFromParent();
	}
	m_uiBoards.clear();

	for (int i = 0; i < sz; i++)
	{
		NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD_WITHOUT_EFFECT, (Ref*)(tps[i]));
	}

}


OptionBaseBoard* OptionBoardManager::getTheLastBoard()
{
	int sz = m_uiBoards.size();
	ST_RETURN_NULL_IF(sz <= 0);
	return m_uiBoards[sz - 1];
}

int OptionBoardManager::size()
{
	int sz = m_uiBoards.size();
	return sz;
}

void OptionBoardManager::fixScreen(OptionBaseBoard *board)
{
	ST_RETURN_IF(!board);
	//用cocostudio搭建时候用的尺寸
	Size resSize = Size(720, 1280);
	Size winSize = CLDWinSize();
	bool isPort = ClientCtrl::share()->isPortrait();
	if(!isPort)
	{
		resSize = Size(resSize.height, resSize.width);
	}
	float scaleWidth = winSize.width / resSize.width;
	float scaleHeight = winSize.height / resSize.height;
	float realScale = min(scaleWidth, scaleHeight);
	board->setScale(realScale);
}
/*
float OptionBoardManager::getOffsetY(OptionBaseBoard *board)
{
	Size bgSize = board->getBgSize() * board->getScale();
	Size winSize = CLDWinSize();

	float bottomHeight = (winSize.height - bgSize.height) / 2.0;
	if (bottomHeight < ST_BANNER_HEIGHT)
	{
		return (ST_BANNER_HEIGHT - bottomHeight);
	}
	return 0;
}
*/