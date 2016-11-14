#ifndef __ST_CLIENT_ANDROID_H__
#define __ST_CLIENT_ANDROID_H__

#include "STBase.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class ClientAndroid
{
public:
	static bool isScreenOriatationPortrait();
	static void urlView(const char * uriStr);                             // 链接跳转
	static void setNotification();                                        //推送通知栏
	static void setNotification(string titleStr, string contentStr,int delayMs, int id);
	static void cancelNotification(int id);
	static void mailTo(const char * email);                              // 发送邮件
	static const char * getData();
	static void showGameAdsPrepare();          //插屏广告
	static void showGameAds();          //插屏广告
	static void showBanner();           //banner
	static void hideBanner();           //隐藏banner

	static bool isDevAdOn();                   //more game icon是否打开
	static void moreGameIcon();         //更多广告

	static void showExitBoard();

	static void setScreenDic(int dic);


	static int getScreenWidth();
	static int getScreenHeight();
	static void googleInvite();
	static void googleRank();
	static void submitLeader(string boardId, int num);

	static void onEvent(string eventId, string paramStr);
	static void onEvent(string eventId);
	static void startLevel(string eventId);
	static void finishLevel(string eventId);
	static void failLevel(string eventId);
	static const char * getUMengOnlineConfig(string keyStr);

	static const char * getBackGroundThemeNames();
	static const char * getCardBackThemeNames();

	static void huanJingSdkTest(int x, int y, int w);

	static bool isInstall(string packageName);
};




#endif // CC_TARGET_PLATFORM

void switchScreen(bool isPortrait);
#endif
