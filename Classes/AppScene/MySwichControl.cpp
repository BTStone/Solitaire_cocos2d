#include "MySwichControl.h"

MySwitchControl* MySwitchControl::create(bool isOpen, Node *bgSprite, Node *leftSprite, Node *rightSprite, Label *leftLabel /* = NULL */, Label *rightLabel /* = NULL */)
{
	MySwitchControl *re = new MySwitchControl();
	if (re && re->init(isOpen, bgSprite, leftSprite, rightSprite, leftLabel, rightLabel))
	{
		re->autorelease();
		return re;
	}

	CC_SAFE_DELETE(re);
	re = NULL;
	return re;
}

bool MySwitchControl::init(bool isOpen, Node *bgSprite, Node *leftSprite, Node *rightSprite, Label *leftLabel, Label *rightLabel)
{
	if (!Node::init())
	{
		return false;
	}
	m_menuItem = MenuItemSprite::create(bgSprite, NULL);
	ST_RETURN_FALSE_IF(!m_menuItem);
	Menu *menu = Menu::createWithItem(m_menuItem);
	ST_RETURN_FALSE_IF(!menu);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,0);
	m_bgSize = bgSprite->getContentSize();
	m_leftSprite = leftSprite;
	m_rightSprite = rightSprite;
	initSpriteAndLabel(leftLabel,rightLabel);
	setStatus(isOpen);
	return true;
}

void MySwitchControl::initSpriteAndLabel(Label *leftLabel, Label *rightLabel)
{
	if (m_leftSprite)
	{
		m_leftSprite->setPosition(Vec2(-m_bgSize.width*0.25, 0));
		this->addChild(m_leftSprite, 1);
	}
	if (m_rightSprite)
	{
		m_rightSprite->setPosition(Vec2(m_bgSize.width*0.25, 0));
		this->addChild(m_rightSprite, 1);
	}
	if (leftLabel)
	{
		leftLabel->setPosition(Vec2(-m_bgSize.width*0.25, 0));
		this->addChild(leftLabel, 1);
		leftLabel->setVisible(false);
	}
	if (rightLabel)
	{
		rightLabel->setPosition(Vec2(m_bgSize.width*0.25, 0));
		this->addChild(rightLabel, 1);
		rightLabel->setVisible(false);
	}
}

void MySwitchControl::setStatus(bool isOpen)
{
	if (m_leftSprite)
	{
		m_leftSprite->setVisible(isOpen);
	}
	if (m_rightSprite)
	{
		m_rightSprite->setVisible(!isOpen);
	}
}

void MySwitchControl::setTarget(Ref *rec, SEL_MenuHandler selector)
{
	m_menuItem->setTarget(rec, selector);
}