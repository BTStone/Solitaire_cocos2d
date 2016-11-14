#ifndef __ST_SOLITAIRE_PLAY_BOARD_H__
#define __ST_SOLITAIRE_PLAY_BOARD_H__

#include "STBase.h"

class PlayBoard : public Node
{
public:
	static PlayBoard *create(bool canReplay = true);
	virtual bool init(bool canReplay);
	void initBg();
	void initMenu(); 
	void addSeperateMenu();
	Size getBgSize();
	void appearEffect(bool isEffect = true);       //���ֵ�Ч��
	void disappearEffect();                 //��ʧ��Ч��

	void closeCallBack(Ref *ref);

	void addMenuAppearEffect();

	static bool isShowing();
protected:
	void menuNewGameCallBack(Ref *ref);
	void menuReplayCallBack(Ref *ref);
	void menuStatisticCallBack(Ref *ref);
	void menuScoreCallBack(Ref *ref);
	void menuSevenDaysCallBack(Ref *ref);

	void addEaseEffectCallFunc(Node *node);
	void tryToShowGameAdCallFunc();
	void showSelfCallFunc(Node *node);
	void removeSelf();

protected:
	Scale9Sprite *m_bg;
	bool m_canReplay;                 //�Ƿ���replay
	static bool m_isShowing;
};

#endif
