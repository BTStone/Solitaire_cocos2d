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

#define ST_PLAY_BOARD_TAG               1000                                      //playBoard的tag
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
	void onEnterGameLayer();                         //在进入游戏前一些数据处理
	virtual bool init();
	void initHeadLabel();                           
	void initEachArea();
	void initGameBg();
	void initBottonMenu();
	void addAutoCollectMenu();
	void fixGameBg();
	void NotificationDailyChallenge();               //只要进入游戏就推送第二天的某一时刻推送每日挑战

	void changeFrameSizeByIsPortrait(bool isPort);   //根据横竖屏修改frameSize
	void setFrameAndDesignSizeWhenPortrait();
	void setFrameAndDesignSizeWhenLandscape();
	void setPortraitStatus(float dt = 0.2f);                        //竖屏
	void setLandscapeStatus(float dt = 0.2f);                       //横屏
	void setLandscapeStatusDelay();                  //橫屏等待  
	void setPortraitStatusDelay();                   //竖屏等待   
	void placeTransQue();

	void addTouchListerner();

	Vec2 getTransportEndPosition();                  //根据运输类的目的地区域和下标确定目的坐标

    void stTest_shock();
	void stTest_chinese();
	void stTest_netTime();

	void useNewTheme();

	string convertToTimeStyle(int sec);              //将秒数转换成时间格式的字符串
	void addDealCardEffect();                        //添加发牌效果
	void refreshPlayBoardWhenSwitchScreen();         //当切换横竖屏时候刷新playBoard

	bool AllAreaIsEmpty();                           //所有的区域都没有牌
	void resumeFrameAndDesignSize();

