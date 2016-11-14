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

	void addPlayEffect();          //��û���κ���ʱplay��˸Ч��
	void removePlayEffect();                                    //ɾ��play��Ч

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
	//CC_SYNTHESIZE_READONLY(float, m_menuWidth, MenuWidth);//��ť�Ŀ��
	//CC_SYNTHESIZE_READONLY(float, m_menuAreaHeight, MenuAreaHeight);//��ť�Ŀ��
	//CC_SYNTHESIZE_READONLY(float, m_adsAreaHeight, AdsAreaHeight);//��ť�Ŀ��
protected:
	//Scale9Sprite *m_menuBg; //�����˵��ı���
	//Sprite *m_arrow;        //ָʾ��ͷ
	//Node *m_notArrowNode;   //�Ǽ�ͷ�����в���
	bool m_isShow;          //�Ƿ���չʾ״̬
	bool m_withEffect;      //�Ƿ����play��˸Ч��,�Լ�ÿ����ս��ť�Ǳ���ʾ

	Node *m_rootCSBNode;    //csb
	Vec2 m_orginBottomBoardPos; //�ײ���ʼλ�ã� �����¼�����ƶ����յ�
};

#endif
