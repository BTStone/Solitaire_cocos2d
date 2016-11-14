#ifndef __ST_INVITE_SUCCESS_BOARD_H__
#define __ST_INVITE_SUCCESS_BOARD_H__

#include "OptionBaseBoard.h"

class InviteSuccessBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(InviteSuccessBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();


protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	
};

#endif
