#ifndef __ST_DAILY_CHALLENGE_HELP_BOARD_H__
#define __ST_DAILY_CHALLENGE_HELP_BOARD_H__

#include "OptionBaseBoard.h"


class DailyChallengeHelpBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(DailyChallengeHelpBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void updateCenterShow();
	void closeEffect();

	void refresh();
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnLeftCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnRightCallBack(Ref *pSender, Widget::TouchEventType type);

protected:
	int m_curIdx;               //当前第几个
	
};



#endif
