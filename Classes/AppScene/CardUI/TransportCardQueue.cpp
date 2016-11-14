#include "TransportCardQueue.h"


///=====================================================begin STShock =========================================

STShock* STShock::create(float dt)
{
	STShock* pRet = new STShock();

	if (pRet && pRet->init(dt)) {
		pRet->autorelease();
	}

	return pRet;
}

bool STShock::init(float dt)
{
	m_time = 0;
	return ActionInterval::initWithDuration(dt);
}


void STShock::shock()
{
	Vec2 originPoint = _target->getPosition();
	float t = _duration / 10.0;
	float by = 10.0f;
	MoveBy *move1 = MoveBy::create(t, CCPoint(by, 0));
	ST_RETURN_IF(!move1);
	MoveBy *move2 = MoveBy::create(t, CCPoint(-by * 2, 0));
	ST_RETURN_IF(!move2);
	MoveBy *move3 = MoveBy::create(t, CCPoint(by * 2, 0));
	ST_RETURN_IF(!move3);
	MoveBy *move4 = MoveBy::create(t, CCPoint(-by * 2, 0));
	ST_RETURN_IF(!move4);
	MoveBy *move5 = MoveBy::create(t, CCPoint(by * 2, 0));
	ST_RETURN_IF(!move5);
	MoveBy *move6 = MoveBy::create(t, CCPoint(-by * 2, 0));
	ST_RETURN_IF(!move6);
	MoveBy *move7 = MoveBy::create(t, CCPoint(by * 2, 0));
	ST_RETURN_IF(!move7);
	MoveBy *move8 = MoveBy::create(t, CCPoint(-by * 2, 0));
	ST_RETURN_IF(!move8);
	MoveBy *move9 = MoveBy::create(t, CCPoint(by * 2, 0));
	ST_RETURN_IF(!move9);
	MoveTo *move10 = MoveTo::create(t, originPoint);
	ST_RETURN_IF(!move10);
	Sequence *seq = Sequence::create(move1, move2, move3, move4, move5, move6, move7, move8, move9, move10, NULL);
	ST_RETURN_IF(!seq);
	_target->runAction(seq);
}


void STShock::update(float time) {
	CC_UNUSED_PARAM(time);
	ST_RETURN_IF(m_time++ > 0);
	shock();
}

///=====================================================end STShock ===========================================

TransportCardQueue* TransportCardQueue::create(CardQueue que, CardArea area, int preIndex)
{
	TransportCardQueue *re = new TransportCardQueue();
	if (re && re->init(que, area, preIndex))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool TransportCardQueue::init(CardQueue que, CardArea area, int preIndex)
{
	if (!UICardQueue::init(que))
	{
		return false;
	}
	m_resArea = area;
	m_resQueIndex = preIndex;
	m_destArea = COLLECT_AREA;
	m_destQueIndex = 0;
	resetCardBg();
	setPortraitStatus();
	return true;
}


void TransportCardQueue::setPortraitStatus()
{
	Size cardSize = CardSprite::getCardSize();
	float gapY = cardSize.height * Place_Area_Gap_Y;
	int len = m_cards.size();
	for (int i = 0; i < len; i++)
	{
		CardSprite *sp = m_cards[i];
		sp->setPosition(Vec2(0, -i*gapY));
	}
}

void TransportCardQueue::setHandOpenAreaStatus()
{
	Size cardSize = CardSprite::getCardSize();
	float gapX = cardSize.width * Hand_Open_Area_Gap_X;
	int len = m_cards.size();
	for (int i = 0; i < len; i++)
	{
		CardSprite *sp = m_cards[i];
		int off = (len - i - 1);
		off = (off > 2) ? 2 : off;
		sp->setPosition(Vec2(-(off)*gapX, 0));
	}
}


void TransportCardQueue::setLandscapeStatus()
{
	Size cardSize = CardSprite::getCardSize();
	float gapX = cardSize.width * Hand_Open_Area_Gap_X;
	int len = m_cards.size();
	for (int i = 0; i < len; i++)
	{
		CardSprite *sp = m_cards[i];
		sp->setPosition(Vec2(-(len - i - 1)*gapX, 0));
	}
}

void TransportCardQueue::resetCardBg()
{
	if (m_cardBg)
	{
		m_cardBg->setVisible(false);
	}
}
