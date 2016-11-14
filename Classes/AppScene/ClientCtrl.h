#ifndef __ST_CLIENT_CTRL_H__
#define __ST_CLIENT_CTRL_H__

#include "STBase.h"

class ScreenCtrl
{
public:
	bool isPortrait();                    //�Ƿ�������
protected:

};

class SoundCtrl
{
public:
	bool isSoundOn();                                 // �����ɡ��Ƿ�չʾ��Ч��UI��ͨ�����жϣ�������ʾ����Ч�����ߡ�û����Ч��ͼ��
	void soundSwitch();                               // �����ɡ��л���Ч���������Ч�����ߡ�û����Ч��ͼ�����á���Ч�����Ѿ�ʵ�ֳ־û���д�ļ���
	void playSound(const char * effectFile, bool loop = false);               // ������Ч
	void playBackGroundSound(const char *backGroundFile, bool loop = true);   //���ű�������
protected:
	void soundSwitch(bool isOn);       // �л���Ч����
};

class UMengSDKCtrl
{
public:
	void initUMeng();
	void umengApplicationDidEnterBackground();
	void umengApplicationWillEnterForeground();
	void umengEnd();
	static void reportEventParam(const char * eventToken, const char * param = NULL);  // �ϱ��¼�
	static void reportEventMap(const char *eventToken, std::map<string, string> mp, int counter = -1);
	static void reportNotifyClickInfo(int id);
	static void startLevel(const char * enventId);
	static void finishLevel(const char * enventId);
	static void failLevel(const char * enventId);

	string parseUMengOnlineConfig(string str, string keyStr);
	vector<string> splitStringByChar(string resStr, char ch = ',');           //���ݷָ���ch����ַ���

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
	void showGameAdsPrepare();          //�������
	void showGameAds();                 //�������
	void showBanner();                  //banner
	void hideBanner();                  //����banner

	bool isDevAdOn();                   //more game icon�Ƿ��
	void moreGameIcon();                //������


	void tryToShowBanner();
	void tryToShowGameAds();                         //�������
	void tryToHideBanner();
	void tryToPreGameAds();

protected:
	int m_preGameAdsNum;

};

class ClientCtrl : public SoundCtrl, public ScreenCtrl, public UMengSDKCtrl, public AdsCtrl
{
public:
	static ClientCtrl * share();
	void init();                             // ��ʼ���ͻ����������

	void rateApp(int starNum);               //����
	void contactUs();                        //��ϵ����
	void setNotification(string titleStr, string contStr, int delayMs, int id);                  //����֪ͨ��
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
