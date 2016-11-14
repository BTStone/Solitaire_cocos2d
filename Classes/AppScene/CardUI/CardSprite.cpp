#include "CardSprite.h"
#include "SkinManager.h"
#include "ScreenDirectionManager.h"

float CardSprite::m_scale = 1.0f;
Size CardSprite::m_cardSize = Size::ZERO;

void CardSprite::setCardScaleAndSize()
{
	float sale = 0.9f;   //Ö½ÅÆÍ¼Æ¬ÓÐ¿Õ°×Çø£¬·Ç¿Õ°×ÇøÕ¼µÄ±ÈÀý
	Sprite *sp = Sprite::create("Pic/Card-BG-Default.png");
	ST_RETURN_IF(!sp);
	Size spSize = sp->getContentSize();

	Size winSize = CLDWinSize();
	float minWidth = min(winSize.width, winSize.height);
	float maxHeight = max(winSize.width, winSize.height);

	float relativeScale = 8.0f / 12.5f;         //¶Ô±È³¤¿í±È
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	float frameWidth = min(frameSize.width, frameSize.height);
	float frameHeight = max(frameSize.width, frameSize.height);
	float frameScale = frameWidth / frameHeight;

	if (frameScale > relativeScale)
	{
		//ÔÝÊ±ÆÁ±ÎipadÆÁÄ»ÊÊÅä
		minWidth = maxHeight * relativeScale;      
	}
	
	//ÊúÆÁ
	m_scale = (minWidth - 8 * Place_Area_Gap_X) / Place_Area_Num / (spSize.width*sale);
	m_cardSize = spSize * m_scale * sale;
}

