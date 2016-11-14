#ifndef __ST_SOLITAIRE_BACK_DROP_BOARD_H__
#define __ST_SOLITAIRE_BACK_DROP_BOARD_H__

#include "OptionBaseBoard.h"


class BackDropBoard : public OptionBaseBoard//,public ScrollViewDelegate
{
public:
	BackDropBoard();
	~BackDropBoard();
	CREATE_FUNC(BackDropBoard);
	virtual bool init();
	void initRootCSBNode();
	void select(int idx);
	void handleButton();

	void hideAllLock();
	void hideAllNewSign();

	void refreshLockAndNew();
	void resetInUseSignPos(int selectNum);

	void showNewSignAt(int idx);
	void showLockSignAt(int idx);

	void setAllBgNotNew();             //所有的牌背设为不新

	void closeEffect();        //消失的效果
	void updateLanguage();

	int getNumFromString(string str);           //根据button的name获取button的标号
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
	void msgInviteSuccessCallBack(Ref *ref);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);

};


#endif
