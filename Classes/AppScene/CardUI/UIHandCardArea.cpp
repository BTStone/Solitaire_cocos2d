#include "UIHandCardArea.h"
#include "UndoManager.h"
#include "ScreenDirectionManager.h"
#include "ClientCtrl.h"

UIHandCardArea* UIHandCardArea::create(HandCardArea areaData)
{
	UIHandCardArea *re = new UIHandCardArea();
	if (re && re->init(areaData))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool UIHandCardArea::init(HandCardArea areaData)
{
	if (!Node::init())
	{
		return false;
	}
	m_areaData = areaData;

	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();

	initQueues();
	return true;
}

void UIHandCardArea::initQueues()
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	float closedPosX = winSize.width - (Place_Area_Gap_X + cardSize.width / 2.0);
	float openPosX = winSize.width - (Place_Area_Gap_X + cardSize.width * 1.5);
	m_areaSize = Size(Place_Area_Gap_X*2 + 2 * cardSize.width + 2 * cardSize.width*Hand_Open_Area_Gap_X,cardSize.height);

	CardQueue closeQue = m_areaData.getClosedQueue();
	CardQueue openQue = m_areaData.getOpenQueue();

	m_closedQueue = UIHandClosedCardQueue::create(closeQue);
	ST_RETURN_IF(!m_closedQueue);
	float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
	m_closedQueue->setPosition(Vec2(closedPosX, posY));
	this->addChild(m_closedQueue);

	m_openQueue = UIHandOpendCardQueue::create(openQue);
	ST_RETURN_IF(!m_openQueue);
	m_openQueue->setPosition(Vec2(openPosX, posY));
	this->addChild(m_openQueue);

	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		setLandscapeStatus();
	}
}

void UIHandCardArea::setHandType(bool isLeft)
{
	bool isPortrait = ClientCtrl::share()->isPortrait();
	if (isPortrait)     //只有竖屏时才改变内部结构
	{
		if (isLeft)
		{
			setLeftHandType();
		}
		else
		{
			setRightHandType();
		}
	}
}

void UIHandCardArea::setLeftHandType()
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	float closedPosX = winSize.width - (Place_Area_Gap_X + cardSize.width / 2.0);
	float openPosX = winSize.width - (Place_Area_Gap_X + cardSize.width * 1.5);
	m_openQueue->setPositionX(closedPosX);
	m_closedQueue->setPositionX(closedPosX - (m_areaSize.width - cardSize.width - 2*Place_Area_Gap_X));
}

void UIHandCardArea::setRightHandType()
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	float closedPosX = winSize.width - (Place_Area_Gap_X + cardSize.width / 2.0);
	float openPosX = winSize.width - (Place_Area_Gap_X + cardSize.width * 1.5);
	m_openQueue->setPositionX(openPosX);
	m_closedQueue->setPositionX(closedPosX);
}

void UIHandCardArea::setPortraitStatus()
{
	Size winSize = CLDWinSize();
	Size cardSize = CardSprite::getCardSize();
	float closedPosX = winSize.width - (Place_Area_Gap_X + cardSize.width / 2.0);
	float openPosX = winSize.width - (Place_Area_Gap_X + cardSize.width * 1.5);
	float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
	m_closedQueue->setPosition(Vec2(closedPosX, posY));
	m_openQueue->setPosition(Vec2(openPosX, posY));
}


void UIHandCardArea::setLandscapeStatus()
{
	Size winSize = CLDWinSize(); 
	Size cardSize = CardSprite::getCardSize();
	float openPosX = winSize.width - (Place_Area_Gap_X + cardSize.width);
	float closedPosX = openPosX - cardSize.width*Hand_Open_Area_Gap_X;
	float openPosY = winSize.height * 0.6;
	float closePosY = openPosY - cardSize.height*1.2;
	m_closedQueue->setPosition(Vec2(closedPosX, closePosY));
	m_openQueue->setPosition(Vec2(openPosX, openPosY));
}

Vec2 UIHandCardArea::getClosedAreaPosition()
{
	return m_closedQueue->getWorldPosOfCardBg();
}
Vec2 UIHandCardArea::getOpenAreaLastCardPosition()
{
	return m_openQueue->getWorldPosOfCardBg();
}

TransportCardQueue* UIHandCardArea::search(Vec2 worldPos)
{
	TransportCardQueue *re = NULL;
	if ((re = m_closedQueue->search(worldPos)) || (re = m_openQueue->search(worldPos)))
	{
		return re;
	}
	return NULL;
}

bool UIHandCardArea::inColosedArea(Vec2 worldPos)
{
	return m_closedQueue->containThePos(worldPos);
}

Vec2 UIHandCardArea::getNextPositionOfClosedQue()
{
	return m_closedQueue->getWorldPosOfCardBg();
}

Vec2 UIHandCardArea::getNextPositionOfOpenQue()
{
	return m_openQueue->getWorldPosOfCardBg();
}

void UIHandCardArea::addCardsAtClosedArea(CardQueue queData)
{
	queData.reverse();
	queData.closeAllCard();
	m_closedQueue->addCards(queData);
}

void UIHandCardArea::addCardsAtOpenArea(CardQueue queData)
{
	m_openQueue->addCards(queData);
}

