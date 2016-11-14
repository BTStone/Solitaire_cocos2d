#include "UIPlaceCardArea.h"
#include "ScreenDirectionManager.h"

UIPlaceCardArea* UIPlaceCardArea::create(PlaceCardArea areaData)
{
	UIPlaceCardArea *re = new UIPlaceCardArea();
	if (re && re->init(areaData))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool UIPlaceCardArea::init(PlaceCardArea areaData)
{
	if (!Node::init())
	{
		return false;
	}
	m_areaData = areaData;

	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	initQueues();
	return true;
}

void UIPlaceCardArea::initQueues()
{
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	m_emptyOffX = (winSize.width - 8 * Place_Area_Gap_X - Place_Area_Num * cardSize.width) / 2.0;
	//m_emptyOffX = 0;// (winSize.width - 8 * Place_Area_Gap_X - Place_Area_Num * cardSize.width) / 2.0;
	for (int i = 0; i < Place_Area_Num; i++)
	{
		PlaceCardQueue queData = m_areaData[i];
		m_queues[i] = UIPlaceCardQueue::create(queData);
		ST_RETURN_IF(!m_queues[i]);
		float beginX = m_emptyOffX + Place_Area_Gap_X + cardSize.width / 2.0f;
		float posX = beginX + i * (cardSize.width + Place_Area_Gap_X);
		float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5 - cardSize.height*1.2;
		m_queues[i]->setPosition(Vec2(posX, posY));
		this->addChild(m_queues[i]);
		m_queues[i]->tryToScale();
	}

	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		log("UIPlaceCardArea::initQueues,isLandscape");
		setLandscapeStatus();
	}
}



void UIPlaceCardArea::setPortraitStatus()
{
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	m_emptyOffX = (winSize.width - 8 * Place_Area_Gap_X - Place_Area_Num * cardSize.width) / 2.0;
	log("UIPlaceCardArea::setLandscapeStatus");
	for (int i = 0; i < Place_Area_Num; i++)
	{
		Size cardSize = CardSprite::getCardSize();
		Size winSize = CLDWinSize();
		float beginX = m_emptyOffX + Place_Area_Gap_X + cardSize.width / 2.0f;
		float posX = beginX + i * (cardSize.width + Place_Area_Gap_X);
		float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5 - cardSize.height*1.2;
		m_queues[i]->setPosition(Vec2(posX, posY));
	}
}

bool UIPlaceCardArea::isAllCardsOpen()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		bool isOp = m_queues[i]->isAllCardsOpen();
		if (!isOp)
		{
			return false;
		}
	}
	return true;
}

void UIPlaceCardArea::setLandscapeStatus()
{
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	m_emptyOffX = (winSize.width - 8 * Place_Area_Gap_X - Place_Area_Num * cardSize.width) / 2.0;
	log("UIPlaceCardArea::setLandscapeStatus");
	for (int i = 0; i < Place_Area_Num; i++)
	{
		Size cardSize = CardSprite::getCardSize();
		Size winSize = CLDWinSize();
		//float beginX = (winSize.width - (8*Place_Area_Gap_X + cardSize.width*Place_Area_Num))/2.0  + cardSize.width * 0.5;  ///只是修改这里会有问题，下面还有一个要修改
		float beginX = m_emptyOffX + cardSize.width * 0.5;  ///只是修改这里会有问题，下面还有一个要修改
		float posX = beginX + i * (cardSize.width + Place_Area_Gap_X);
		log("UIPlaceCardArea::setLandscapeStatus,posX:%f", posX);
		float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
		m_queues[i]->setPosition(Vec2(posX, posY));
	}
}

int UIPlaceCardArea::getQueSize(int queIdx)
{
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return 0;
	}
	return m_queues[queIdx]->size();
}

TransportCardQueue* UIPlaceCardArea::search(Vec2 worldPos)
{
	int queIndex = getQueIndexByPos(worldPos);
	ST_RETURN_NULL_IF(queIndex == -1);
	TransportCardQueue *re = m_queues[queIndex]->search(worldPos);
	if (re)
	{
		re->setResQueIndex(queIndex);
	}
	return re;
}

