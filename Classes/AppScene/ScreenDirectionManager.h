#ifndef __ST_SOLITAIRE_SCREEN_DIRECTION_H__
#define __ST_SOLITAIRE_SCREEN_DIRECTION_H__

#include "STBase.h"

enum ScreenType
{
	kScreenPortrait = 0,
	kScreenLandscape
};

class ScreenDirectionManager
{
public:
	static ScreenDirectionManager* share();
	void init();
	void setSelectDic(int dic);

	CC_SYNTHESIZE(ScreenType, m_screenType, ScreenType);
	CC_SYNTHESIZE_READONLY(int, m_selectDic, SelectDic);            //����ѡ���Զ���0����������-1����������1����
protected:
	static ScreenDirectionManager *m_shared;
	

};


#endif
