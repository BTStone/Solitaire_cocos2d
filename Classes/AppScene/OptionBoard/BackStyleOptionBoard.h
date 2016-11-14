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

	void setAllBgNotNew();             //���е��Ʊ���Ϊ����

	int getNumFromString(string str);           //����button��name��ȡbutton�ı��

	void closeEffect();        //��ʧ��Ч��
	void updateLanguage();
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
	void msgRateSuccessCallBack(Ref *ref);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);
};



#endif
