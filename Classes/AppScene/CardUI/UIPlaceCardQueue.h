#ifndef __ST_SOLITAIRE_UI_PLACE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_UI_PLACE_CARD_QUEUE_H__

#include "UICardQueue.h"
#include "TransportCardQueue.h"

#define CLOSE_CARD_GAP_SCALE 0.3

class UIPlaceCardQueue : public UICardQueue
{
public:
	static UIPlaceCardQueue* create(CardQueue queData);
	virtual bool init(CardQueue queData);
	TransportCardQueue* search(Vec2 touchPos);                    //根据点击的位置查找点击到的对象，加入到运输队列，并从原来的队列中删除
	void initCardsPosition();                                     //设置位置
	void resetCardBg();                                           //重置空位时的背景图片
	void addCards(CardQueue queData);
	Vec2 getPositionAt(int index);                                //获取某一下标下纸牌所在的位置（非世界坐标）
	bool canPlaceTheCard(Card card);                              //是否能够放置这张牌
	Vec2 getNextPosition();                                       //下张牌要放置的世界坐标
	bool tryToOpenTheLastCard();                                  //尝试打开最后一张牌，如果已经打开则返回false，如果没有打开则打开，返回true
	bool tryToCloseTheLastCard();                                 //尝试关闭最后一张牌， 如果已经关闭返回false，否则返回true
	bool isInTheLastCard(Vec2 worldPos);                          //此点是否在最后一张牌上（检测点）
	bool intersectsTheCard(Vec2 worldCardPos);                    //此点是否在最后一张牌上（检测接触）
	CardSprite *getTheSecondToLastCard();                         //获取倒数第二张牌
	CardSprite *at(int idx);
	Vec2 getCardWorldPosition(int idx);
	vector<Card> getCardsAfter(int idx);
	bool containTheCard(Card card);

	int getOpenCardNum();                                         //获取打开牌的个数
	float getOverLenth();                                         //获取超出的长度
	bool willOverScreen();                                        //是否将要超出屏幕
	bool alreadyOverScreen();                                     //是否已经超出屏幕
	bool tryToScale();                                            //尝试缩放（超出屏幕）,可以缩放则返回true，否则返回false
	float getOpenCardMoveByLenth();
	void openCardMoveBy(float mvBy);                              //打开状态的牌缩放（不包括第一个）
	void scaleOverCallFunc();
	void scaleWhenOverScreen();                                   //超出屏幕是缩放
	bool scaleWhenRemoveCard();
	bool isOriginGapOfOpenCard();                                 //判断m_openCardGapY是否是初始间距cardSize.height * Place_Area_Gap_Y;
protected:
	float m_openCardGapY;                                  //展开牌之间的间距Y
	float m_originGapY;                                    //展开牌之间的初始间距Y
};

#endif