int UIPlaceCardArea::searchTheCard(Card card)
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		if (m_queues[i]->containTheCard(card))
		{
			return i;
		}
	}
	return -1;
}

int UIPlaceCardArea::getQueIndexByPos(Vec2 worldPos)
{
	Vec2 pos = convertToNodeSpace(worldPos);
	Size cardSize = CardSprite::getCardSize();
	float gap = Place_Area_Gap_X;
	if ((pos.x - m_emptyOffX) < 0){ return -1; }
	int index = (pos.x - m_emptyOffX)/ (gap + cardSize.width);
	if (index > (Place_Area_Num - 1)){ return -1; }
	return index;
	/*
	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		Size winSize = CLDWinSize();
		float beginX = (winSize.width - (8 * Place_Area_Gap_X + cardSize.width*Place_Area_Num)) / 2.0 + cardSize.width * 0.5;  ///只是修改这里会有问题，下面还有一个要修改
		//float beginX = Place_Area_Gap_X + cardSize.width * 3;
		float minX = beginX - cardSize.width*0.5;
		float maxX = beginX + (cardSize.width + Place_Area_Gap_X)*(Place_Area_Num - 1) + cardSize.width / 2.0;
		if (pos.x < minX || pos.x > maxX){ return -1; }
		float dis = pos.x - minX;
		index = dis / (gap + cardSize.width);
	}
	index = index < 0 ? 0 : index;
	index = index > (Place_Area_Num - 1) ? (Place_Area_Num - 1) : index;
	return index;*/
}


int UIPlaceCardArea::canPlaceTheCard(TransportCardQueue *que)
{
	CardArea resArea = que->getResArea();
	int resQueIdx = que->getResQueIndex();
	for (int i = 0; i < Place_Area_Num; i++)
	{
		bool isRes = ((resArea == PLACE_AREA) && (resQueIdx == i));
		if (canPlaceTheCard(que, i) && !isRes)
		{
			return i;
		}
	}
	return -1;
}


bool UIPlaceCardArea::canPlaceTheCard(TransportCardQueue *que, int queIdx)
{
	log("UIPlaceCardAreaccanplaceTheCard,queIdx:%d", queIdx);
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	int sz = que->size();
	ST_RETURN_FALSE_IF(sz <= 0);
	CardSprite *firstCard = que->getTheFirstCard();
	ST_RETURN_FALSE_IF(!firstCard);
	Card firstCardInfo = firstCard->getCardInfo();
	log("UIPlaceCardAreaccanplaceTheCardxxx,queIdx:%d", queIdx);
	return m_queues[queIdx]->canPlaceTheCard(firstCardInfo);
}


Vec2 UIPlaceCardArea::getNextPosition(int queIdx)
{
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return Vec2::ZERO;
	}
	return m_queues[queIdx]->getNextPosition();
}


void UIPlaceCardArea::addCardsAtIndex(CardQueue queData, int queIdx)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	ST_RETURN_IF(queData.size() == 0);
	m_queues[queIdx]->addCards(queData);
}

bool UIPlaceCardArea::tryToOpenTheLastCardOfTheQue(int queIdx)
{
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	return m_queues[queIdx]->tryToOpenTheLastCard();
}

bool UIPlaceCardArea::tryToCloseTheLastCardOfTheQue(int queIdx)
{
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	return m_queues[queIdx]->tryToCloseTheLastCard();
}

bool UIPlaceCardArea::isInTheLastCardOfTheQue(int queIdx, Vec2 worldPos)
{
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	return m_queues[queIdx]->intersectsTheCard(worldPos);
}

