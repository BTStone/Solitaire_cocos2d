#ifndef __ST_NETWORK_FAIL_H__
#define __ST_NETWORK_FAIL_H__


#include "OptionBaseBoard.h"

class NetworkFailBoard : public OptionBaseBoard
{
public:
	CREATE_FUNC(NetworkFailBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();

protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
};

#endif
