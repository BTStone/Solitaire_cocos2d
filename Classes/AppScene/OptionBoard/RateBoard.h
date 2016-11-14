#ifndef __ST_RATE_BOARD_H__
#define __ST_RATE_BOARD_H__

#include "OptionBaseBoard.h"

class RateBoard : public OptionBaseBoard
{
public:
	static bool isExist()
	{
		return m_isExist;
	}
	CREATE_FUNC(RateBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void closeEffect();


	int getNumFromString(string str);           //根据button的name获取button的标号
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnRateCallBack(Ref *pSender, Widget::TouchEventType type);
	
protected:
	void menuRateCallBack(Ref *pSender, Widget::TouchEventType type);
	
protected:
	static bool m_isExist;
};

#endif
