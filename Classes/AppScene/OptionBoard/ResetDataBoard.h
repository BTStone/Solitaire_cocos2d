#ifndef __ST_RESET_DATA_BOARD_H__
#define __ST_RESET_DATA_BOARD_H__

#include "OptionBaseBoard.h"

class ResetDataBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(ResetDataBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();

protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnResetCallBack(Ref *pSender, Widget::TouchEventType type);
};

#endif