void UIHandCardArea::flipCard()
{
	const char *strIsDraw3 = "st_is_draw_3";
	bool isDraw3 = UserDefault::getInstance()->getBoolForKey(strIsDraw3, false);
	int flipNum = 1;
	if (isDraw3)
	{
		flipNum = 3;
	}
	vector<Card> cards = m_closedQueue->removeLastCardByNum(flipNum);
	int sz = cards.size();
	if (sz <= 0)
	{
		shuffleCard();
		return;
	}
	for (int i = 0; i < sz; i++)
	{
		cards[i].m_isOpen = true;
	}
	TransportCardQueue *que = TransportCardQueue::create(cards, HAND_CLOSE_AREA);
	ST_RETURN_IF(!que);
	que->setLandscapeStatus();
	this->addChild(que, 100);
	Vec2 resWorldPos = m_closedQueue->getWorldPosOfCardBg();
	Vec2 destWorldPos = m_openQueue->getWorldPosOfCardBg();
	Vec2 resPos = convertToNodeSpace(resWorldPos);
	Vec2 destPos = convertToNodeSpace(destWorldPos);
	que->setPosition(resPos);
	float dt = Move_And_Untouchable_Time;
	MoveTo *mvTo = MoveTo::create(dt, destPos);
	ST_RETURN_IF(!mvTo);
	CallFuncN *callFunc = CallFuncN::create(this, callfuncN_selector(UIHandCardArea::transportArrivedCallFunc));
	ST_RETURN_IF(!callFunc);
	Sequence *seq = Sequence::create(mvTo, callFunc, NULL);
	ST_RETURN_IF(!seq);
	que->runAction(seq);
	m_openQueue->addCardEffectWithNum(sz);
	if (flipNum > 1)
	{
		ClientCtrl::share()->playSound(SOUND_FLIP_CARD_3);
	}
	else
	{
		ClientCtrl::share()->playSound(SOUND_FLIP_CARD_1);
	}
}

void UIHandCardArea::shuffleCard()
{
	CardQueue queData = m_openQueue->getQueData();
	int sz = queData.size();
	ST_RETURN_IF(sz <= 0);
	queData.closeAllCard();
	queData.reverse();
	m_openQueue->removeAllCards();
	m_closedQueue->addCards(queData);

	//记录回退信息
	UndoInfo info; 
	info.m_resArea = HAND_OPEN_AREA;
	info.m_resQueIdx = 0;
	info.m_desArea = HAND_CLOSE_AREA;
	info.m_desQueIdx = 0;
	info.m_number = sz;
	info.m_scoreOff = 0;
	info.m_opened = false;
	UndoManager::share()->pushBack(info);

	NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)(true));
}

void UIHandCardArea::openAreaRemoveCardEffectWithNum(int remNum)
{
	m_openQueue->removeCardEffectWithNum(remNum);
}


void UIHandCardArea::openAreaAddCardEffectWithNum(int remNum)
{
	m_openQueue->addCardEffectWithNum(remNum);
}

void UIHandCardArea::transportArrivedCallFunc(Node *node)
{
	TransportCardQueue *que = dynamic_cast<TransportCardQueue*>(node);
	ST_RETURN_IF(!que);
	CardQueue queData = que->getQueData();
	m_openQueue->addCards(queData);

	//记录回退信息
	UndoInfo info;
	info.m_resArea = HAND_CLOSE_AREA;
	info.m_resQueIdx = 0;
	info.m_desArea = HAND_OPEN_AREA;
	info.m_desQueIdx = 0;
	info.m_number = que->size();
	info.m_scoreOff = 0;
	info.m_opened = false;
	UndoManager::share()->pushBack(info);


	que->removeFromParent();
	NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)(true));
}

bool UIHandCardArea::canAutoCollect()
{
	bool openOnlyOne = m_openQueue->isOnlyOneCard();
	bool isOpenEmp = m_openQueue->isEmpty();
	bool isClosedEmp = m_closedQueue->isEmpty();
	return (openOnlyOne || isOpenEmp) && isClosedEmp;
}

bool UIHandCardArea::isEmpty()
{
	return (m_closedQueue->isEmpty() && m_openQueue->isEmpty());
}


void UIHandCardArea::updateCard()
{
	m_closedQueue->updateCard();
	m_openQueue->updateCard();
}

void UIHandCardArea::syncData()
{
	//同步信息
	CardQueue que = m_closedQueue->getQueData();
	m_areaData.setClosedQueue(que);
	CardQueue openQue = m_openQueue->getQueData();
	m_areaData.setOpenQueue(openQue);
}

string UIHandCardArea::bakAllCards()
{
	syncData();
	return m_areaData.bakAllCards();
}

void UIHandCardArea::addClosedAreaTip()
{
	m_closedQueue->addTip();
}

void UIHandCardArea::addOpenAreaTip()
{
	int sz = m_openQueue->size();
	ST_RETURN_IF(sz <= 0);
	m_openQueue->addTip();
}

int UIHandCardArea::getClosedAreaSize()
{
	return m_closedQueue->size();
}

int UIHandCardArea::getOpenAreaSize()
{
	return m_openQueue->size();
}

CardSprite* UIHandCardArea::getOpenAreaLastCard()
{
	return m_openQueue->getTheLastCard();
}

vector<Card> UIHandCardArea::removeTheLastNumCardsOfOpenQue(int num)
{
	vector<Card> re = m_openQueue->removeLastCardByNum(num);
	syncData();
	return re;
}


vector<Card> UIHandCardArea::removeTheLastNumCardsOfClosedQue(int num)
{
	return m_closedQueue->removeLastCardByNum(num);

}