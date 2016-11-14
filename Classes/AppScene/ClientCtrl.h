#ifndef __ST_CLIENT_CTRL_H__
#define __ST_CLIENT_CTRL_H__

#include "STBase.h"

class ScreenCtrl
{
public:
	bool isPortrait();                    //是否是竖屏
protected:

};

class SoundCtrl
{
public:
	bool isSoundOn();                                 // 【集成】是否展示音效。UI层通过该判断，决定显示“音效”或者“没有音效”图标
	void soundSwitch();                               // 【集成】切换音效。点击“音效”或者“没有音效”图标后调用。音效设置已经实现持久化（写文件）
	void playSound(const char * effectFile, bool loop = false);               // 播放音效
	void playBackGroundSound(const char *backGroundFile, bool loop = true);   //播放背景音乐
protected:
	void soundSwitch(bool isOn);       // 切换音效开关
};

class UMengSDKCtrl
{
public:
	void initUMeng();
	void umengApplicationDidEnterBackground();
	void umengApplicationWillEnterForeground();
	void umengEnd();
	static void reportEventParam(const char * eventToken, const char * param = NULL);  // 上报事件
	static void reportEventMap(const char *eventToken, std::map<string, string> mp, int counter = -1);
	static void reportNotifyClickInfo(int id);
	static void startLevel(const char * enventId);
	static void finishLevel(const char * enventId);
	static void failLevel(const char * enventId);

	string parseUMengOnlineConfig(string str, string keyStr);
	vector<string> splitStringByChar(string resStr, char ch = ',');           //根据分隔符ch拆分字符串

	string getUMengOnlineConfig(string keyStr);


protected:
	//int initUMengSDK();
	//void didEnterBG();
	//void willEnterFG();
};

class AdsCtrl
{
public:
	void showExitBoard();
	void showGameAdsPrepare();          //插屏广告
	void showGameAds();                 //插屏广告
	void showBanner();                  //banner
	void hideBanner();                  //隐藏banner

	bool isDevAdOn();                   //more game icon是否打开
	void moreGameIcon();                //更多广告


	void tryToShowBanner();
	void tryToShowGameAds();                         //插屏广告
	void tryToHideBanner();
	void tryToPreGameAds();

protected:
	int m_preGameAdsNum;

};

class ClientCtrl : public SoundCtrl, public ScreenCtrl, public UMengSDKCtrl, public AdsCtrl
{
public:
	static ClientCtrl * share();
	void init();                             // 初始化客户端相关内容

	void rateApp(int starNum);               //评价
	void contactUs();                        //联系我们
	void setNotification(string titleStr, string contStr, int delayMs, int id);                  //推送通知栏
	void cancelNotification(int id);
	string getNetData();
	void setScreenDic(int dic);

	int getScreenWidth();
	int getScreenHeight();

	void googleInvite();

	void googleRank();
	void submitScore(int num);
	void submitMoves(int num);
	void submitTime(int num);
	void submitLeader(string boardId, int num);

	bool isAllAppInstall(vector<string> packageNames);
	bool isInstall(string packageName);

	string getBackGroundThemeNames();
	string getCardBackThemeNames();

	void huanJingSdkTest(int xPer, int yPer, int wPer);

protected:
	static ClientCtrl *m_shared;
};

#endif
