#ifndef __ST_INVITE_FRIEND_SIMPLE_BOARD_H__
#define __ST_INVITE_FRIEND_SIMPLE_BOARD_H__

#include "OptionBaseBoard.h"

class InviteFriendSimpleBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(InviteFriendSimpleBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();


protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnInviteCallBack(Ref *pSender, Widget::TouchEventType type);
};

#endif
