#include "CSBHelper.h"
#include"cocostudio/CocoStudio.h"

void CSBHelper::setChildVisible(Node *rootNode, string name, bool isShow)
{
	ST_RETURN_IF(!rootNode);
	Node * child = ui::Helper::seekNodeByName(rootNode, name);
	if (child)
	{
		child->setVisible(isShow);
	}
}

CSBChildShowType CSBHelper::getChildVisible(Node *rootNode, string name)
{
	if (!rootNode)
	{
		return kChildNotExist;
	}
	Node * child = ui::Helper::seekNodeByName(rootNode, name);
	if (child)
	{
		if (child->isVisible())
		{
			return kChildShow;
		}
		return kChildHide;
	}
	return kChildNotExist;
}


void CSBHelper::updateFntTextToLabel(Node *rootNode, string name, string text, float maxWidth, const TextHAlignment& alignment, Color3B color, float fontScale, Color4B shadowCo)
{
	ST_RETURN_IF(!rootNode);
	Node * child = ui::Helper::seekNodeByName(rootNode, name);
	TextBMFont *textFnt = dynamic_cast<TextBMFont*>(child);
	ST_RETURN_IF(!textFnt);
	float scaleX = textFnt->getScaleX();
	float scaleY = textFnt->getScaleY();
	float realFontScale = min(scaleX, scaleY);
	Color3B realColor = textFnt->getColor();
	if (!(color.r == 1 && color.b == 1 && color.g == 1))
	{
		realColor = color;
	}
	if (fontScale >= 0)
	{
		realFontScale = fontScale;
	}

	Vec2 anchPos = textFnt->getAnchorPoint();
	textFnt->setVisible(false);

	string inGameName = name +  "_in_game";
	Node *inGameNode = textFnt->getParent()->getChildByName(inGameName);
	ST_SAFE_REMOVE_FROM_PARENT(inGameNode);

	STLabelBMFont *lab = STLabelBMFont::createWithBMFont("EurostileBold.fnt", text);
	ST_RETURN_IF(!lab);
	lab->setScale(realFontScale);
	if (maxWidth > 0)
	{
		lab->setMaxLineWidth(maxWidth / realFontScale);
	}
	if (!(shadowCo.r == 1 && shadowCo.b == 1 && shadowCo.g == 1 && shadowCo.a == 1))
	{
		lab->enableShadow(shadowCo);
	}
	lab->setAlignment(alignment);
	lab->setAnchorPoint(anchPos);
	lab->setColor(realColor);
	lab->setPosition(textFnt->getPosition());
	lab->setName(inGameName);
	
	textFnt->getParent()->addChild(lab, textFnt->getLocalZOrder());
}

void CSBHelper::registerButton(Node *rootNode, string btnName, const stWidgetTouchCallback& callback)
{
	ST_RETURN_IF(!rootNode);
	Node * child = ui::Helper::seekNodeByName(rootNode, btnName);
	Button *button = dynamic_cast<Button*>(child);
	if (button)
	{
		button->addTouchEventListener(callback);
	}
}

void CSBHelper::registerCheckBox(Node *rootNode, string btnName, Ref* target, SEL_SelectedStateEvent selector)
{
	ST_RETURN_IF(!rootNode);
	Node * child = ui::Helper::seekNodeByName(rootNode, btnName);
	CheckBox *box = dynamic_cast<CheckBox*>(child);
	if (box)
	{
		box->addEventListenerCheckBox(target, selector);
	}
}

void CSBHelper::setCheckBoxSelected(Node *rootNode, string btnName, bool isSelect)
{
	ST_RETURN_IF(!rootNode);
	Node * child = ui::Helper::seekNodeByName(rootNode, btnName);
	CheckBox *box = dynamic_cast<CheckBox*>(child);
	if (box)
	{
		box->setSelected(isSelect);
	}
}

void CSBHelper::loadImageViewTexture(Node *rootNode, string name, string filePath)
{
	Node *node = ui::Helper::seekNodeByName(rootNode, name);
	ImageView *image = dynamic_cast<ImageView*>(node);
	if (image)
	{
		image->loadTexture(filePath);
	}
}