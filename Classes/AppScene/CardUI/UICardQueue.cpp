#include "UICardQueue.h"

UICardQueue* UICardQueue::create(CardQueue queData)
{
	UICardQueue *re = new UICardQueue();
	if (re && re->init(queData))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool UICardQueue::init(CardQueue queData)
{
	if (!Node::init())
	{
		return false;
	}
	m_queData = queData;
	m_cardBg = NULL;
	m_tipLight = NULL;
	initCardBg();
	initCards();
	return true;
}

void UICardQueue::initCardBg()
{
	m_cardBg = Sprite::create("Pic/Card-Site.png");
	ST_RETURN_IF(!m_cardBg);
	float cardScale = CardSprite::getCardScale();
	m_cardBg->setScale(cardScale);
	this->addChild(m_cardBg, 0);

}

void UICardQueue::initCards()
{
	int len = m_queData.size();
	for (int i = 0; i < len; i++)
	{
		Card card = m_queData[i];
		CardSprite *cardSp = CardSprite::create(card);
		ST_RETURN_IF(!cardSp);
		cardSp->setPosition(Vec2::ZERO);
		this->addChild(cardSp);
		m_cards.push_back(cardSp);
	}
}

Card UICardQueue::removeTheLastCard()
{
	int len = m_cards.size();
	if (len == 0){ return INVALID_CARD; }
	CardSprite *lastSp = m_cards[len - 1];
	lastSp->removeFromParent();
	m_cards.pop_back();
	Card card = m_queData.removeTheLastCard();
	return card;
}

vector<Card> UICardQueue::removeCardsAfterIndex(int index)
{
	vector<Card> reCards;
	int sz = m_cards.size();
	while (sz > index)
	{
		CardSprite * lastCardSp = m_cards[sz - 1];
		Card lastCard = lastCardSp->getCardInfo();
		lastCardSp->removeFromParent();
		m_cards.erase(m_cards.begin() + sz - 1);
		sz = m_cards.size();
	}
	reCards = m_queData.removeCardsAfterIndex(index);
	return reCards;
}

vector<Card> UICardQueue::removeLastCardByNum(int num)
{
	vector<Card> reCards;
	for (int i = 0; i < num; i++)
	{
		if (m_cards.size() > 0)
		{
			m_cards[m_cards.size()-1]->removeFromParent();
			m_cards.pop_back();
		}
	}
	reCards = m_queData.removeLastCardByNum(num);
	return reCards;
}

void UICardQueue::removeAllCards()
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards[i]->removeFromParent();
	}
	m_cards.clear();
	m_queData.clear();
}

CardSprite* UICardQueue::operator[](int index)
{
	if (index < 0 || index >= (int)(m_cards.size()))
	{
		return NULL;
	}
	return m_cards[index];
}


int UICardQueue::size()
{
	return (int)m_cards.size();
}


void UICardQueue::clear()
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards[i]->removeFromParent();
	}
	m_cards.clear();
	m_queData.clear();
}

CardSprite* UICardQueue::getTheLastCard()
{
	int sz = m_cards.size();
	ST_RETURN_NULL_IF(sz <= 0);
	return m_cards[sz - 1];
}

CardSprite* UICardQueue::getTheFirstCard()
{
	int sz = m_cards.size();
	ST_RETURN_NULL_IF(sz <= 0);
	return m_cards[0];
}

bool UICardQueue::isEmpty()
{
	int sz = m_cards.size();
	return (sz == 0);
}

bool UICardQueue::isAllCardsOpen()
{
	int sz = m_cards.size();
	if (sz == 1){ return true; }
	for (int i = 0; i < sz; i++)
	{
		if (!m_cards[i]->getCardInfo().m_isOpen)
		{
			return false;
		}
	}
	return true;
}

Vec2 UICardQueue::getWorldPosOfCardBg()
{
	return convertToWorldSpace(Vec2::ZERO);
}

void UICardQueue::updateCard()
{
	int sz = m_cards.size();
	for (int i = 0; i < sz; i++)
	{
		m_cards[i]->initCard();
	}
}


void UICardQueue::addTipLightAfterIndex(int idx)
{
	int sz = m_cards.size();
	ST_RETURN_IF(idx < 0 || idx >= sz);
	Vec2 pos = m_cards[idx]->getPosition();
	Size cardSize = CardSprite::getCardSize();
	float cardScale = CardSprite::getCardScale();

	if (!m_tipLight)
	{
		m_tipLight = Node::create();
		ST_RETURN_IF(!m_tipLight);
		this->addChild(m_tipLight, 100);
	}

	for (int i = idx; i < sz; i++)
	{
		Vec2 pos = m_cards[i]->getPosition();
		Sprite *edge = Sprite::create("Pic/UI_Card_Yellow_Light.png");
		ST_RETURN_IF(!edge);
		edge->setScale(cardScale*1.2);
		edge->setPosition(pos);
		//edge->setPosition(Vec2(0, -i*gapY));
		m_tipLight->addChild(edge,1);

		CardSprite *sp = CardSprite::create(m_cards[i]);
		ST_RETURN_IF(!sp);
		sp->setPosition(pos);
		m_tipLight->addChild(sp,2);

		CCFadeIn *fadeIn = CCFadeIn::create(1.0f);
		ST_RETURN_IF(!fadeIn);
		CCFadeOut *fadeO = CCFadeOut::create(1.0f);
		ST_RETURN_IF(!fadeO);
		Sequence *seq = Sequence::create(fadeIn, fadeO, NULL);
		ST_RETURN_IF(!seq);
		Repeat *repeat = Repeat::create(seq, 5);
		ST_RETURN_IF(!repeat);
		CallFunc *callFunc = CallFunc::create(this, callfunc_selector(UICardQueue::removeTipLight));
		ST_RETURN_IF(!callFunc);
		Sequence *sq = Sequence::create(repeat, callFunc, NULL);
		ST_RETURN_IF(!sq);
		if (i == sz - 1)
		{
			edge->runAction(sq);
		}
		else
		{
			edge->runAction(repeat);
		}
	}
}

void UICardQueue::removeSelf(Node *node)
{
	if (node)
	{
		node->removeFromParent();
	}
}

void UICardQueue::removeTipLight()
{
	if (m_tipLight)
	{
		m_tipLight->removeFromParent();
		m_tipLight = NULL;
	}
}
