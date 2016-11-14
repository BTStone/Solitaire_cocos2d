#include "OptionBaseBoard.h"
#include "OptionBoardManager.h"
#include "ClientCtrl.h"
#include "CardSprite.h"

bool OptionBaseBoard::init(OptionBoardType tp)
{
	if (!Node::init())
	{
		return false;
	}
	m_optionBoardType = tp;
	m_rootCSBNode = NULL;
	STHelper::separateTouch(this);
	STHelper::addDarkLayer(this);
	return true;
}



void OptionBaseBoard::initRootCSBNode()
{
	//子类重写
}


void OptionBaseBoard::closeEffect()
{
	//从manager删除
	OptionBoardManager::share()->pop();
}
void OptionBaseBoard::removeSelfCallFunc()
{
	int boardNum = OptionBoardManager::share()->size();
	if (boardNum <= 0)
	{
		ClientCtrl::share()->tryToHideBanner();
	}
	this->removeFromParent();

	static int idx = 0;
	idx++;
	//NotificationCenter::getInstance()->postNotification(ST_SHOW_OPTION_BOARD, (Ref*)idx);
}

Size OptionBaseBoard::getBgSize()
{
	if (!m_rootCSBNode)
	{
		return Size::ZERO;
	}
	Node *bg = ui::Helper::seekNodeByName(m_rootCSBNode, "BoardBg");
	if (bg)
	{
		return bg->getContentSize();
	}
	return Size::ZERO;
}