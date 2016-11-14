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
	void setPortraitStatus();                 //����
	void setLandscapeStatus();                //����
	void setLandscapeStatusDelay();           //�M���ȴ�  
	void setPortraitStatusDelay();            //�����ȴ� 

	void loadPic();
	void initLogo();                          //��˾logo
	void addCardShowEffect();                 //������ſ���Ч��
	void addAppNameShowEffect();              //���app name��Ч��
	void resetPosition();
protected:
	void logoShowOverCallFunc();
	void cardShowOverCallFunc();
	void appNameShowOverCallFunc();

	void msgSwitchScreen(Ref *ref);           //�л�������

	static void recordInstallData();          //��¼��װ����

protected:
	Sprite *m_logo;
	Node *m_cardNode;
	Sprite *m_light;
	Sprite *m_appName;
};


#endif
