#include "UIHandClosedCardQueue.h"

UIHandClosedCardQueue* UIHandClosedCardQueue::create(CardQueue queData)
{
	UIHandClosedCardQueue *re = new UIHandClosedCardQueue();
	if (re && re->init(queData))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}


bool UIHandClosedCardQueue::init(CardQueue queData)
{
	if (!UICardQueue::init(queData))
	{
		return false;
	}
	m_tip = NULL;
	m_resArea = HAND_CLOSE_AREA;
	initCardsPosition();
	resetCardBg();
	return true;
}


void UIHandClosedCardQueue::resetCardBg()
{
	Texture2D *bgTexture = TextureCache::getInstance()->addImage("Pic/Card-Site-Empty.png");
	ST_RETURN_IF(!bgTexture);
	Sprite *sp = Sprite::create("Pic/Card-Site-Empty.png");
	ST_RETURN_IF(!sp);
	Size spSize = sp->getContentSize();
	SpriteFrame *bgFrame = SpriteFrame::createWithTexture(bgTexture, Rect(0, 0, spSize.width, spSize.height));
	ST_RETURN_IF(!bgFrame);
	if (m_cardBg)
	{
		m_cardBg->setSpriteFrame(bgFrame);
	}
	Sprite *replaySp = Sprite::create("Pic/UI-Icon-Replay-x48.png");
	ST_RETURN_IF(!replaySp);
	replaySp->setPosition(Vec2(spSize.width / 2.0, spSize.height / 2.0));
	m_cardBg->addChild(replaySp);
}


void UIHandClosedCardQueue::initCardsPosition()
{

}

void UIHandClosedCardQueue::addCard(Card card)
{

}

void UIHandClosedCardQueue::addCards(CardQueue queData)
{
	m_queData.pushBack(queData);
	int sz = queData.size();
	for (int i = 0; i < sz; i++)
	{
		Card card = queData[i];
		card.m_isOpen = false;
		CardSprite *cardSp = CardSprite::create(card);
		ST_RETURN_IF(!cardSp);
		Vec2 pos = getWorldPosOfCardBg();
		pos = convertToNodeSpace(pos);
		cardSp->setPosition(pos);
		this->addChild(cardSp);
		m_cards.push_back(cardSp);
	}
}


TransportCardQueue* UIHandClosedCardQueue::search(Vec2 worldPos)
{
	bool isContain = containThePos(worldPos);
	ST_RETURN_NULL_IF(!isContain);
	removeTip();
	vector<Card> cards;
	TransportCardQueue *re = TransportCardQueue::create(cards, m_resArea);
	if (re)
	{
		re->setResArea(HAND_CLOSE_AREA);
		re->setResQueIndex(0);
	}
	return re;
}


bool UIHandClosedCardQueue::containThePos(Vec2 worldPos)
{
	Size cardSize = CardSprite::getCardSize();
	Vec2 originPos = Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rect = Rect(originPos, cardSize);
	Vec2 pos = convertToNodeSpace(worldPos);
	return rect.containsPoint(pos);
}

void UIHandClosedCardQueue::addTip()
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
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UIHandClosedCardQueue::removeTip));
		ST_RETURN_IF(!callFunc);
		Sequence *sq = Sequence::create(repeat, callFunc, NULL);
		ST_RETURN_IF(!sq);
		m_tip->runAction(sq);
	}
}

void UIHandClosedCardQueue::removeTip()
{
	if (m_tip)
	{
		m_tip->removeFromParent();
		m_tip = NULL;
	}
}
