#ifndef __ST_NEW_THEME_TIP_BOARD_H__
#define __ST_NEW_THEME_TIP_BOARD_H__

#include "OptionBaseBoard.h"

class NewThemeTipBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(NewThemeTipBoard);
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
