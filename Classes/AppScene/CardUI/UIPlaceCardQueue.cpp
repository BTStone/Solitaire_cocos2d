#include "UIPlaceCardQueue.h"

UIPlaceCardQueue* UIPlaceCardQueue::create(CardQueue queData)
{
	UIPlaceCardQueue *re = new UIPlaceCardQueue();
	if (re && re->init(queData))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool UIPlaceCardQueue::init(CardQueue queData)
{
	if (!UICardQueue::init(queData))
	{
		return false;
	}
	m_resArea = PLACE_AREA;
	initCardsPosition();
	resetCardBg();
	return true;
}

float UIPlaceCardQueue::getOpenCardMoveByLenth()
{
	float overLenth = getOverLenth();
	int openNum = getOpenCardNum();
	if (openNum < 2){ return 0; }
	float mvBy = 0;
	if (overLenth > 0)   //超出屏幕了
	{
		mvBy = overLenth / (openNum - 1);
	}
	else if (m_openCardGapY < m_originGapY)
	{
		mvBy = overLenth / (openNum - 1);
		if (m_openCardGapY - mvBy > m_originGapY)
		{
			mvBy = m_openCardGapY - m_originGapY;
		}
	}
	m_openCardGapY -= mvBy;
	return mvBy;
}

bool UIPlaceCardQueue::tryToScale()
{
	float mvBy = getOpenCardMoveByLenth();
	if (fabs(mvBy - 0) < 0.5){ return true; }
	openCardMoveBy(mvBy);
	return true;

	bool bl = alreadyOverScreen();           //是否已经超出屏幕
	if (bl)
	{
		scaleWhenOverScreen();               //缩小
		return true;
	}
	return scaleWhenRemoveCard();            //放大
}

bool UIPlaceCardQueue::scaleWhenRemoveCard()
{
	bool bl1 = isOriginGapOfOpenCard();       //是否是初始间距
	ST_RETURN_FALSE_IF(bl1);
	float overLenth = getOverLenth();
	ST_RETURN_FALSE_IF(overLenth > 0);
	int openNum = getOpenCardNum();
	ST_RETURN_FALSE_IF(openNum <= 1);
	int scaleNum = openNum - 1;             //第一张不进行位置缩放
	float lenthPer = -overLenth / scaleNum; //移动多少
	if (m_openCardGapY + lenthPer > m_originGapY)
	{
		lenthPer = m_originGapY - m_openCardGapY;
	}
	m_openCardGapY += lenthPer;
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		float dt = 0.2f;
		float by = -i*lenthPer;
		MoveBy *mvBy = MoveBy::create(dt, Vec2(0, by));
		ST_RETURN_FALSE_IF(!mvBy);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UIPlaceCardQueue::scaleOverCallFunc));
		ST_RETURN_FALSE_IF(!callFunc);
		Sequence *seq = Sequence::create(mvBy, callFunc, NULL);
		ST_RETURN_FALSE_IF(!seq);
		NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)false);
		m_cards[i]->runAction(seq);
	}
}

int UIPlaceCardQueue::getOpenCardNum()
{
	int count = 0;
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		Card cd = m_cards[i]->getCardInfo();
		if (cd.m_isOpen)
		{
			count++;
		}
	}
	//log("getOpenCardNum,count:%d", count);
	return count;
}


void UIPlaceCardQueue::openCardMoveBy(float mvBy)
{
	int len = mvBy;
	int openNum = getOpenCardNum();
	ST_RETURN_IF(openNum <= 1);
	int scaleNum = openNum - 1;             //第一张不进行位置缩放
	int sz = m_cards.size();
	int beginMoveIdx = sz - openNum + 1;
	for (int i = beginMoveIdx; i < sz; i++)
	{
		bool isOpen = m_cards[i]->getCardInfo().m_isOpen;
		if (isOpen)
		{
			Vec2 pos = m_cards[i]->getPosition();
			float dt = 0.2f;
			float by = (i-beginMoveIdx+1)*len;
			MoveTo *mvBy = MoveTo::create(dt,pos + Vec2(0, by));
			ST_RETURN_IF(!mvBy);
			CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UIPlaceCardQueue::scaleOverCallFunc));
			ST_RETURN_IF(!callFunc);
			Sequence *seq = Sequence::create(mvBy, callFunc, NULL);
			ST_RETURN_IF(!seq);
			if (i == 0)
			{
				///NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)false);
			}

			if (0)///(i == sz - 1)
			{
				m_cards[i]->runAction(seq);
			}
			else
			{
				m_cards[i]->runAction(mvBy);
			}
		}
		
	}
}

