#ifndef __ST_SOLITAIRE_START_LAYER_H__
#define __ST_SOLITAIRE_START_LAYER_H__

#include "STBase.h"

class StartLayer : public Layer
{
public:
	StartLayer();
	~StartLayer();
	CREATE_FUNC(StartLayer);
	ST_SCENE_FUNC(StartLayer);
	virtual bool init();
	void setFrameAndDesignSizeWhenPortrait();
	void setFrameAndDesignSizeWhenLandscape();
	void setPortraitStatus();                 //竖屏
	void setLandscapeStatus();                //横屏
	void setLandscapeStatusDelay();           //橫屏等待  
	void setPortraitStatusDelay();            //竖屏等待 

	void loadPic();
	void initLogo();                          //公司logo
	void addCardShowEffect();                 //添加四张卡牌效果
	void addAppNameShowEffect();              //添加app name的效果
	void resetPosition();
protected:
	void logoShowOverCallFunc();
	void cardShowOverCallFunc();
	void appNameShowOverCallFunc();

	void msgSwitchScreen(Ref *ref);           //切换横竖屏

	static void recordInstallData();          //记录安装日期

protected:
	Sprite *m_logo;
	Node *m_cardNode;
	Sprite *m_light;
	Sprite *m_appName;
};


#endif
