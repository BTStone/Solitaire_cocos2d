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

	void setAllBgNotNew();             //���е��Ʊ���Ϊ����

	void closeEffect();        //��ʧ��Ч��
	void updateLanguage();

	int getNumFromString(string str);           //����button��name��ȡbutton�ı��
protected:
	void selectBgCallBack(Ref *pSender, Widget::TouchEventType type);
	void msgInviteSuccessCallBack(Ref *ref);
protected:
	void btnCloseCallBack(Ref *pSender, Widget::TouchEventType type);

};


#endif
