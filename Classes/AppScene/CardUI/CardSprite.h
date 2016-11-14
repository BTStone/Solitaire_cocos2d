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

	bool containThePos(Vec2 worldPos);               //�ж����������Ƿ�������ֽ����

	void open();                                     //�ɹر�״̬���򿪵Ĺ���
	void close();                                    //�ɴ򿪵��ر�
	void showOpenStatus();                           //չʾ�򿪵�״̬
	void showClosedStatus();                         //չʾ�رյ�״̬

	void tipOverCallFunc();
	CC_SYNTHESIZE_READONLY(Card, m_cardInfo, CardInfo);
protected:
	Sprite *m_cardBg;                                //ֽ�Ƶı���������������棩
	Node *m_cardFace;                                //ֽ�Ƶ�����Ԫ�أ����֡���ɫ��
	static float m_scale;                            //ֽ�Ƶ����ű���
	static Size m_cardSize;                          //ֽ�ƵĴ�С(contentZize * scale)
	//Scale9Sprite *m_tip;
};

#endif
