#include "NetTime.h"
#include"cocostudio/CocoStudio.h"
#include "ClientCtrl.h"
using namespace cocostudio::timeline;

using namespace extension;
using namespace network;

bool NetTime::init()
{
	if (!Node::init())
	{
		return false;
	}
	addConnectEffect();
	return true;
}

void NetTime::addConnectEffect()
{
	STHelper::addDarkLayer(this);
}

void NetTime::requestNetTime(Ref * pTarget, SEL_NetTime pSucSelector, SEL_NetTime pFailSelector)
{
	requestNetTime();
	m_pTarget = pTarget;
	m_pSuccessSelector = pSucSelector;
	m_pFailSelector = pFailSelector;
}

void NetTime::requestNetTime()
{
	HttpRequest* request = new (std::nothrow) HttpRequest();

	//海外:http://aws.pvp.monthurs.com/time.php
	//国内:http://qdev.pvp.monthurs.com/time.php
	string urlStr = "http://aws.pvp.monthurs.com/time.php";  //默认海外
	string cfgStr = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_DAILY_CHALLENGE_PARAM);
	string value = ClientCtrl::share()->parseUMengOnlineConfig(cfgStr, "server_addr");
	log("configNettime,cfgStr:%s,value:%s", cfgStr.c_str(), value.c_str());
	if (strstr(value.c_str(), "http"))
	{
		urlStr = value;
	}
	request->setUrl(urlStr.c_str());        //replaceTemp
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(NetTime::onHttpRequestCompleted, this));
	HttpClient * httpClient = HttpClient::getInstance();
	httpClient->setTimeoutForConnect(5);
	httpClient->setTimeoutForRead(5);
	if (0)       //是否立马
	{
		request->setTag("WebTime immediate");
		HttpClient::getInstance()->sendImmediate(request);
	}
	else
	{
		request->setTag("WebTime");
		HttpClient::getInstance()->send(request);
	}
	request->release();
 }

void NetTime::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		//通讯失败调用
		if (m_pTarget && m_pFailSelector)
		{
			(m_pTarget->*m_pFailSelector)(this);
		}
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	log("response code: %ld", statusCode);
	bool issss = response->isSucceed();
	if (!response->isSucceed())
	{
		//通讯失败调用
		if (m_pTarget && m_pFailSelector)
		{
			(m_pTarget->*m_pFailSelector)(this);
		}
		//MessageBox("no internet","no internet");
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	//MessageBox("internet", "internet");
	// dump data
	std::vector<char> *buffer = response->getResponseData();
	log("Http Test, dump data: ");
	std::string str = "";
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		log("%c", (*buffer)[i]);
		char a = (*buffer)[i];
        str.append(1, a);
	}
	log("%s", str.c_str());
	if (strlen(str.c_str()) < 1)        //有网时突然中断
	{
		if (m_pTarget && m_pSuccessSelector)
		{
			(m_pTarget->*m_pFailSelector)(this);
		}
		return;
	}
	//spliteTime(str);
	convertStrToTime(str);
	//通讯成功调用
	if (m_pTarget && m_pSuccessSelector)
     {
		 (m_pTarget->*m_pSuccessSelector)(this);
     }
	/*
	log("\n");
	if (response->getHttpRequest()->getReferenceCount() != 2)
	{
		log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
	}*/
}

void NetTime::spliteTime(std::string htmlStr)
{
	std::vector<std::string> tmp;
	splitString(tmp, htmlStr, "baidu_time(");

	std::string str = tmp.at(1);
	std::string tmpTime = str.substr(0, 10);
	time_t tt = atoi(tmpTime.c_str());
	initTime(tt);
}

void NetTime::convertStrToTime(string str)
{
	time_t tt = atoi(str.c_str());
	initTime(tt);
}

void NetTime::initTime(time_t time)
{
	time_t t;
	struct tm *p;
	t = time;
	p = localtime(&t);
	/*char s[100];
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
	printf("%d: %s\n", (int)t, s);*/

	setSecond(p->tm_sec);
	setDay(p->tm_mday);
	setMonth(p->tm_mon + 1);
	setYear(p->tm_year + 1900);
	setMinute(p->tm_min);
	setHour(p->tm_hour);//北京时间等于世界时间加8小时  
	setTime(t);
}

void NetTime::splitString(std::vector<std::string> &contentVector, std::string content, std::string pattern)
{
	std::string::size_type pos;
	//std::vector<std::string> result;    
	content += pattern;//扩展字符串以方便操作    
	int size = content.size();
	log("spliteString, content:%s, size:%d", content.c_str(), size);
	for (int i = 0; i < size; i++)
	{
		pos = content.find(pattern, i);
		if (pos < size)
		{
			std::string s = content.substr(i, pos - i);
			// result.push_back(s);    
			contentVector.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

void NetTime::readJson(std::string jsonStr)
{
	 rapidjson::Document doc;
     doc.Parse<0>(jsonStr.c_str());
     if (!doc.IsObject())
         return;
	 if (doc.HasMember("Year") && doc.HasMember("Month") && doc.HasMember("Day") && doc.HasMember("Hour") && doc.HasMember("Minute") && doc.HasMember("Second"))
     {
         m_year = doc["Year"].GetInt();
         m_month = doc["Month"].GetInt();
         m_day = doc["Day"].GetInt();
         m_hour = doc["Hour"].GetInt();
         m_minute = doc["Minute"].GetInt();
         m_second = doc["Second"].GetInt();
     }
 }
