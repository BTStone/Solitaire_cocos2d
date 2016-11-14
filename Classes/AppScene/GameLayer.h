#ifndef __ST_SOLITAIRE_GAME_LAYER_H__
#define __ST_SOLITAIRE_GAME_LAYER_H__

#include "UIHandCardArea.h"
#include "UIPlaceCardArea.h"
#include "UICollectCardArea.h"
#include "CardDataManager.h"
#include "TransportCardQueue.h"
#include "BottomMenuNode.h"
#include "TipManager.h"
#include "UndoManager.h"

#include "NetTime.h"

#define ST_PLAY_BOARD_TAG               1000                                      //playBoard��tag
#define Seperate_layer_tag              1111
#define Option_board_begin_tag          3000                                      //option board

#define Net_request_effect              3333
class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();
	ST_SCENE_FUNC(GameLayer);
	CREATE_FUNC(GameLayer);
	void onEnterGameLayer();                         //�ڽ�����ϷǰһЩ���ݴ���
	virtual bool init();
	void initHeadLabel();                           
	void initEachArea();
	void initGameBg();
	void initBottonMenu();
	void addAutoCollectMenu();
	void fixGameBg();
	void NotificationDailyChallenge();               //ֻҪ������Ϸ�����͵ڶ����ĳһʱ������ÿ����ս

	void changeFrameSizeByIsPortrait(bool isPort);   //���ݺ������޸�frameSize
	void setFrameAndDesignSizeWhenPortrait();
	void setFrameAndDesignSizeWhenLandscape();
	void setPortraitStatus(float dt = 0.2f);                        //����
	void setLandscapeStatus(float dt = 0.2f);                       //����
	void setLandscapeStatusDelay();                  //�M���ȴ�  
	void setPortraitStatusDelay();                   //�����ȴ�   
	void placeTransQue();

	void addTouchListerner();

	Vec2 getTransportEndPosition();                  //�����������Ŀ�ĵ�������±�ȷ��Ŀ������

    void stTest_shock();
	void stTest_chinese();
	void stTest_netTime();

	void useNewTheme();

	string convertToTimeStyle(int sec);              //������ת����ʱ���ʽ���ַ���
	void addDealCardEffect();                        //��ӷ���Ч��
	void refreshPlayBoardWhenSwitchScreen();         //���л�������ʱ��ˢ��playBoard

	bool AllAreaIsEmpty();                           //���е�����û����
	void resumeFrameAndDesignSize();

