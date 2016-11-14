#ifndef __ST_SOLITAIRE_ROOT__OPTION_BOARD_H__
#define __ST_SOLITAIRE_ROOT__OPTION_BOARD_H__

#include "OptionBaseBoard.h"
#include "MySwichControl.h"
#include "CardSprite.h"

class RootOptionBoard : public OptionBaseBoard
{
public:
	RootOptionBoard();
	~RootOptionBoard();
	static RootOptionBoard* create();
	virtual bool init();


	void initRootCSBNode();
	void handleButton();
	void updateCheckBoxState();
	void updateTheme();
	void updateScreenSignPosition();

	void closeEffect();        //消失的效果

protected:
	void switchHintCallBack(Ref *ref);
	void switchSoundCallBack(Ref *ref);
	void switchDrawCallBack(Ref *ref);
	void switchLeftHandCallBack(Ref *ref);
	

	void msgUpdateUICallBack(Ref *ref);                             //选择皮肤后，更新显示
	void msgUpdateLanguageCallBack(Ref *ref);


	void boxSwitchHintCallBack(Ref *ref, CheckBoxEventType tp);
	void boxSwitchSoundCallBack(Ref *ref, CheckBoxEventType tp);
	void boxSwitchDrawCallBack(Ref *ref, CheckBoxEventType tp);
	void boxSwitchLeftHandCallBack(Ref *ref, CheckBoxEventType tp);


protected:
	void btnBackDropCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnFrontStyleCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnBackStyleCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnLanguageCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnContactUsCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnInviteCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnAutoScreenCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnPortraitScreenCallBack(Ref *pSender, Widget::TouchEventType type);
	void btnLandscapeScreenCallBack(Ref *pSender, Widget::TouchEventType type);

	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);


};

#endif
