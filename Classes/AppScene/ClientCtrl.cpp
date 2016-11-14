#include "ClientCtrl.h"
#include "ClientAndroid.h"
#include "LockManager.h"
#include "ScreenDirectionManager.h"
#include "OptionBaseBoard.h"
#include "OptionBoardManager.h"
#include "PlayBoard.h"
//#include "MetaCocos.h"
/*
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//android
#include "MobClickCpp.h"
using namespace umeng;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  //ios
#include "MobClickCpp.h"
using namespace umeng;
#else         //win32

#endif
*/
ClientCtrl* ClientCtrl::m_shared = NULL;


// BEGIN of ScreenCtrl
// ================================================
bool ScreenCtrl::isPortrait()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//android
	return ClientAndroid::isScreenOriatationPortrait();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  //ios
	return true;      //暂时
#else         //win32
	return(St_is_portrait);
#endif
}

// ================================================
// END of ScreenCtrl

// BEGIN of SoundCtrl
// ================================================
static const char * sStrIsSoundOn = "isSoundOn";
bool SoundCtrl::isSoundOn()
{
	bool defaultVal = true;
	return UserDefault::getInstance()->getBoolForKey(sStrIsSoundOn, defaultVal);
}
void SoundCtrl::soundSwitch()
{
	bool isNowOn = isSoundOn();
	soundSwitch(!isNowOn);
}
void SoundCtrl::soundSwitch(bool isOn)
{
	UserDefault::getInstance()->setBoolForKey(sStrIsSoundOn, isOn);
}
void SoundCtrl::playSound(const char * effectFile, bool loop)
{
	ST_RETURN_IF(!isSoundOn());
	SimpleAudioEngine::getInstance()->playEffect(effectFile, loop);
}
void SoundCtrl::playBackGroundSound(const char *backGroundFile, bool loop)
{
	ST_RETURN_IF(!isSoundOn());
	SimpleAudioEngine::getInstance()->playBackgroundMusic(backGroundFile, loop);
}
// ================================================
// END of SoundCtrl


// BEGIN of UMengSDKCtrl
// ================================================

void UMengSDKCtrl::initUMeng()
{/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("57454d1867e58eaa820027d8", "Google");  //初始化umeng
	umeng::MobClickCpp::updateOnlineConfig();
#endif
	*/
}

void UMengSDKCtrl::umengApplicationDidEnterBackground()
{/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	umeng::MobClickCpp::applicationDidEnterBackground();
#endif
	*/
}
void UMengSDKCtrl::umengApplicationWillEnterForeground()
{/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	umeng::MobClickCpp::applicationWillEnterForeground();
#endif*/
}
void UMengSDKCtrl::umengEnd()
{/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	umeng::MobClickCpp::end();
#endif*/
}

void UMengSDKCtrl::reportEventParam(const char * eventId, const char * param)
{
	string paramStr = "";
	if (param != NULL)
	{
		paramStr = param;
	}
	else
	{
		paramStr = eventId;// STLang::shared()->getReportParamByEventId(eventId);
	}
	//log("ParamStrddd:%s",paramStr.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//MobClickCpp::event(eventId, param);
	ClientAndroid::onEvent(eventId, paramStr);
	
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//IOS
#endif
	
}


void UMengSDKCtrl::reportEventMap(const char *eventId, std::map<string, string> mp, int counter)
{
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		eventDict dict;
		map<string, string>::iterator   it;
		for (it = mp.begin(); it != mp.end(); it++)
		{
			string key = it->first;
			string val = it->second;
			dict[key.c_str()] = val.c_str();
		}
		if (counter == -1)
		{
			MobClickCpp::event(eventId, &dict);
		}
		else
		{
			MobClickCpp::event(eventId, &dict, counter);
		}
#endif
	*/	
	
}

void UMengSDKCtrl::reportNotifyClickInfo(int id)
{
	const char * pars[] = { "DailyChallengeIsReady", "LeaveThreeDays","NoFinishGame"};

	ST_RETURN_IF((id != Notification_Daily_Challenge_Id) &&
		(id != Notification_Three_Days_Leave) &&
		(id != Notification_No_Win));

	int paramIdx = id - Notification_Daily_Challenge_Id;
	ClientCtrl::share()->reportEventParam("ClickNotification", pars[paramIdx]);
}

