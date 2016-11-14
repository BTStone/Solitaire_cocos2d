#ifndef __ST_SOLITAIRE_CARD_SPRITE_H__
#define __ST_SOLITAIRE_CARD_SPRITE_H__

#include "STBase.h"
#include "Card.h"
USING_NS_CC;

class CardSprite : public Node
{
public:
	static CardSprite* create(Card card);
	static CardSprite* create(CardSprite *cardSprite);
	static void setCardScaleAndSize();
	virtual bool init(Card card);
	virtual bool init(CardSprite *cardSprite);
	void initCard();
	void initCardBg();
	void initCardFace();
	string getNumFrameNameByID(int faceID);
	string getSmallColorFrameNameByID(int faceID);
	string getBigColorFrameNameByID(int faceID);

	static float getCardScale();
	static Size getCardSize();

	bool containThePos(Vec2 worldPos);               //判断世界坐标是否在这张纸牌内

	void open();                                     //由关闭状态到打开的过程
	void close();                                    //由打开到关闭
	void showOpenStatus();                           //展示打开的状态
	void showClosedStatus();                         //展示关闭的状态

	void tipOverCallFunc();
	CC_SYNTHESIZE_READONLY(Card, m_cardInfo, CardInfo);
protected:
	Sprite *m_cardBg;                                //纸牌的背景（背面或者正面）
	Node *m_cardFace;                                //纸牌的正面元素（数字、花色）
	static float m_scale;                            //纸牌的缩放比例
	static Size m_cardSize;                          //纸牌的大小(contentZize * scale)
	//Scale9Sprite *m_tip;
};

#endif
