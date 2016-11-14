#ifndef __ST_INVITE_DAILY_CHALLENGE_BOARD_H__
#define __ST_INVITE_DAILY_CHALLENGE_BOARD_H__

#include "OptionBaseBoard.h"

class InviteDailyChallengeBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(InviteDailyChallengeBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();


protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnOkCallBack(Ref *pSender, Widget::TouchEventType type);

};

#endif