void UIPlaceCardQueue::scaleWhenOverScreen()
{
	float overLenth = getOverLenth();
	ST_RETURN_IF(overLenth <= 0);
	int openNum = getOpenCardNum();
	ST_RETURN_IF(openNum <= 1);
	int scaleNum = openNum - 1;             //第一张不进行位置缩放
	float lenthPer = overLenth / scaleNum;  //移动多少
	m_openCardGapY -= lenthPer;
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		float dt = 0.2f;
		float by = i*lenthPer;
		MoveBy *mvBy = MoveBy::create(dt, Vec2(0, by));
		ST_RETURN_IF(!mvBy);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UIPlaceCardQueue::scaleOverCallFunc));
		ST_RETURN_IF(!callFunc);
		Sequence *seq = Sequence::create(mvBy, callFunc, NULL);
		ST_RETURN_IF(!seq);
		if (i == 0)
		{
			NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)false);
		}
		
		if (i == sz - 1)
		{
			m_cards[i]->runAction(seq);
		}
		m_cards[i]->runAction(mvBy);
	}
}

void UIPlaceCardQueue::scaleOverCallFunc()
{
	NotificationCenter::getInstance()->postNotification(ST_SET_GAME_LAYER_TOUCH_ENABLE, (Ref*)true);
}

float UIPlaceCardQueue::getOverLenth()
{
	Size cardSize = CardSprite::getCardSize();
	CardSprite *lastCard = getTheLastCard();
	ST_RETURN_FALSE_IF(!lastCard);
	Vec2 lastPos = lastCard->getPosition();
	Vec2 lastWorldPos = convertToWorldSpace(lastPos);
	return ((cardSize.height*0.5) - lastWorldPos.y);
}

bool UIPlaceCardQueue::willOverScreen()
{
	Size cardSize = CardSprite::getCardSize();
	CardSprite *lastCard = getTheLastCard();
	ST_RETURN_FALSE_IF(!lastCard);
	Vec2 lastPos = lastCard->getPosition();
	Vec2 lastWorldPos = convertToWorldSpace(lastPos);
	return (lastWorldPos.y < (m_openCardGapY+cardSize.height*0.5));
}

bool UIPlaceCardQueue::alreadyOverScreen()
{
	Size cardSize = CardSprite::getCardSize();
	CardSprite *lastCard = getTheLastCard();
	ST_RETURN_FALSE_IF(!lastCard);
	Vec2 lastPos = lastCard->getPosition();
	Vec2 lastWorldPos = convertToWorldSpace(lastPos);
	return (lastWorldPos.y < cardSize.height*0.5);
}

bool UIPlaceCardQueue::isOriginGapOfOpenCard()
{
	return (fabs(m_originGapY - m_openCardGapY) <= 0.05);
}


void UIPlaceCardQueue::initCardsPosition()
{
	Size cardSize = CardSprite::getCardSize();
	float gapY = cardSize.height * Place_Area_Gap_Y;
	m_originGapY = gapY;
	m_openCardGapY = m_originGapY;
	int len = m_cards.size();
	for (int i = 1; i < len; i++)
	{
		CardSprite *sp = m_cards[i];
		CardSprite *preSp = m_cards[i-1];
		Card preInfo = preSp->getCardInfo();
		float posY = preSp->getPositionY() - gapY;
		if (!preInfo.m_isOpen)
		{
			posY = preSp->getPositionY() - gapY * CLOSE_CARD_GAP_SCALE;
		}
		sp->setPosition(Vec2(0, posY));
	}
}

void UIPlaceCardQueue::resetCardBg()
{
	Texture2D *bgTexture = TextureCache::getInstance()->addImage("Pic/Card-Site-Empty.png");
	ST_RETURN_IF(!bgTexture);
	Sprite *sp = Sprite::create("Pic/Card-Site-Empty.png");
	ST_RETURN_IF(!sp);
	Size spSize = sp->getContentSize();
	SpriteFrame *bgFrame = SpriteFrame::createWithTexture(bgTexture,Rect(0,0,spSize.width,spSize.height));
	ST_RETURN_IF(!bgFrame);
	if (m_cardBg)
	{
		m_cardBg->setSpriteFrame(bgFrame);
	}
}


TransportCardQueue* UIPlaceCardQueue::search(Vec2 worldPos)
{
	int sz = m_cards.size();
	int index = -1;
	for (int i = (sz-1); i >= 0; i--)
	{
		CardSprite *sp = m_cards[i];
		Card cardInfo = sp->getCardInfo();
		if (cardInfo.m_isOpen && sp->containThePos(worldPos))
		{
			index = i;
			break;
		}
	}
	ST_RETURN_NULL_IF(index == -1);
	Vec2 pos = getPositionAt(index);
	pos = convertToWorldSpace(pos);
	vector<Card> cards = removeCardsAfterIndex(index);
	TransportCardQueue *re = TransportCardQueue::create(cards, m_resArea);
	ST_RETURN_NULL_IF(!re);
	removeTipLight();
	re->setPosition(pos);
	return re;
}

void UIPlaceCardQueue::addCards(CardQueue queData)
{
	m_queData.pushBack(queData);
	int sz = queData.size();
	for (int i = 0; i < sz; i++)
	{
		Card card = queData[i];
		CardSprite *cardSp = CardSprite::create(card);
		ST_RETURN_IF(!cardSp);
		Vec2 pos = getNextPosition();
		pos = convertToNodeSpace(pos);
		cardSp->setPosition(pos);
		this->addChild(cardSp);
		m_cards.push_back(cardSp);
	}
	tryToScale();
}


