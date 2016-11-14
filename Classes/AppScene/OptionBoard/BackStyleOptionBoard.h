#ifndef __ST_SOLITAIRE_BACK_STYLE_OPTION_BOARD__
#define __ST_SOLITAIRE_BACK_STYLE_OPTION_BOARD__


#include "OptionBaseBoard.h"

class BackStyleOptionBoard : public OptionBaseBoard//,public ScrollViewDelegate
{
public:
	BackStyleOptionBoard();
	~BackStyleOptionBoard();
	CREATE_FUNC(BackStyleOptionBoard);
	virtual bool init();
	void initRootCSBNode();
	void select(int idx);
	void handleButton();


	void showNewSignAt(int idx);
	void showLockSignAt(int idx);
	void hideAllLock();
	void hideAllNewSign();


	void refreshLockAndNew();
	void resetInUseSignPos(int selectNum);

	void setAllBgNotNew();             //所有的牌背设为不新

	int getNumFromString(string str);           //根据button的name获取button的标号

	void closeEffect();        //消失的效果
	void updateLanguage();
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
	void msgRateSuccessCallBack(Ref *ref);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
};



#endif