bool UIPlaceCardArea::isMoveEffectly(int resIdx, int destIdx)
{
	ST_RETURN_FALSE_IF(resIdx < 0 || resIdx >= Place_Area_Num);
	ST_RETURN_FALSE_IF(destIdx < 0 || destIdx >= Place_Area_Num);
	CardSprite *resLastCard = m_queues[resIdx]->getTheLastCard();             //由于已经点击，后面的已经被删除，只要获取最后一个就行
	CardSprite *destLastCard = m_queues[destIdx]->getTheLastCard();
	ST_RETURN_FALSE_IF((!resLastCard) && (!destLastCard));
	if (resLastCard && destLastCard)
	{
		Card resInfo = resLastCard->getCardInfo();
		Card destInfo = destLastCard->getCardInfo();
		if (resInfo.m_number == destInfo.m_number && resInfo.isColorAlike(destInfo))
		{
			return false;
		}
	}
	return true;
}


Vec2 UIPlaceCardArea::getTheLastCardPosOfTheQue(int queIdx)
{
	Vec2 re = Vec2::ZERO;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return Vec2::ZERO;
	}
	CardSprite *lastSp = m_queues[queIdx]->getTheLastCard();
	if (lastSp)
	{
		re = m_queues[queIdx]->convertToWorldSpace(lastSp->getPosition());
	}
	return re;
}

Vec2 UIPlaceCardArea::getCardWorldPosition(int row, int cow)
{
	if (cow < 0 || cow >= Place_Area_Num)
	{
		return Vec2::ZERO;
	}
	return m_queues[cow]->getCardWorldPosition(row);
}

vector<Vec2> UIPlaceCardArea::getAllCardsWorldPosition()
{
	vector<Vec2> re;
	for (int i = 0; i < Place_Area_Num; i++)
	{
		int sz = m_queues[i]->size();
		for (int j = 0; j < sz; j++)
		{
			CardSprite *sp = m_queues[i]->at(j);
			Vec2 pos = sp->getPosition();
			Vec2 worldPos = m_queues[i]->convertToWorldSpace(pos);
			re.push_back(worldPos);
		}
	}
	return re;
}

void UIPlaceCardArea::stTest_scale()
{
	m_queues[6]->openCardMoveBy(10);
}


void UIPlaceCardArea::tryToScale()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		tryToScaleTheQue(i);
	}
}

bool UIPlaceCardArea::tryToScaleTheQue(int queIdx)
{
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	m_queues[queIdx]->tryToScale();
}

void UIPlaceCardArea::openLastCards()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i]->tryToOpenTheLastCard();
	}
}

void UIPlaceCardArea::updateCard()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i]->updateCard();
	}
}

void UIPlaceCardArea::syncData()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		CardQueue que = m_queues[i]->getQueData();
		que.openTheLastCard();
		m_areaData.setQue(i, que);
	}
}

string UIPlaceCardArea::bakAllCards()
{
	syncData();
	return m_areaData.bakAllCards();
}


bool UIPlaceCardArea::isEmpty()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		bool empt = m_queues[i]->isEmpty();
		if (!empt)
		{
			return false;
		}
	}
	return true;
}

vector<Card> UIPlaceCardArea::getCardsAt(int queIdx, int cardIdx)
{
	vector<Card> re;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return re;
	}
	return m_queues[queIdx]->getCardsAfter(cardIdx);
}

void UIPlaceCardArea::addTipLightAt(int queIdx, int cardIdx)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Place_Area_Num);
	m_queues[queIdx]->addTipLightAfterIndex(cardIdx);
}

vector<Card> UIPlaceCardArea::removeTheLastNumCardsOfQue(int queIdx, int num)
{
	vector<Card> reTem;
	if (queIdx < 0 || queIdx >= Place_Area_Num)
	{
		return reTem;
	}
	vector<Card> re= m_queues[queIdx]->removeLastCardByNum(num);
	m_queues[queIdx]->tryToScale();
	syncData();
	return re;
}

void UIPlaceCardArea::removeAllTipLight()
{
	for (int i = 0; i < Place_Area_Num; i++)
	{
		m_queues[i]->removeTipLight();
	}
}
