#ifndef __ST_SOLITAIRE_FRONT_STYLE_OPTION_BOARD__
#define __ST_SOLITAIRE_FRONT_STYLE_OPTION_BOARD__


#include "OptionBaseBoard.h"
#include "CardSprite.h"

class FrontStyleOptionBoard : public OptionBaseBoard//,public ScrollViewDelegate
{
public:
	CREATE_FUNC(FrontStyleOptionBoard);
	virtual bool init();
	void initRootCSBNode();
	void select(int idx);
	void resetInUseSignPos(int selectNum);
	int getNumFromString(string str);           //根据button的name获取button的标号

	void handleButton();

	void closeEffect();        //消失的效果
	void updateLanguage();
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
};



#endif
