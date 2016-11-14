#ifndef __ST_RATE_SUCCESS_BOARD_H__
#define __ST_RATE_SUCCESS_BOARD_H__

#include "OptionBaseBoard.h"

class RateSuccessBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(RateSuccessBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();


protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	
};

#endif
