#include "UICollectCardQueue.h"

UICollectCardQueue* UICollectCardQueue::create(CardQueue queData)
{
	UICollectCardQueue *re = new UICollectCardQueue();
	if (re && re->init(queData))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool UICollectCardQueue::init(CardQueue queData)
{
	if (!UICardQueue::init(queData))
	{
		return false;
	}
	m_resArea = COLLECT_AREA;
	initCardsPosition();
	return true;
}

void UICollectCardQueue::initCardsPosition()
{

}


void UICollectCardQueue::addCards(CardQueue queData, bool withEffect)
{
	int sz = queData.size();
	ST_RETURN_IF(sz <= 0);
	m_queData.pushBack(queData);
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
		if (withEffect)
		{
			addCollectCardEffect(card.m_color);
		}
		
	}
}


TransportCardQueue* UICollectCardQueue::search(Vec2 worldPos)
{
	bool isContain = containThePos(worldPos);
	ST_RETURN_NULL_IF(!isContain);
	Card card = removeTheLastCard();
	ST_RETURN_NULL_IF(card.isInvalidCard());
	vector<Card> cards;
	cards.push_back(card);
	TransportCardQueue *re = TransportCardQueue::create(cards, m_resArea);
	ST_RETURN_NULL_IF(!re);
	Vec2 pos = convertToWorldSpace(Vec2::ZERO);
	re->setPosition(pos);
	return re;
}

bool UICollectCardQueue::intersectsTheCard(Vec2 worldCardPos)
{
	Size cardSize = CardSprite::getCardSize();
	Vec2 originPos = Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rect = Rect(originPos, cardSize);
	Vec2 pos = convertToNodeSpace(worldCardPos);
	Vec2 posOr = Vec2(pos.x - cardSize.width / 2.0, pos.y - cardSize.height / 2.0);
	Rect rt = Rect(posOr, cardSize);
	bool bl1 = rect.intersectsRect(rt);
	bool bl2 = (pos.x >= originPos.x && pos.x <= (originPos.x + cardSize.width));
	return bl1 && bl2;
}

bool UICollectCardQueue::containThePos(Vec2 worldPos)
{
	Size cardSize = CardSprite::getCardSize();
	Vec2 originPos = Vec2(-cardSize.width / 2.0, -cardSize.height / 2.0);
	Rect rect = Rect(originPos, cardSize);
	Vec2 pos = convertToNodeSpace(worldPos);
	return rect.containsPoint(pos);
	Vec2 posOr = Vec2(pos.x - cardSize.width / 2.0, pos.y - cardSize.height / 2.0);
	Rect rt = Rect(posOr, cardSize);
	bool bl1 = rect.intersectsRect(rt);
	bool bl2 = (pos.x >= originPos.x && pos.x <= (originPos.x + cardSize.width));
	return bl1 && bl2;
}

bool UICollectCardQueue::canCollectTheCard(Card card)
{
	return m_queData.canCollectTheCard(card);
}

void UICollectCardQueue::collectTheCard(Card card)
{
	m_queData.pushBack(card);
	CardSprite *cardSp = CardSprite::create(card);
	ST_RETURN_IF(!cardSp);
	cardSp->setPosition(Vec2::ZERO);
	this->addChild(cardSp);
	m_cards.push_back(cardSp);
}


Vec2 UICollectCardQueue::getNextPosition()
{
	return getWorldPosOfCardBg();
}

void UICollectCardQueue::addCollectCardEffect(CardColor color)
{
	string picName = "";
	if (color == BLACK_CARD)
	{
		picName = "Pic/Card-Color-Spades.plist";
	}
	else if (color == RED_CARD)
	{
		picName = "Pic/Card-Color-Hearts.plist";
	}
	else if (color == FLOWER_CARD)
	{
		picName = "Pic/Card-Color-Clubs.plist";
	}
	else
	{
		picName = "Pic/Card-Color-Diamonds.plist";
	}
	ParticleSystemQuad *particleSystem = ParticleSystemQuad::create(picName);
	//particleSystem->setOpacity(1.0f);
	//particleSystem->setPositionType(kCCPositionTypeGrouped);   // 粒子效果成为一个整体]
	/*
	Size picSize = Size(80, 90);
	ParticleSystem* particleSystem = ParticleExplosion::create();
	particleSystem->setTexture(TextureCache::getInstance()->addImage(picName.c_str()));
	//particleSystem->setDuration(2.0f);
	particleSystem->setLife(0.5f);
	particleSystem->setLifeVar(0.05f);
	particleSystem->setStartSize(picSize.height*0.3);
	particleSystem->setStartSizeVar(picSize.height*0.1);
	particleSystem->setTotalParticles(200);
	particleSystem->setSpeed(200);
	particleSystem->setSpeedVar(20);*/
	particleSystem->setPosition(Vec2::ZERO);
	particleSystem->setAutoRemoveOnFinish(true);
	this->addChild(particleSystem,100);
}