protected:
	void update(float dt);
	void updateTimeLabel();
	void updatePlayTime();
	void updateAutoTipTime();

	void refreshChallengeDate();                                    //ˢ����ս��Ϣ(������߷֣��Լ���������)

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	bool canAutoCollect();                                          //�ж��Ƿ�����Զ�ȫ���ƣ��������գ���������ȫ����
	void menuAutoCollectCallBack(Ref *pSender);
	void autoCollect();
	void autoCollectEndCallFuncN(Node *node);
	void autoCollectOverCallFunc();
	void flipCard(); 
	void dragEndWithPos(Vec2 endWorldPos);                          //�϶�������Ĳ���
	void autoMove();                                                //���϶�������Զ��ƶ�
	bool isMoveSuccess();                                           //�Ƿ��ƶ��ɹ���Ŀ�ĵ���ʼ����һ����false����һ����true
	bool isMoveEffectly();                                          //�Ƿ�����Ч�ƶ�
	int changeScoreWhenMoveSuccess();                               //���ƶ��ɹ�ʱ�����������ĸı�
	void changeScoreByUndoInfo(UndoInfo info);                      //����ʱ�����������ĸı�

	void transportMove(Vec2 beginWorldPos = Vec2::ZERO);            //�������ƶ���ָ��λ��
	void transportShockOfFailMove();                                //�����ƶ�ʧ�ܣ���Ч��
	void transportArrivedDestination();                             //�������˵�Ŀ�ĵ�֮��Ĳ��������������������ֽ�ƣ�
	bool resQueueActionWhenMoveSuccess();                           //���ƶ��ɹ�ʱԴ���еĲ������������Ƿ���ƣ����ƴ����Ƿ��ƶ��Ƶȣ�
	void resQueueActionByUndoInfo(UndoInfo info);                  

	void msgSwitchScreen(Ref *ref);                                 //�л�������
	void msgSwitchHandTypeCallBack(Ref *ref);                       //�л�������
	void msgSetTouchEnableCallBack(Ref *ref);                       //���ñ���Ϸ���Ƿ�ɵ��
	void msgShowPlayBoardCallBack(Ref *ref);                        //��ʾplayBoard
	void showPlayBoardWithOutEffect();
	void msgTipCallBack(Ref *ref);                                  //��ʾ
	void newTip(bool isManual = true);                              //��ʾ�� isManual�����Ƿ��ֶ���ʾ
	void newTipWithManual(TipInfo tipInfo);                         //�ֶ���ʾ
	void newTipWithAuto(TipInfo tipInfo);                           //�Զ���ʾ
	void msgUndoCallBack(Ref *ref);                                 //����
	void undo();
	void undoOverCallFunc(Node *node);
	void msgShowOptionBoardCallBack(Ref *ref);                      //��ʾ�������
	void msgShowOptionBoardWithOutEffectCallBack(Ref *ref);
	void msgNewGameCallBack(Ref *ref);                              //����Ϸ
	void msgDailyChallengeGameCallBack(Ref *ref);                   //ÿ����ս��Ϸ
	void msgReplayCallBack(Ref *ref);                               //���������Ϸ
	void msgUpdateUICallBack(Ref *ref);                             //ѡ��Ƥ���󣬸�����ʾ
	void msgConnectNetworkShowDailyChallengeCallBack(Ref *ref);     //������飬��ʾÿ����ս���
	void msgUpdateLanguageCallBack(Ref *ref);                       //��������
	void msgGameEnterBackGroundCallBack(Ref *ref);                  //��Ϸ�����̨
	void msgExitGameCallBack(Ref *ref);                             //�˳���Ϸ
	void msgClickAndroidReturnButtonCallBack(Ref *ref);             //������ؼ��ص�Android
	void msgChangeToNewThemeCallBack(Ref *ref);
	void msgShowNewThemeBoardCallBack(Ref *ref);
	void clickAndroidReturnButtonDelayCallFunc();

	void tryToPreGameAd(int tp);
	void tryToShowGameAd(int tp);

	void onNetSuccessComplete(NetTime * pSender);
	void onNetFailComplete(NetTime * pSender);
	void updateCard();

	void resetGame();                                               //����UI��������������

	void dealCardOverCallFunc();                                    //����Ч������
	void tipOverCallFunc(Node *node);                               //��ʾЧ������
	void boardShowEndCallFunc(Node *node);                          //���ص�show֮��
	void addNewThemeTipBoardDailyCallFunc();
	void tryToShowGameAdCallFunc();

	vector<Vec2> getDealPositions();                                //��ȡ����ʱ�����Ƶ���������

	void bakAllCards();                                             //�洢����ֽ��

	bool isWin();                                                   //�ж��Ƿ��Ѿ�ȫ��
	void win();                                                     //52����ȫ�պ�Ĳ��� 
	int getIndexOfTheLastNumCardOfQue(CardArea area, int queIdx, int num);              //��ȡĳһ���е����ڼ��������ڵ��±�
	Vec2 getCardWorldPosition(CardArea area, int queIdx, int cardIdx = 0);              //��ȡĳһ�±����Ƶ���������
	Vec2 getNextPostionOfAreaOfQueue(CardArea area, int queIdx);                        //��ȡĳһ������һ�������Ƶ���������
	vector<Card> removeTheLastNumCards(CardArea area, int queIdx, int num);             //ɾ��ĳһ���е��������

	void stTest_win();
	int getBestScoreOfToday();

	void addSeparateTouch();
	void removeSeparateTouch();

	void tryToShowInvitePlayDailyBoard();
	bool tryToRemoveOptionBoard();
	bool tryToRemovePlayBoard();

	void reportNewGameStartLevel();
	void reportDailyChallengeStartLevel();
	void reportEndLevel();
	void reportNewGameEndLevel();
	void reportDailyChallengeEndLevel();


protected:
	Sprite *m_bg;
	UIHandCardArea *m_uiHandArea;
	UIPlaceCardArea *m_uiPlaceArea;
	UICollectCardArea *m_uiCollectArea; 
	CardDataManager m_cardDataManager;
	bool m_touchAble;                                 //�жϱ����Ƿ�ɵ���ı�ʶ���������ƶ���ʱ������ʱ���ɵ��
	Vec2 m_touchBeganPoint;                           //onTouchBegan�ĳ�ʼ���λ��
	Vec2 m_touchMovePoint;                            //onTouchMoved�Ĺ����е����λ��
	bool m_isDrag;                                    //�ж��Ƿ����϶��ƣ�true������϶���λ���жϣ�false�������ƶ�
	TransportCardQueue *m_transportQue;               //�������

	int m_score;                                      //����
	int m_time;                                       //��ʱ(��)
	int m_moveTimes;                                  //����
	STLabelBMFont *m_scoreLabel;
	STLabelBMFont *m_timeLabel;
	STLabelBMFont *m_moveLabel;
	bool m_canTime;                                   //�Ƿ���Լ�ʱ
	BottomMenuNode *m_bottomMenu;                     //�ײ��˵�
	vector<CardSprite*> m_dealCards;                  //����Ч���õ�����
	TipManager *m_tipManager;                         //��ʾ
	bool m_tipping;                                   //�Ƿ�������ʾ
	int m_autoTipTime;                                //�Զ���
	Menu *m_autoCollectMenu;
	bool m_isLeftHandType;                            //�Ƿ�������ģʽ
	bool m_canClickReturnBt;                          //��ֹ�ܿ������ֶ�����
	bool m_isAutoCollecting;                          //�Ƿ������Զ�����

public:
	static bool m_isStoreCardLibMode;                 //�Ƿ��Ǵ洢���ƿ��ģʽ

};



#endif