protected:
	void update(float dt);
	void updateTimeLabel();
	void updatePlayTime();
	void updateAutoTipTime();

	void refreshChallengeDate();                                    //刷新挑战信息(当月最高分，以及所在日期)

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	bool canAutoCollect();                                          //判断是否可以自动全收牌（手牌区空，置牌区牌全开）
	void menuAutoCollectCallBack(Ref *pSender);
	void autoCollect();
	void autoCollectEndCallFuncN(Node *node);
	void autoCollectOverCallFunc();
	void flipCard(); 
	void dragEndWithPos(Vec2 endWorldPos);                          //拖动结束后的操作
	void autoMove();                                                //非拖动，点击自动移动
	bool isMoveSuccess();                                           //是否移动成功，目的地与始发地一样则false，不一样则true
	bool isMoveEffectly();                                          //是否是有效移动
	int changeScoreWhenMoveSuccess();                               //当移动成功时，监听分数的改变
	void changeScoreByUndoInfo(UndoInfo info);                      //回退时，监听分数的改变

	void transportMove(Vec2 beginWorldPos = Vec2::ZERO);            //运输类移动到指定位置
	void transportShockOfFailMove();                                //由于移动失败，振动效果
	void transportArrivedDestination();                             //运输类运到目的地之后的操作，（根据运输类添加纸牌）
	bool resQueueActionWhenMoveSuccess();                           //当移动成功时源队列的操作（置牌区是否打开牌，收牌打开区是否移动牌等）
	void resQueueActionByUndoInfo(UndoInfo info);                  

	void msgSwitchScreen(Ref *ref);                                 //切换横竖屏
	void msgSwitchHandTypeCallBack(Ref *ref);                       //切换左右手
	void msgSetTouchEnableCallBack(Ref *ref);                       //设置本游戏层是否可点击
	void msgShowPlayBoardCallBack(Ref *ref);                        //显示playBoard
	void showPlayBoardWithOutEffect();
	void msgTipCallBack(Ref *ref);                                  //提示
	void newTip(bool isManual = true);                              //提示， isManual代表是否手动提示
	void newTipWithManual(TipInfo tipInfo);                         //手动提示
	void newTipWithAuto(TipInfo tipInfo);                           //自动提示
	void msgUndoCallBack(Ref *ref);                                 //回退
	void undo();
	void undoOverCallFunc(Node *node);
	void msgShowOptionBoardCallBack(Ref *ref);                      //显示设置面板
	void msgShowOptionBoardWithOutEffectCallBack(Ref *ref);
	void msgNewGameCallBack(Ref *ref);                              //新游戏
	void msgDailyChallengeGameCallBack(Ref *ref);                   //每日挑战游戏
	void msgReplayCallBack(Ref *ref);                               //重新这局游戏
	void msgUpdateUICallBack(Ref *ref);                             //选择皮肤后，更新显示
	void msgConnectNetworkShowDailyChallengeCallBack(Ref *ref);     //联网检查，显示每日挑战面板
	void msgUpdateLanguageCallBack(Ref *ref);                       //更新语言
	void msgGameEnterBackGroundCallBack(Ref *ref);                  //游戏进入后台
	void msgExitGameCallBack(Ref *ref);                             //退出游戏
	void msgClickAndroidReturnButtonCallBack(Ref *ref);             //点击返回键回调Android
	void msgChangeToNewThemeCallBack(Ref *ref);
	void msgShowNewThemeBoardCallBack(Ref *ref);
	void clickAndroidReturnButtonDelayCallFunc();

	void tryToPreGameAd(int tp);
	void tryToShowGameAd(int tp);

	void onNetSuccessComplete(NetTime * pSender);
	void onNetFailComplete(NetTime * pSender);
	void updateCard();

	void resetGame();                                               //清理UI，并且重置数据

	void dealCardOverCallFunc();                                    //发牌效果结束
	void tipOverCallFunc(Node *node);                               //提示效果结束
	void boardShowEndCallFunc(Node *node);                          //面板回弹show之后
	void addNewThemeTipBoardDailyCallFunc();
	void tryToShowGameAdCallFunc();

	vector<Vec2> getDealPositions();                                //获取发牌时所有牌的最终坐标

	void bakAllCards();                                             //存储备份纸牌

	bool isWin();                                                   //判断是否已经全收
	void win();                                                     //52张牌全收后的操作 
	int getIndexOfTheLastNumCardOfQue(CardArea area, int queIdx, int num);              //获取某一队列倒数第几张牌所在的下标
	Vec2 getCardWorldPosition(CardArea area, int queIdx, int cardIdx = 0);              //获取某一下标下牌的世界坐标
	Vec2 getNextPostionOfAreaOfQueue(CardArea area, int queIdx);                        //获取某一队列下一个放置牌的世界坐标
	vector<Card> removeTheLastNumCards(CardArea area, int queIdx, int num);             //删除某一队列的最后几张牌

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
	bool m_touchAble;                                 //判断本层是否可点击的标识符，例如移动牌时，翻牌时不可点击
	Vec2 m_touchBeganPoint;                           //onTouchBegan的初始点击位置
	Vec2 m_touchMovePoint;                            //onTouchMoved的过程中点击的位置
	bool m_isDrag;                                    //判断是否是拖动牌，true则根据拖动的位置判断，false则智能移动
	TransportCardQueue *m_transportQue;               //运输队列

	int m_score;                                      //分数
	int m_time;                                       //用时(秒)
	int m_moveTimes;                                  //步数
	STLabelBMFont *m_scoreLabel;
	STLabelBMFont *m_timeLabel;
	STLabelBMFont *m_moveLabel;
	bool m_canTime;                                   //是否可以计时
	BottomMenuNode *m_bottomMenu;                     //底部菜单
	vector<CardSprite*> m_dealCards;                  //发牌效果用到的牌
	TipManager *m_tipManager;                         //提示
	bool m_tipping;                                   //是否正在提示
	int m_autoTipTime;                                //自动提
	Menu *m_autoCollectMenu;
	bool m_isLeftHandType;                            //是否是左手模式
	bool m_canClickReturnBt;                          //防止很快点击出现多个面板
	bool m_isAutoCollecting;                          //是否正在自动回收

public:
	static bool m_isStoreCardLibMode;                 //是否是存储卡牌库的模式

};



#endif
