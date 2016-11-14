#ifndef __ST_SOLITAIRE_WIN_BOARD_H__
#define __ST_SOLITAIRE_WIN_BOARD_H__

#include "OptionBaseBoard.h"

class WinBoard : public OptionBaseBoard
{
public:
	static void setData(int moves, int time, int score, int todayBest, int record);
	static WinBoard* create();
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void updateContentKey();
	void updateContentValue();
	
	void addSnowEffect();
	void closeEffect();
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnNewGameCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnInviteCallBack(Ref *pSender, Widget::TouchEventType type);

protected:
	virtual void removeSelfCallFunc();
	string convertToTimeStyle(int sec);
protected:
	static int m_moves;             //步数
	static int m_time;              //用时
	static int m_score;             //分数
	static int m_todayBest;         //今日最好分数
	static int m_record;            //最好
	
};


#endif
