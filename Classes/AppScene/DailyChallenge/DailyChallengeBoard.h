#ifndef __ST_DAILY_CHALLENGE_BOARD_H__
#define __ST_DAILY_CHALLENGE_BOARD_H__

#include "OptionBaseBoard.h"
#include "Calendar.h"
#include "NetTime.h"

#define This_Month_Total_Label 222
#define Daily_Copper_Tag       333        //铜牌
#define Daily_Silver_Tag       334        //银牌
#define Daily_Golden_Tag       335        //金牌

class DailyChallengeBoard : public OptionBaseBoard
{
public:
	DailyChallengeBoard();
	~DailyChallengeBoard();
	static DailyChallengeBoard* create(int year, int mon, int day);
	virtual bool init(int year, int mon, int day);

	void initRootCSBNode();
	void handleButton();
	void initCalendar(int year, int mon, int day);     //日历

	void refresh();
	void refreshCrownOfSolved(bool isSolved);
	void refreshPlayMenu(bool isSuc);
	void addSuccessEffect();
	void showTrophyInfoBoard();

	void tryToShowRightSign();                          //在金牌下添加对号标志
	static void setIsAutoShow(bool isAuto);
	static bool getIsAutoShow();

	void closeEffect();
	void updateLanguage();
	void updateUIState();
protected:
	void startDailyChallengeGame();                    //开始每日挑战
	void menuTrophyCliekCallBack(Ref *pSender, Widget::TouchEventType type); //点击奖杯
	void closeTrophyInfoBoardCallBack(Ref *ref);

	void msgSelectCalendarDayCallBack(Ref *ref);        //选择了日历上的日期，相应的更新m_dataLabel, m_solvedStatus, 以及进度条的信息

	void addTrophyInfoBoard(int btTag, Vec2 pos);

protected:
	void btnHelpCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnPlayCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void checkBoxSwitchCallBack(cocos2d::Ref *pSender, CheckBoxEventType type);


protected:
	Calendar *m_calendar;
	static bool m_isAutoShow;            //是否自动展示
};

#endif
