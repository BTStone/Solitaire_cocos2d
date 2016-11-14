#include "UICollectCardArea.h"
#include "ScreenDirectionManager.h"
UICollectCardArea* UICollectCardArea::create(CollectCardArea areaData)
{
	UICollectCardArea *re = new UICollectCardArea();
	if (re && re->init(areaData))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool UICollectCardArea::init(CollectCardArea areaData)
{
	if (!Node::init())
	{
		return false;
	}
	Size cardSize = CardSprite::getCardSize();
	Size winSize = CLDWinSize();
	m_areaData = areaData;
	initQueues();
	return true;
}

void UICollectCardArea::initQueues()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		CardQueue queData = m_areaData[i];
		m_queues[i] = UICollectCardQueue::create(queData);
		ST_RETURN_IF(!m_queues[i]);
		Size cardSize = CardSprite::getCardSize();
		Size winSize = CLDWinSize();
		float beginX = Place_Area_Gap_X + cardSize.width / 2.0f;
		float posX = beginX + i * (cardSize.width + Place_Area_Gap_X);
		float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
		m_queues[i]->setPosition(Vec2(posX, posY));
		this->addChild(m_queues[i]);
	}
	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		log("UICollectCardArea::initQueues,isLandscape");
		setLandscapeStatus();
	}
}


void UICollectCardArea::setPortraitStatus()
{
	log("UICollectCardArea::setPortraitStatus");
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		Size cardSize = CardSprite::getCardSize();
		Size winSize = CLDWinSize();
		float beginX = Place_Area_Gap_X + cardSize.width / 2.0f;
		float posX = beginX + i * (cardSize.width + Place_Area_Gap_X);
		float posY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
		m_queues[i]->setPosition(Vec2(posX, posY));
	}
}

bool UICollectCardArea::isEmpty()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		bool isEmp = m_queues[i]->isEmpty();
		if (!isEmp)
		{
			return false;
		}
	}
	return true;
}

void UICollectCardArea::setLandscapeStatus()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		Size cardSize = CardSprite::getCardSize();
		Size winSize = CLDWinSize();
		float posX = Place_Area_Gap_X + cardSize.width / 2.0f;
		log("UICollectCardArea::setLandscapeStatus,posX:%f", posX);
		float beginY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
		float posY = beginY - i*cardSize.height*1.05;
		m_queues[i]->setPosition(Vec2(posX, posY));
	}
}

TransportCardQueue* UICollectCardArea::search(Vec2 worldPos)
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


int UICollectCardArea::getQueIndexByPos(Vec2 worldPos)
{
	Vec2 pos = convertToNodeSpace(worldPos);
	Size cardSize = CardSprite::getCardSize();
	float gap = Place_Area_Gap_X;
	float beginX = Place_Area_Gap_X + cardSize.width / 2.0f;
	int maxX = beginX + (Place_Area_Gap_X + cardSize.width)*(Collect_Area_Num - 1) + cardSize.width / 2.0f;
	if (maxX < pos.x){ return -1; }
	int index = pos.x / (gap + cardSize.width);

	bool isLandscape = ScreenDirectionManager::share()->getScreenType() == kScreenLandscape;
	if (isLandscape)
	{
		Size winSize = CLDWinSize();
		float beginY = winSize.height - Game_Layer_Head_Label_Height - cardSize.height*0.5;
		float maxY = beginY + cardSize.height*0.5;
		if (maxY < pos.y){ return -1;}
		float dis = maxY - pos.y;
		index = dis / (gap + cardSize.height);
	}
	//index = index < 0 ? 0 : index;
	//index = index > (Collect_Area_Num - 1) ? (Collect_Area_Num - 1) : index;

	index = index < 0 ? -1 : index;
	index = index > (Collect_Area_Num - 1) ? -1 : index;
	return index;
}

int UICollectCardArea::isContain(Vec2 worldPos)
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		if (m_queues[i]->intersectsTheCard(worldPos))
		{
			return i;
		}
	}
	return -1;
}

int UICollectCardArea::canCollectTheCard(TransportCardQueue *que)
{
	CardArea resArea = que->getResArea();
	int resQueIdx = que->getResQueIndex();
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		bool isRes = ((resArea == COLLECT_AREA) && (resQueIdx == i));
		if (canCollectTheCard(que, i) && !isRes)
		{
			return i;
		}
	}
	return -1;
}

int UICollectCardArea::canCollectTheCard(Card card)
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		bool canCollect = m_queues[i]->canCollectTheCard(card);
		if (canCollect)
		{
			return i;
		}
	}
	return -1;
}

