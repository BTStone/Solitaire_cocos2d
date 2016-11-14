/*���������϶����������ƶ������е��������*/
#ifndef __ST_SOLITAIRE_TRANSPORT_CARD_QUEUE_H__
#define __ST_SOLITAIRE_TRANSPORT_CARD_QUEUE_H__

#include "UICardQueue.h"


/// ��Ч��
class STShock : public ActionInterval
{
public:
	//super methods
	static STShock* create(float dt);
	bool init(float dt);
	void update(float time);
	void shock();
protected:
	int m_time;            //����Ĵ���
};

class TransportCardQueue : public UICardQueue
{
public:
	static TransportCardQueue* create(CardQueue que, CardArea area, int preIndex = 0);
	virtual bool init(CardQueue que, CardArea area, int preIndex);
	void setPortraitStatus();                                  //��ֱ��������λ��
	void setLandscapeStatus();                                 //ˮƽ��������λ��
	void setHandOpenAreaStatus();                              //ˮƽ��������λ��
	void resetCardBg();                                        //���ÿ�λʱ�ı���ͼƬ

	CC_SYNTHESIZE(int, m_resQueIndex, ResQueIndex);            //�˶���֮ǰ����������±�
	CC_SYNTHESIZE(CardArea, m_destArea, DestArea);             //Ŀ������
	CC_SYNTHESIZE(int, m_destQueIndex, DestQueIndex);          //Ŀ������Ķ���

protected:

};

#endif