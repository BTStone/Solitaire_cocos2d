/*

获取网络时间
思路就是使用百度的时间api获取到网页，然后在网页中抓取到时间戳 查看网页的源代码就可以发现源代码中有一个

window.baidu_time(1447841230045); 这一串数字就是时间戳 这里我们只要精确到秒级所以取前面10位就可以。

抓取时间戳就是通过“window.baidu_time(”关键字把网页源代码字符串切割成两半 然后取后面一半的前面10个字符，再把字符串转换为时间就可以了。
*/
#ifndef __ST_NET_TIME_H__
#define __ST_NET_TIME_H__
#include "STBase.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"      //网络

class NetTime;
typedef void (Ref::*SEL_NetTime)(NetTime * pSender);
#define netTime_selector(_SELECTOR) (SEL_NetTime)(&_SELECTOR)

class NetTime : public Node
{
public:
	CREATE_FUNC(NetTime);
	void requestNetTime(); 
	void requestNetTime(Ref * pTarget, SEL_NetTime pSucSelector, SEL_NetTime pFailSelector);
	void addConnectEffect();            //添加联网进行时效果，黑色半透明，旋转
	CC_SYNTHESIZE(int, m_day, Day);
	CC_SYNTHESIZE(int, m_month, Month);
	CC_SYNTHESIZE(int, m_year, Year);
	CC_SYNTHESIZE(int, m_hour, Hour);
	CC_SYNTHESIZE(int, m_second, Second);
	CC_SYNTHESIZE(int, m_minute, Minute);
	CC_SYNTHESIZE(time_t, m_time, Time);//时间chuo 
	static void splitString(std::vector<std::string> &contentVector, std::string content, std::string pattern);//切割字符串
protected:
	virtual bool init();
	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);
	void readJson(std::string jsonStr);
	void spliteTime(std::string htmlStr);
	void initTime(time_t time);
	void convertStrToTime(string str);
protected:
	SEL_NetTime m_pSuccessSelector;          //通讯成功时的回调
	SEL_NetTime m_pFailSelector;             //通讯失败时的回调(无网)
	Ref * m_pTarget;

};


#endif