#ifndef __ST_SOLITAIRE_CARD_QUEUE_H__
#define __ST_SOLITAIRE_CARD_QUEUE_H__

#include "Card.h"
#include "STBase.h"
#include <vector>

using namespace std;

class CardQueue
{
public:
	CardQueue();
	CardQueue(vector<Card> cards);
	void init(CardQueue que);
	Card removeTheLastCard();                                //删除最后一个元素
	vector<Card> removeCardsAfterIndex(int index);           //删除某个元素（包括这个元素）的所有元素
	vector<Card> removeLastCardByNum(int num);               //从队列末尾删除num数量的牌
	void pushBack(Card card);                                //在最后添加一个元素
	void pushBack(vector<Card> cards);                       //在最后添加一序列元素
	void pushBack(CardQueue queData);                        //在最后添加一序列元素
	Card operator [](int index);                             //重载[]运算符
	CardQueue& operator=(CardQueue &source);                 //重载=运算符

	int size();                                              //m_cards的长度
	void clear();                                            //清空
	Card getTheLastCard();                                   //获取最后一张牌
	bool isEmpty();                                          //判断队列中是否为空

	bool canCollectTheCard(Card card);                       //判断是否可以收牌，收牌规则：同色&&比最后一张牌大于一
	void collectTheCard(Card card);                          //直接收取牌

	bool canPlaceTheCard(Card card);                         //判断是否能够放置这张牌（置牌区）
	void openTheLastCard();                                  //将最后一张牌设置为打开状态
	void closeTheLastCard();                                 //将最后一张牌设置为关闭状态
	void closeAllCard();                                     //将所有的Card的打开状态设置为false
	void openAllCard();                                      //将所有的Card的打开状态设置为true

	void reverse();                                          //反向倒置
	string convertToString(bool openLast = false);           //将本列所有牌的信息存进一个字符串里并返回,格式2，3，45，。。。
	string convertToInfoStr();                               //将本列所有牌的信息存进一个字符串里并返回,格式0-1，2-11等
	void stTest_print();
protected:
	vector<Card> m_cards;
};

const CardQueue EMPTY_CARD_QUEUE;
#endif
