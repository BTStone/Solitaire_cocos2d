#include "UIHandOpenCardQueue.h"


UIHandOpendCardQueue* UIHandOpendCardQueue::create(CardQueue queData)
{
	UIHandOpendCardQueue *re = new UIHandOpendCardQueue();
	if (re && re->init(queData))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool UIHandOpendCardQueue::init(CardQueue queData)
{
	if (!UICardQueue::init(queData))
	{
		return false;
	}
	m_resArea = HAND_OPEN_AREA;
	m_tip = NULL;
	initCardsPosition();
	resetCardBg();
	return true;
}

void UIHandOpendCardQueue::initCardsPosition()
{
	Size cardSize = CardSprite::getCardSize();
	float gapX = cardSize.width * Hand_Open_Area_Gap_X;
	int len = m_cards.size();
	int count = 0;
	
	for (int i = len - 1; i >= 0; i--)
	{
		CardSprite *sp = m_cards[i];
		if (count < 3)
		{
			sp->setPosition(Vec2(-count*gapX, 0));
		}
		else
		{
			sp->setPosition(Vec2(-2*gapX, 0));
		}
		count++;
	}
}

void UIHandOpendCardQueue::resetCardBg()
{
	if (m_cardBg)
	{
		m_cardBg->setVisible(false);
	}
}

void UIHandOpendCardQueue::addCard(Card card)
{
	m_queData.pushBack(card);
	CardSprite *sp = CardSprite::create(card);
	ST_RETURN_IF(!sp);

}

void UIHandOpendCardQueue::addCards(CardQueue queData)
{
	m_queData.pushBack(queData);
	int sz = queData.size();
	Size cardSize = CardSprite::getCardSize();
	float gapX = cardSize.width * Hand_Open_Area_Gap_X;
	for (int i = 0; i < sz; i++)
	{
		Card card = queData[i];
		CardSprite *cardSp = CardSprite::create(card);
		ST_RETURN_IF(!cardSp);
		int off = sz - i - 1;
		off = (off > 2) ? 2 : off;
		cardSp->setPosition(Vec2(-(off)*gapX, 0));
		this->addChild(cardSp);
		m_cards.push_back(cardSp);
	}
	//initCardsPosition();
}


TransportCardQueue* UIHandOpendCardQueue::search(Vec2 worldPos)
{
	CardSprite *lastSp = getTheLastCard();
	ST_RETURN_NULL_IF(!lastSp);
	bool isContain = lastSp->containThePos(worldPos);
	ST_RETURN_NULL_IF(!isContain);
	removeTip();
	Card card = removeTheLastCard();
	ST_RETURN_NULL_IF(card.isInvalidCard());
	vector<Card> cards;
	cards.push_back(card);
	TransportCardQueue *re = TransportCardQueue::create(cards, m_resArea);
	ST_RETURN_NULL_IF(!re);
	re->setResArea(HAND_OPEN_AREA);
	re->setResQueIndex(0);
	Vec2 pos = convertToWorldSpace(Vec2::ZERO);
	re->setPosition(pos);
	return re;
}

void UIHandOpendCardQueue::addCardEffectWithNum(int addNum)
{
	if (addNum == 1)
	{
		lastTwoCardsMoveByOnePosToLeft();
	}
	else if (addNum > 1)
	{
		lastTwoCardsMoveToMostLeft();
	}
}


void UIHandOpendCardQueue::removeCardEffectWithNum(int remNum)
{
	if (remNum == 1)
	{
		lastTwoCardsMoveByOnePosToRight();
	}
	else if (remNum > 1)
	{
		lastTwoCardsMoveToRightIrregular();
	}
}

void UIHandOpendCardQueue::lastTwoCardsMoveByOnePosToLeft()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	int num = 0;
	float dt = Move_And_Untouchable_Time;
	for (int i = sz - 1; num<2 && i >= 0; i--)
	{
		num++;
		Size cardSize = CardSprite::getCardSize();
		float byX = cardSize.width * Hand_Open_Area_Gap_X;
		MoveBy *mvBy = MoveBy::create(dt, Vec2(-byX, 0));
		ST_RETURN_IF(!mvBy);
		CardSprite *sp = m_cards[i];
		sp->runAction(mvBy);
	}
}

void UIHandOpendCardQueue::lastTwoCardsMoveToMostLeft()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	int num = 0;
	float dt = Move_And_Untouchable_Time;
	for (int i = sz - 1; num < 2 && i >= 0; i--)
	{
		num++;
		Size cardSize = CardSprite::getCardSize();
		float byX = cardSize.width * Hand_Open_Area_Gap_X;
		MoveTo *mvTo = MoveTo::create(dt, Vec2(-2*byX, 0));
		ST_RETURN_IF(!mvTo);
		CardSprite *sp = m_cards[i];
		sp->runAction(mvTo);
	}
}


void UIHandOpendCardQueue::lastTwoCardsMoveByOnePosToRight()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	int num = 0;
	float dt = Move_And_Untouchable_Time;
	for (int i = sz - 1; num < 2 && i >= 0; i--)
	{
		num++;
		Size cardSize = CardSprite::getCardSize();
		float byX = cardSize.width * Hand_Open_Area_Gap_X;
		MoveBy *mvBy = MoveBy::create(dt, Vec2(byX, 0));
		ST_RETURN_IF(!mvBy);
		CardSprite *sp = m_cards[i];
		sp->runAction(mvBy);
	}
}


void UIHandOpendCardQueue::lastTwoCardsMoveToRightIrregular()
{
	int sz = m_cards.size();
	ST_RETURN_IF(sz <= 0);
	int num = 0;
	float dt = Move_And_Untouchable_Time;
	for (int i = sz - 1; num < 2 && i >= 0; i--)
	{
		num++;
		Size cardSize = CardSprite::getCardSize();
		float byX = cardSize.width * Hand_Open_Area_Gap_X;
		MoveBy *mvBy = MoveBy::create(dt, Vec2((3-num)*byX, 0));
		ST_RETURN_IF(!mvBy);
		CardSprite *sp = m_cards[i];
		sp->runAction(mvBy);
	}
}



void UIHandOpendCardQueue::addTip()
{
	if (!m_tip)
	{
		Size cardSize = CardSprite::getCardSize();
		float cardScale = CardSprite::getCardScale();
		m_tip = Sprite::create("Pic/UI_Card_Yellow_Light.png");
		ST_RETURN_IF(!m_tip);
		m_tip->setScale(cardScale*1.2);
		this->addChild(m_tip, 100);
		CCFadeIn *fadeIn = CCFadeIn::create(1.0f);
		ST_RETURN_IF(!fadeIn);
		CCFadeOut *fadeO = CCFadeOut::create(1.0f);
		ST_RETURN_IF(!fadeO);
		Sequence *seq = Sequence::create(fadeIn, fadeO, NULL);
		ST_RETURN_IF(!seq);
		Repeat *repeat = Repeat::create(seq, 5);
		ST_RETURN_IF(!seq);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UIHandOpendCardQueue::removeTip));
		ST_RETURN_IF(!callFunc);
		Sequence *sq = Sequence::create(repeat, callFunc, NULL);
		ST_RETURN_IF(!sq);
		m_tip->runAction(sq);
	}
}

void UIHandOpendCardQueue::removeTip()
{
	if (m_tip)
	{
		m_tip->removeFromParent();
		m_tip = NULL;
	}
}

bool UIHandOpendCardQueue::isOnlyOneCard()
{
	int sz = m_cards.size();
	return (sz == 1);
}