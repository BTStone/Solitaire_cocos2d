#ifndef __ST_SOLITAIRE_STATISTICS_BOARD_H__
#define __ST_SOLITAIRE_STATISTICS_BOARD_H__

#include "STBase.h"
#include "OptionBaseBoard.h"

class StatisticsBoard : public OptionBaseBoard
{
public:
	StatisticsBoard();
	~StatisticsBoard();
	CREATE_FUNC(StatisticsBoard);
	virtual bool init();
	void initRootCSBNode();
	void handleButton();


	string convertToTimeStyle(int sec);

	void updateLanguage();
	void updateContentKey();
	void updateContentValue();

	void resetData();
	void closeEffect();        //消失的效果
protected:
	void msgResetDataCallBack(Ref *ref);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnResetCallBack(Ref *pSender, Widget::TouchEventType type);
};

#endif
