#include "STHelper.h"


void STHelper::separateTouch(Node * parent)
{
	// 为规避在多点触碰场景下，无法隔绝完，这里生成5个隔绝层
	int menuNum = 5;
	for (int i = 0; i < menuNum; i++)
	{
		// 隔绝当前面板弹出前，所有节点的交互
		Node * spaceNode = Node::create();
		ST_RETURN_IF(!spaceNode);
		spaceNode->setContentSize(CCSizeMake(CLDWinSize().width * 3, CLDWinSize().height * 3));
		MenuItemSprite * menuSeparate = MenuItemSprite::create(
			spaceNode,
			spaceNode);
		ST_RETURN_IF(!menuSeparate);

		Menu * pMenu = Menu::createWithItem(menuSeparate);
		ST_RETURN_IF(!pMenu);
		pMenu->setPosition(Vec2::ZERO);
		parent->addChild(pMenu);
	}
	FileUtils::getInstance()->getWritablePath();
}

void STHelper::addDarkLayer(Node *board)
{
	Size winSize = CLDWinSize();
	//UIFullScreenFix(fixWinSize); 
	float bet = 6.0f;
	LayerColor * under = LayerColor::create(ccc4(0, 0, 0, 135), winSize.width*bet, winSize.height*bet);
	ST_RETURN_IF(!under);
	under->setPosition(ccp(-winSize.width*bet / 2.0, -winSize.height*bet / 2.0));
	board->addChild(under,-1);
}

string STHelper::getDocPath()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//D:/installpakage/sdk/cocos2d-x-3.7.1/cocos2d-x-3.7.1/projects/solitaire_cocos2dx
	return "D:/installpakage/sdk/cocos2d-x-3.7.1/cocos2d-x-3.7.1/projects/Solitaire/Documents/";
#else
	return FileUtils::getInstance()->getWritablePath();
#endif
}