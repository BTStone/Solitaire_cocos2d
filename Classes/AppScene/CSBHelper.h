#ifndef __ST_CSB_HELPER_H__
#define __ST_CSB_HELPER_H__

#include "STBase.h"

enum CSBChildShowType
{
	kChildShow,
	kChildHide,
	kChildNotExist,
};

class CSBHelper
{
public:
	typedef std::function<void(Ref*, Widget::TouchEventType)> stWidgetTouchCallback;

	static void setChildVisible(Node *rootNode, string name, bool isShow);
	static CSBChildShowType getChildVisible(Node *rootNode, string name);
	static void updateFntTextToLabel(Node *rootNode, string name, string text, float maxWidth = -1.0f, 
		const TextHAlignment& alignment = TextHAlignment::LEFT, Color3B color = Color3B(1, 1, 1), float fontScale = -1.0f, Color4B shadowCo = Color4B(1,1,1,1));

	static void registerButton(Node *rootNode, string btnName, const stWidgetTouchCallback& callback);
	static void registerCheckBox(Node *rootNode, string btnName, Ref* target, SEL_SelectedStateEvent selector);
	static void setCheckBoxSelected(Node *rootNode, string btnName, bool isSelect);
	static void loadImageViewTexture(Node *rootNode, string btnName, string filePath);
};


#endif
