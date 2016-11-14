#ifndef __ST_SOLITAIRE_MY_SWITCH_CONTROL_H__
#define __ST_SOLITAIRE_MY_SWITCH_CONTROL_H__

#include "STBase.h"

class MySwitchControl : public Node
{
public:
	static MySwitchControl *create(bool isOpen, Node *bgSprite, Node *leftSprite,
		Node *rightSprite, Label *leftLabel = NULL, Label *rightLabel = NULL);
	virtual bool init(bool isOpen, Node *bgSprite, Node *leftSprite, 
		Node *rightSprite, Label *leftLabel, Label *rightLabel);
	void initSpriteAndLabel(Label *leftLabel, Label *rightLabel);
	void setStatus(bool isOpen);                        //左边为打开isOpen为true，右边为false
	void setTarget(Ref *rec, SEL_MenuHandler selector);
protected:
	MenuItemSprite *m_menuItem;
	Size m_bgSize;
	Node *m_leftSprite;
	Node *m_rightSprite;
};

#endif
