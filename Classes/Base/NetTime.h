/*

��ȡ����ʱ��
˼·����ʹ�ðٶȵ�ʱ��api��ȡ����ҳ��Ȼ������ҳ��ץȡ��ʱ��� �鿴��ҳ��Դ����Ϳ��Է���Դ��������һ��

window.baidu_time(1447841230045); ��һ�����־���ʱ��� ��������ֻҪ��ȷ���뼶����ȡǰ��10λ�Ϳ��ԡ�

ץȡʱ�������ͨ����window.baidu_time(���ؼ��ְ���ҳԴ�����ַ����и������ Ȼ��ȡ����һ���ǰ��10���ַ����ٰ��ַ���ת��Ϊʱ��Ϳ����ˡ�
*/
#ifndef __ST_NET_TIME_H__
#define __ST_NET_TIME_H__
#include "STBase.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"      //����

class NetTime;
typedef void (Ref::*SEL_NetTime)(NetTime * pSender);
#define netTime_selector(_SELECTOR) (SEL_NetTime)(&_SELECTOR)

class NetTime : public Node
{
public:
	CREATE_FUNC(NetTime);
	void requestNetTime(); 
	void requestNetTime(Ref * pTarget, SEL_NetTime pSucSelector, SEL_NetTime pFailSelector);
	void addConnectEffect();            //�����������ʱЧ������ɫ��͸������ת
	CC_SYNTHESIZE(int, m_day, Day);
	CC_SYNTHESIZE(int, m_month, Month);
	CC_SYNTHESIZE(int, m_year, Year);
	CC_SYNTHESIZE(int, m_hour, Hour);
	CC_SYNTHESIZE(int, m_second, Second);
	CC_SYNTHESIZE(int, m_minute, Minute);
	CC_SYNTHESIZE(time_t, m_time, Time);//ʱ��chuo 
	static void splitString(std::vector<std::string> &contentVector, std::string content, std::string pattern);//�и��ַ���
protected:
	virtual bool init();
	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);
	void readJson(std::string jsonStr);
	void spliteTime(std::string htmlStr);
	void initTime(time_t time);
	void convertStrToTime(string str);
protected:
	SEL_NetTime m_pSuccessSelector;          //ͨѶ�ɹ�ʱ�Ļص�
	SEL_NetTime m_pFailSelector;             //ͨѶʧ��ʱ�Ļص�(����)
	Ref * m_pTarget;

};


#endif