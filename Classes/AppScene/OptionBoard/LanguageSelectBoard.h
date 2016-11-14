#ifndef __ST_LANGUAGE_SELECT_BOARD_H__
#define __ST_LANGUAGE_SELECT_BOARD_H__

#include "OptionBaseBoard.h"

class LanguageSelectBoard : public OptionBaseBoard
{
public:
	LanguageSelectBoard();
	~LanguageSelectBoard();
	static LanguageSelectBoard* create();
	virtual bool init();
	void initRootCSBNode();
	void handleButton();
	void updateLanguage();
	void updateTitle();
	void updateContent();
	void updateSelectSign();

	void closeEffect();


	void select(int langIdx);
	void setSelectSignPos(int langIdx);

	int getNumFromString(string str);           //根据button的name获取button的标号
protected:
	void menuLanguageSelectCallBack(Ref *pSender, Widget::TouchEventType type);

protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnSelectCallBack(Ref *pSender, Widget::TouchEventType type);
};



#endif