void UMengSDKCtrl::startLevel(const char * enventId)
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//umeng::MobClickCpp::startLevel(enventId);
	ClientAndroid::startLevel(enventId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

void UMengSDKCtrl::finishLevel(const char * enventId)
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//umeng::MobClickCpp::finishLevel(enventId);
	ClientAndroid::finishLevel(enventId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

void UMengSDKCtrl::failLevel(const char * enventId)
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//umeng::MobClickCpp::failLevel(enventId);
	ClientAndroid::failLevel(enventId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

string UMengSDKCtrl::getUMengOnlineConfig(string keyStr)
{
	string value = "";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//value = MobClickCpp::getConfigParam(keyStr.c_str());
	value = ClientAndroid::getUMengOnlineConfig(keyStr);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
	return value;
}
string UMengSDKCtrl::parseUMengOnlineConfig(string str, string keyStr)
{
	int stLen = str.size();
	string tempStr = "";
	string reStr = "";
	bool isValue = false;   //'='之后为value
	bool inArea = false;
	for (int i = 0; i < stLen; i++)
	{
		bool isNotSp = str[i] != ' ' && str[i] != '\n' && str[i] != '\t';      //非空格
		if (str[i] == '{')
		{
			inArea = true;
		}
		else if (str[i] == '}')
		{
			inArea = false;
			isValue = false;
			if (strcmp(keyStr.c_str(), tempStr.c_str()) == 0)
			{
				return reStr;
			}
			tempStr = "";
			reStr = "";
		}
		else if (str[i] == '=' && inArea)
		{
			isValue = true;
		}
		else if (isNotSp && !isValue && inArea)
		{
			tempStr += str[i];
		}
		else if (isValue && isNotSp)
		{
			reStr += str[i];
		}
		
	}
	return "";
}

vector<string> UMengSDKCtrl::splitStringByChar(string resStr, char ch)
{
	vector<string> re;
	string str = "";
	int len = resStr.size();
	for (int i = 0; i < len; i++)
	{
		bool isNotSp = resStr[i] != ' ' && resStr[i] != '\n' && resStr[i] != '\t';      //非空格
		if (isNotSp && resStr[i] != ch)
		{
			str += resStr[i];
		}
		else if (resStr[i] == ch)
		{
			re.push_back(str);
			str = "";
		}
	}
	if (str.size() > 3)
	{
		re.push_back(str);
	}
	return re;
}

// ================================================
// END of UMengSDKCtrl



// BEGIN of AdsCtrl
// ================================================

void AdsCtrl::showExitBoard()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::showExitBoard();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}


void AdsCtrl::showGameAdsPrepare()
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::showGameAdsPrepare();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}


void AdsCtrl::showGameAds()
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::showGameAds();
	ClientCtrl::share()->reportEventParam("AdsNewGame");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

void AdsCtrl::showBanner()
{
	int playNum = UserDefault::getInstance()->getIntegerForKey(STRG_TOTAL_PLAY_NUM, 0);
	ST_RETURN_IF(playNum <= 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::showBanner();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

void AdsCtrl::hideBanner()
{
	log("AdsCtrl,hideBanner");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::hideBanner();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

void AdsCtrl::tryToPreGameAds()
{
	m_preGameAdsNum++;
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string gameAdStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_interstitial");
	log("tryToPreGameAds,adsString:%s,gameAdStr:%s, prenum:%d", adsString.c_str(), gameAdStr.c_str(), m_preGameAdsNum);
	if (gameAdStr == "0")
	{
		log("tryToPreGameAds0000000000,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		return;
	}
	else if (gameAdStr == "1")
	{
		log("tryToPreGameAds1111111111,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		ClientCtrl::share()->showGameAdsPrepare();
	}
	else if (gameAdStr == "2")
	{
		log("tryToPreGameAds22222222222,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		bool canShow = (m_preGameAdsNum % 2 == 0);
		if (0)
		{
			canShow = (rand() % 2) == 0;
		}
		if (canShow)
		{
			ClientCtrl::share()->showGameAdsPrepare();
		}
	}
	else if (gameAdStr == "3")
	{
		log("tryToPreGameAds3333333333,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		int randAd = rand() % 4;
		if (randAd != 0)
		{
			ClientCtrl::share()->showGameAdsPrepare();
		}
	}
	else
	{
		log("tryToPreGameAds555555555,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		ClientCtrl::share()->showGameAdsPrepare();
	}
}

void AdsCtrl::tryToShowGameAds()
{
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string gameAdStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_interstitial");
	log("tryToShowGameAds,adsString:%s,gameAdStr:%s, prenum:%d", adsString.c_str(), gameAdStr.c_str(), m_preGameAdsNum);
	if (gameAdStr == "0")
	{
		log("tryToShowGameAds0000000,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		return;
	}
	else if (gameAdStr == "1")
	{
		log("tryToShowGameAds11111111111,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		ClientCtrl::share()->showGameAds();
	}
	else if (gameAdStr == "2")
	{
		log("tryToShowGameAds222222,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		bool canShow = (m_preGameAdsNum % 2 == 0);
		if (0)
		{
			canShow = (rand() % 2) == 0;
		}
		if (canShow)
		{
			ClientCtrl::share()->showGameAds();
		}
	}
	else if (gameAdStr == "3")
	{
		log("tryToShowGameAds33333333333,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		int randAd = rand() % 4;
		if (randAd != 0)
		{
			ClientCtrl::share()->showGameAds();
		}
	}
	else
	{
		log("tryToShowGameAds555555555,adsString:%s,gameAdStr:%s", adsString.c_str(), gameAdStr.c_str());
		ClientCtrl::share()->showGameAds();
	}
}


void AdsCtrl::tryToShowBanner()
{
	bool isPort = ClientCtrl::share()->isPortrait();
	ST_RETURN_IF(!isPort);
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	log("AdsCtrl::tryToShowBanner, adsString:%s, bannerString:%s", adsString.c_str(), bannerStr.c_str());
	if (bannerStr == "0")
	{
		return;
	}
	else if (bannerStr == "1")
	{
		ClientCtrl::share()->showBanner();
	}
	else if (bannerStr == "2")
	{
		bool isRate = UserDefault::getInstance()->getBoolForKey(STRG_HAS_ALREADY_RATE_US, false);
		if (isRate)
		{
			ClientCtrl::share()->showBanner();
		}
	}
	else if (bannerStr == "3")
	{
		int sz = OptionBoardManager::share()->size();
		bool hasOptionBoard = (sz > 0);
		bool hasPlayBoard = PlayBoard::isShowing();
		if (hasOptionBoard || hasPlayBoard)
		{
			ClientCtrl::share()->showBanner();
		}
	}
	else
	{
		ClientCtrl::share()->showBanner();
	}
}


void AdsCtrl::tryToHideBanner()
{
	string adsString = ClientCtrl::share()->getUMengOnlineConfig(ST_UMENG_ADS_PARAM);
	string bannerStr = ClientCtrl::share()->parseUMengOnlineConfig(adsString, "ad_banner");
	if (bannerStr == "3")
	{
		hideBanner();
	}
}

bool AdsCtrl::isDevAdOn()
{
	log("AdsCtrl,isDevAdOn");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return ClientAndroid::isDevAdOn();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	return true;
#endif
}

void AdsCtrl::moreGameIcon()
{
	log("AdsCtrl,moreGameIcon");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::moreGameIcon();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#endif
}

// ================================================
// END of AdsCtrl




// BEGIN of ClientCtrl
// ================================================

ClientCtrl* ClientCtrl::share()
{
	if (!m_shared)
	{
		m_shared = new ClientCtrl();
		m_shared->init();
	}
	return m_shared;
}

void ClientCtrl::init()
{
	m_preGameAdsNum = 0;
	initUMeng();

	ScreenDirectionManager::share()->init();
}

void ClientCtrl::rateApp(int starNum)
{
	//解锁
	LockManager::share()->unLockAllByRate();
	ST_RETURN_IF(starNum < 4);
	//跳转网页
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char * rateUrl = "https://play.google.com/store/apps/details?id=magic.sunflower.card.solitaire.free";  //必须从c++传到java，不然会闪退
	ClientAndroid::urlView(rateUrl);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	MessageBox("Test for rate", "");
#endif
}

void ClientCtrl::contactUs()
{
	ClientCtrl::share()->reportEventParam("ContactUs");
	//发送邮件
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char * str = "mailto:mplayonline@hotmail.com";  //必须从c++传到java，不然会闪退
	ClientAndroid::mailTo(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	MessageBox("Test for contact", "");
#endif
}

string ClientCtrl::getNetData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return ClientAndroid::getData();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	time_t ss = STTimeManager::share()->getLocalTime();
	struct tm *localTm = localtime(&ss);
	char buf[64];
	sprintf(buf, "%d-%02d-%02d", localTm->tm_year+1900, localTm->tm_mon+1, localTm->tm_mday);
	return buf;
#else
	time_t ss = STTimeManager::share()->getLocalTime();
	struct tm *localTm = localtime(&ss);
	char buf[64];
	sprintf(buf, "%d-%02d-%02d", localTm->tm_year+1900, localTm->tm_mon+1, localTm->tm_mday);
	return buf;
#endif
}


void ClientCtrl::setNotification(string titleStr, string contStr, int delayMs, int id)
{
	//推送
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::setNotification(titleStr, contStr, delayMs, id);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

void ClientCtrl::cancelNotification(int id)
{
	//推送
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::cancelNotification(id);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

void ClientCtrl::setScreenDic(int dic)
{
	//推送
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::setScreenDic(dic);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

int ClientCtrl::getScreenHeight()
{
	int re = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	re = ClientAndroid::getScreenHeight();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
	return re;
}

int ClientCtrl::getScreenWidth()
{
	int re = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	re = ClientAndroid::getScreenWidth();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
	return re;
}

void ClientCtrl::googleInvite()
{
	//解锁
	LockManager::share()->unLockAllByInvite();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::googleInvite();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

void ClientCtrl::googleRank()
{
	string defaultId = "";
	//MetaCocos::getInstance()->invokeMeta(MP_SHOWRANK, defaultId);
	//return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::googleRank();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

void ClientCtrl::submitScore(int num)
{
	string idStr = "CgkIoNS1so0bEAIQBg";
	submitLeader(idStr, num);
}

void ClientCtrl::submitMoves(int num)
{
	string idStr = "CgkIoNS1so0bEAIQBw";
	submitLeader(idStr, num);
}

void ClientCtrl::submitTime(int num)
{
	string idStr = "CgkIoNS1so0bEAIQCA";
	submitLeader(idStr, num);
}

void ClientCtrl::submitLeader(string boardId, int num)
{
	if (0)         //Meta sdk
	{
		char buf[64];
		sprintf(buf, "%d", num);
		string str = "{\"score:\"";
		str += buf;
		str += ",\"category:\"";
		str += boardId;
		str += "}";
		//MetaCocos::getInstance()->invokeMeta(MP_UPLOADSCORE, str);
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		ClientAndroid::submitLeader(boardId, num);    //Common_magic sdk
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
		//MessageBox("Test for setNotification", "");
#endif
	}
}

string ClientCtrl::getBackGroundThemeNames()
{
	string re = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	re = ClientAndroid::getBackGroundThemeNames();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
	return re;
}

string ClientCtrl::getCardBackThemeNames()
{
	string re = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	re = ClientAndroid::getCardBackThemeNames();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
	return re;
}

bool ClientCtrl::isAllAppInstall(vector<string> packageNames)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return false;
#endif
	int len = packageNames.size();
	string packageName = "";
	for (int i = 0; i < len; i++)
	{
		packageName = "";
		packageName += packageNames[i];
		log("isAllAppInstallxx, packageName:%s", packageName.c_str());
		if (!isInstall(packageName))
		{
			return false;
		}
	}
	return true;
}
bool ClientCtrl::isInstall(string packageName)
{
	bool re = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	re = ClientAndroid::isInstall(packageName);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
	return re;
}


void ClientCtrl::huanJingSdkTest(int xPer, int yPer, int wPer)
{
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	int x = xPer / 100.0 * frameSize.width;
	int y = frameSize.height - yPer / 100.0 *frameSize.height;
	int w = wPer / 100.0 * frameSize.width;
	log("ClientCtrl::huanjingsdktest,xPer:%d, yPer:%d, wPer:%d, x:%d, y:%d, w:%d",xPer, yPer, wPer, x, y, w);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ClientAndroid::huanJingSdkTest(x, y, w);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
	//MessageBox("Test for setNotification", "");
#endif
}

// ================================================
// END of ClientCtrl
