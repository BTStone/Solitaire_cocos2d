#ifndef __ST_SOLITAIRE_BOTTOM_MENU_NODE_H__
#define __ST_SOLITAIRE_BOTTOM_MENU_NODE_H__

#include "STBase.h"

#define Play_Light_Tag 111
#define More_Game_Icon       "moreGameIconButton"

class BottomMenuNode : public Node
{
public:
	static BottomMenuNode* create(bool withEffect = true);
	virtual bool init(bool withEffect);

	void initRootCSBNode();
	void handleButton();

	void show();
	void hide();

	void switchShowAndHide();
	void setHandType(bool isLeft);

	void updateLanguage();
	void fixScreen();

	void addPlayEffect();          //当没有任何牌时play闪烁效果
	void removePlayEffect();                                    //删除play特效

	void tryToShowMoreGameIcon();
	void tryToShowDailyPointSign();
	void tryToShowPlayBoard(bool canReplay);
	bool tryToHidePlayBoard();
	void updateReplayState(bool canReplay);

protected:
	void btnMoreGameIconCallBack(Ref *pSender, Widget::TouchEventType type);

	void btnPlayCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnOptionCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnDailyCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnHintCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnUndoCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnHidePlayBoardCallBack(Ref *pSender, Widget::TouchEventType type);


	void btnNewGameCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnReplayCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnStatisticCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnScoreCallBack(Ref *pSender, Widget::TouchEventType type);


protected:
	//CC_SYNTHESIZE_READONLY(float, m_menuWidth, MenuWidth);//按钮的宽度
	//CC_SYNTHESIZE_READONLY(float, m_menuAreaHeight, MenuAreaHeight);//按钮的宽度
	//CC_SYNTHESIZE_READONLY(float, m_adsAreaHeight, AdsAreaHeight);//按钮的宽度
protected:
	//Scale9Sprite *m_menuBg; //各个菜单的背景
	//Sprite *m_arrow;        //指示箭头
	//Node *m_notArrowNode;   //非箭头的所有部分
	bool m_isShow;          //是否是展示状态
	bool m_withEffect;      //是否添加play闪烁效果,以及每日挑战按钮角标提示

	Node *m_rootCSBNode;    //csb
	Vec2 m_orginBottomBoardPos; //底部初始位置， 方便记录上下移动的终点
};

#endif
