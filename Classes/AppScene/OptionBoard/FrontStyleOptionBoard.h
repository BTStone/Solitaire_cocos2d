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
	int getNumFromString(string str);           //����button��name��ȡbutton�ı��

	void handleButton();

	void closeEffect();        //��ʧ��Ч��
	void updateLanguage();
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
};



#endif