Vec2 UIPlaceCardQueue::getPositionAt(int index)
{
	int sz = m_cards.size();
	if (index < 0 || index >= sz)
	{
		return Vec2::ZERO;
	}
	return m_cards[index]->getPosition();
	/*
	float posX = 0;
	Size cardSize = CardSprite::getCardSize();
	float posY = - (cardSize.height) * index * Place_Area_Gap_Y;
	return Vec2(posX, posY);*/
}

bool UIPlaceCardQueue::canPlaceTheCard(Card card)
{
	return m_queData.canPlaceTheCard(card);
}


Vec2 UIPlaceCardQueue::getNextPosition()
{
	CardSprite *lastCard = getTheLastCard();
	if (!lastCard)
	{
		return getWorldPosOfCardBg();
	}
	Size cardSize = CardSprite::getCardSize();
	Vec2 lastPos = lastCard->getPosition();
	//float offY = -cardSize.height * Place_Area_Gap_Y;
	float offY = -m_openCardGapY;
	if (!lastCard->getCardInfo().m_isOpen)
	{
		offY = -m_originGapY * CLOSE_CARD_GAP_SCALE;
	}
	Vec2 nextPos = lastPos + Vec2(0, offY);
	nextPos = convertToWorldSpace(nextPos);
	return nextPos;
}

bool UIPlaceCardQueue::tryToOpenTheLastCard()
{
	CardSprite *lastSp = getTheLastCard();
	ST_RETURN_FALSE_IF(!lastSp);
	bool isOpen = lastSp->getCardInfo().m_isOpen;
	lastSp->open();
	m_queData.openTheLastCard();
	return (!isOpen);
}

bool UIPlaceCardQueue::tryToCloseTheLastCard()
{
	CardSprite *lastSp = getTheLastCard();
	ST_RETURN_FALSE_IF(!lastSp);
	bool isOpen = lastSp->getCardInfo().m_isOpen;
	lastSp->close();
	m_queData.closeTheLastCard();
	return (!isOpen);
}

bool UIPlaceCardQueue::intersectsTheCard(Vec2 worldCardPos)
{
	Vec2 lastCardPos = Vec2::ZERO;
	CardSprite *lastCard = getTheLastCard();
	if (lastCard)
	{
		lastCardPos = lastCard->getPosition();
	}
	Vec2 touchPos = convertToNodeSpace(worldCardPos);
	Size cardSize = CardSprite::getCardSize();
	Vec2 originPos = lastCardPos + Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rect = Rect(originPos, cardSize);
	Vec2 posOr = touchPos + Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rt = Rect(posOr, cardSize);
	bool bl1 = rect.intersectsRect(rt);
	bool bl2 = (touchPos.x >= originPos.x && touchPos.x <= (originPos.x + cardSize.width));
	return bl1 && bl2;
}



bool UIPlaceCardQueue::isInTheLastCard(Vec2 worldPos)
{
	Vec2 lastCardPos = Vec2::ZERO;
	CardSprite *lastCard = getTheLastCard();
	if (lastCard)
	{
		lastCardPos = lastCard->getPosition();
	}
	Vec2 touchPos = convertToNodeSpace(worldPos);
	Size cardSize = CardSprite::getCardSize();
	Vec2 originPos = lastCardPos + Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rect = Rect(originPos, cardSize);
	return rect.containsPoint(touchPos);
	Vec2 posOr = Vec2(touchPos.x - cardSize.width / 2.0, touchPos.y - cardSize.height / 2.0);
	Rect rt = Rect(posOr, cardSize);
	bool bl1 = rect.intersectsRect(rt);
	bool bl2 = (touchPos.x >= originPos.x && touchPos.x <= (originPos.x + cardSize.width));
	return bl1 && bl2;
}

CardSprite* UIPlaceCardQueue::getTheSecondToLastCard()
{
	int sz = m_cards.size();
	ST_RETURN_NULL_IF(sz < 2);
	return m_cards[sz - 2];
}

CardSprite* UIPlaceCardQueue::at(int idx)
{
	return m_cards[idx];
}

Vec2 UIPlaceCardQueue::getCardWorldPosition(int idx)
{
	Vec2 re = Vec2::ZERO;
	CardSprite *sp = m_cards[idx];
	if (!sp) { return re; }
	Vec2 spPos = sp->getPosition();
	re = convertToWorldSpace(spPos);
	return re;
}

vector<Card> UIPlaceCardQueue::getCardsAfter(int idx)
{
	int sz = m_cards.size();
	vector<Card> re;
	for (int i = 0; i < sz; i++)
	{
		if (i >= idx)
		{
			CardSprite *sp = m_cards[i];
			Card cd = sp->getCardInfo();
			re.push_back(cd);
		}
	}
	return re;
}

bool UIPlaceCardQueue::containTheCard(Card card)
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		Card cd = m_cards[i]->getCardInfo();
		if (cd == card)
		{
			return true;
		}
	}
	return false;
}