bool UICollectCardArea::canCollectTheCard(TransportCardQueue *que, int queIdx)
{
	ST_RETURN_FALSE_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	int sz = que->size();
	ST_RETURN_FALSE_IF(sz != 1);
	CardSprite *firstCard = que->getTheFirstCard();
	ST_RETURN_FALSE_IF(!firstCard);
	Card firstCardInfo = firstCard->getCardInfo();
	return m_queues[queIdx]->canCollectTheCard(firstCardInfo);
}


void UICollectCardArea::collectTheCard(TransportCardQueue *que, int queIdx)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	int sz = que->size();
	ST_RETURN_IF(sz != 1);
	CardSprite *firstCard = que->getTheFirstCard();
	ST_RETURN_IF(!firstCard);
	Card firstCardInfo = firstCard->getCardInfo();
	m_queues[queIdx]->collectTheCard(firstCardInfo);
}

Vec2 UICollectCardArea::getNextPosition(int queIdx)
{
	Vec2 re = Vec2::ZERO;
	if (queIdx < 0 || queIdx >= Collect_Area_Num)
	{
		return re;
	}
	return m_queues[queIdx]->getNextPosition();
}

void UICollectCardArea::addCardsAtIndex(CardQueue queData, int queIdx, bool withEffect)
{
	ST_RETURN_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	ST_RETURN_IF(queData.size() == 0);
	m_queues[queIdx]->addCards(queData, withEffect);
	syncData();
}

void UICollectCardArea::updateCard()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		m_queues[i]->updateCard();
	}
}

void UICollectCardArea::syncData()
{
	//同步信息
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		CardQueue que = m_queues[i]->getQueData();
		m_areaData.setQue(i, que);
	}
}

string UICollectCardArea::bakAllCards()
{
	syncData();
	return m_areaData.bakAllCards();
}

Card UICollectCardArea::getTheNextCollectCard()
{
	Card reCard;
	reCard.m_number = 14;
	reCard.m_color = BLACK_CARD;
	vector<Card> collectCards = getTheCardsCanCollect();
	int sz = collectCards.size();
	if (sz <= 0){ return INVALID_CARD; }
	for (int i = 0; i < sz; i++)
	{
		if (collectCards[i].m_number < reCard.m_number)
		{
			reCard = collectCards[i];
		}
	}
	return reCard;
}

vector<Card> UICollectCardArea::getTheCardsCanCollect()
{
	vector<Card> cards;
	//添加4个A
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		Card cd;
		cd.m_number = 1;
		cd.m_color = (CardColor)i;
		cards.push_back(cd);
	}

	for (int i = 0; i < Collect_Area_Num; i++)
	{
		bool isEmp = m_queues[i]->isEmpty();
		if (!isEmp)
		{
			CardSprite * lastSp = m_queues[i]->getTheLastCard();
			Card lastCd = lastSp->getCardInfo();
			int sz = cards.size();
			//删除已有花色A
			for (int i = 0; i < sz; i++)
			{
				Card c;
				c.m_number = 1;
				c.m_color = lastCd.m_color;
				if (cards[i] == c)
				{
					cards.erase(cards.begin() + i);
					break;
				}
			}

			//添加下一张牌
			if (lastCd.m_number > 0 && lastCd.m_number < 14)
			{
				cards.push_back(Card(lastCd.m_number + 1, lastCd.m_color, true));
			}

		}
	}
	return cards;
}

int UICollectCardArea::getQueSize(int queIdx)
{
	if (queIdx < 0 || queIdx >= Collect_Area_Num)
	{
		return 0;
	}
	return m_queues[queIdx]->size();
}

int UICollectCardArea::getAllCardSize()
{
	int re = 0;
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		re += getQueSize(i);
	}
	return re;
}

void UICollectCardArea::clear()
{
	for (int i = 0; i < Collect_Area_Num; i++)
	{
		m_queues[i]->clear();
	}
}


vector<Card> UICollectCardArea::removeTheLastNumCardsOfQue(int queIdx, int num)
{
	vector<Card> re;
	if (queIdx < 0 || queIdx >= Collect_Area_Num)
	{
		return re;
	}
	return m_queues[queIdx]->removeLastCardByNum(num);
}


CardSprite* UICollectCardArea::getTheLastCard(int queIdx)
{
	ST_RETURN_NULL_IF(queIdx < 0 || queIdx >= Collect_Area_Num);
	return m_queues[queIdx]->getTheLastCard();
}