CardSprite* CardSprite::create(Card card)
{
	CardSprite *re = new CardSprite();
	if (re && re->init(card))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

CardSprite* CardSprite::create(CardSprite *cardSprite)
{
	CardSprite *re = new CardSprite();
	if (re && re->init(cardSprite))
	{
		re->autorelease();
		return re;
	}
	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool CardSprite::init(Card card)
{
	if (!Node::init())
	{
		return false;
	}
	m_cardInfo = card;
	m_cardBg = NULL;
	m_cardFace = NULL;
	//m_tip = NULL;

	initCard();
	this->setScale(m_scale);
	return true;
}

bool CardSprite::init(CardSprite *cardSprite)
{
	if (!Node::init())
	{
		return false;
	}
	m_cardInfo = cardSprite->getCardInfo();
	m_cardBg = NULL;
	m_cardFace = NULL;
	//m_tip = NULL;

	initCard();
	this->setScale(m_scale);
	return true;
}

void CardSprite::initCard()
{
	initCardBg();
	if (m_cardInfo.m_isOpen)
	{
		initCardFace();
	}
}

void CardSprite::initCardBg()
{
	if (m_cardBg)
	{
		m_cardBg->removeFromParent();
		m_cardBg = NULL;
	}
	if (m_cardInfo.m_isOpen)
	{
		m_cardBg = Sprite::create("Pic/Card-BG-Default.png");
	}
	else
	{
		int cardBackID = SkinManager::share()->getCardBackID();
		string frameName = SkinManager::share()->getCardBgFileNameByCardBgId(cardBackID);
		m_cardBg = Sprite::create(frameName);
	}
	ST_RETURN_IF(!m_cardBg);
	//m_cardBg->setAnchorPoint(Vec2(0.5f, 0.58f));
	m_cardBg->setPosition(Vec2::ZERO);
	this->addChild(m_cardBg, 0);
}

void CardSprite::initCardFace()
{
	if (m_cardFace)
	{
		m_cardFace->removeFromParent();
		m_cardFace = NULL;
	}
	m_cardFace = Node::create();
	ST_RETURN_IF(!m_cardFace);
	m_cardFace->setPosition(Vec2::ZERO);
	this->addChild(m_cardFace, 1);

	Size bgSize = m_cardBg->getContentSize();
	int cardFaceID = SkinManager::share()->getCardFaceID();
	string numFrameName = getNumFrameNameByID(cardFaceID);
	string smallColorFrameName = getSmallColorFrameNameByID(cardFaceID);
	string bigColorFrameName = getBigColorFrameNameByID(cardFaceID);

	Sprite *numSp = Sprite::create(numFrameName.c_str());
	ST_RETURN_IF(!numSp);
	numSp->setScale(0.9f);
	numSp->setPosition(Vec2(-bgSize.width*0.19, bgSize.height*0.29));
	m_cardFace->addChild(numSp, 1);

	Sprite *smallSp = Sprite::create(smallColorFrameName.c_str());
	ST_RETURN_IF(!smallSp);
	smallSp->setScale(0.55f);
	smallSp->setPosition(Vec2(bgSize.width*0.19, bgSize.height*0.29));
	m_cardFace->addChild(smallSp, 1);

	Sprite *bigSp = Sprite::create(bigColorFrameName.c_str());
	ST_RETURN_IF(!bigSp);
	bigSp->setPosition(Vec2(bgSize.width*0, -bgSize.height*0.15));
	m_cardFace->addChild(bigSp, 1);

	if (cardFaceID == 2)
	{
		numSp->setPosition(Vec2(-bgSize.width*0.2, bgSize.height*0.32));
		numSp->setScale(0.8f);
		smallSp->setScale(0.5f);
		bigSp->setPosition(Vec2(bgSize.width * 0, -bgSize.height*0.12));
	}
}

string CardSprite::getNumFrameNameByID(int faceID)
{
	string re = "cocostudio/ui/Card/Front/";
	if (m_cardInfo.m_color == BLACK_CARD || m_cardInfo.m_color == FLOWER_CARD)
	{
		re += "Card-Black-Num-";
	}
	else
	{
		re += "Card-Red-Num-";
	}
	char buf[32];
	sprintf(buf, "%02d", m_cardInfo.m_number);
	re += buf;
	if (faceID > 1)
	{
		sprintf(buf, "-s%d", faceID);
		re += buf;
	}
	re += ".png";
	return re;
}

string CardSprite::getSmallColorFrameNameByID(int faceID)
{
	string re = "Pic/";
	switch (m_cardInfo.m_color)
	{
	case BLACK_CARD:
		re += "Card-Color-Spades";
		break;
	case RED_CARD:
		re += "Card-Color-Hearts";
		break;
	case FLOWER_CARD:
		re += "Card-Color-Clubs";
		break;
	case SQUARE_CARD:
		re += "Card-Color-Diamonds";
		break;
	default:
		break;
	}
	char buf[32];
	if (faceID > 1)
	{
		sprintf(buf, "-s%d", faceID);
		re += buf;
	}
	re += ".png";
	return re;
}

string CardSprite::getBigColorFrameNameByID(int faceID)
{
	if (m_cardInfo.m_number <= 10)
	{
		return getSmallColorFrameNameByID(faceID);
	}
	const char *jqkName[] = { "Jack", "Queen", "King" };                   //J¡¢Q¡¢K
	const char *colorName[] = { "Spades", "Hearts", "Clubs", "Diamonds" }; //ºÚ¡¢ºì¡¢»¨¡¢¿é
	string re = "cocostudio/ui/Card/Front/Card-Texture-";
	re += jqkName[m_cardInfo.m_number - 11];
	re += colorName[m_cardInfo.m_color];
	if (faceID > 1)
	{
		char buf[32];
		sprintf(buf, "-s%d", faceID);
		re += buf;
	}
	re += ".png";
	return re;
}
//
//Size CardSprite::getContentSize()
//{
//	if (m_cardBg)
//	{
//		return m_cardSize;
//	}
//	return Size::ZERO;
//}

float CardSprite::getCardScale()
{
	return m_scale;
}

Size CardSprite::getCardSize()
{
	return m_cardSize;
}

bool CardSprite::containThePos(Vec2 worldPos)
{
	Size realSize = m_cardBg->getContentSize()*0.9;
	Vec2 originPos = Vec2(-realSize.width / 2.0, -realSize.height / 2.0);
	Rect rect = Rect(originPos, realSize);
	Vec2 pos = convertToNodeSpace(worldPos);
	return rect.containsPoint(pos);
	Vec2 posOr = Vec2(pos.x - realSize.width / 2.0, pos.y - realSize.height / 2.0);
	Rect rt = Rect(posOr, realSize);
	bool bl1 = rect.intersectsRect(rt);
	bool bl2 = (pos.x >= originPos.x && pos.x <= (originPos.x + realSize.width));
	return bl1 && bl2;
}

void CardSprite::open()
{
	ST_RETURN_IF(m_cardInfo.m_isOpen);
	float openTm = 0.1f;
	ScaleTo *scale1 = ScaleTo::create(openTm, 0, m_scale);
	ST_RETURN_IF(!scale1);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(CardSprite::showOpenStatus));
	ST_RETURN_IF(!callFunc);
	ScaleTo *scale2 = ScaleTo::create(openTm, m_scale, m_scale);
	ST_RETURN_IF(!scale2);
	Sequence *seq = Sequence::create(scale1, callFunc, scale2, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
}


void CardSprite::close()
{
	ST_RETURN_IF(!m_cardInfo.m_isOpen);
	float openTm = Move_And_Untouchable_Time / 2.0;
	ScaleTo *scale1 = ScaleTo::create(openTm, 0, m_scale);
	ST_RETURN_IF(!scale1);
	CallFunc *callFunc = CallFunc::create(this, callfunc_selector(CardSprite::showClosedStatus));
	ST_RETURN_IF(!callFunc);
	ScaleTo *scale2 = ScaleTo::create(openTm, m_scale, m_scale);
	ST_RETURN_IF(!scale2);
	Sequence *seq = Sequence::create(scale1, callFunc, scale2, NULL);
	ST_RETURN_IF(!seq);
	this->runAction(seq);
}


void CardSprite::showClosedStatus()
{
	m_cardInfo.m_isOpen = false;
	if (m_cardFace)
	{
		m_cardFace->removeFromParent();
		m_cardFace = NULL;
	}
	
	initCard();
}

void CardSprite::showOpenStatus()
{
	m_cardInfo.m_isOpen = true;
	initCard();
}
