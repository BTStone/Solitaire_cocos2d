#ifndef __ST_INVITE_FRIEND_BOARD_H__
#define __ST_INVITE_FRIEND_BOARD_H__

#include "OptionBaseBoard.h"

class InviteFriendBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(InviteFriendBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();

	
	static void setResString(string resStr);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnInviteCallBack(Ref *pSender, Widget::TouchEventType type);
protected:
	static string m_resString;                //来源，winBoard或者rootOptionBoard
};

#endif
