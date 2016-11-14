#include "ClientAndroid.h"
#include "ClientCtrl.h"
#include "SkinManager.h"
#include "OptionBaseBoard.h"

static const char * CCJavaClassName = "magic/sunflower/card/solitaire/free/AppActivity";
static const char * UMengJavaClassName = "magic/sunflower/card/solitaire/free/UMeng";
static const char * CCJavaClassNameNotification = "magic/sunflower/card/solitaire/free/UnityNotificationManager";


void switchScreen(bool isPortrait)
{
	NotificationCenter::sharedNotificationCenter()->postNotification(ST_SWITCH_SCREEN, (Ref*)(isPortrait));
}
void exitGame()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(ST_EXIT_GAME);
}

void clickReturnButton()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(ST_CLICK_ANDROID_RETURN_BUTTON);
}

void applyNewTheme(string gameBgName, string cardBgName, string themePkgName)
{
	if (0) //直接替换
	{
		int gameBgId = SkinManager::share()->getGameBgIdByGameBgName(gameBgName);
		int cardBgId = SkinManager::share()->getCardBgIdByGameBgName(cardBgName);

		SkinManager::share()->selectGameBg(gameBgId);
		SkinManager::share()->selectCardBack(cardBgId);
	}
	else //二次提示
	{
		SkinManager::share()->refreshFileNames();
		SkinManager::share()->setFindNewTheme(true);
		SkinManager::share()->setNewGameBgName(gameBgName);
		SkinManager::share()->setNewCardBackName(cardBgName);
		NotificationCenter::getInstance()->postNotification(ST_SHOW_NEW_THEME_BOARD_DAILY);
		ClientCtrl::share()->reportEventParam("CandyThemeApply",themePkgName.c_str());
	}

	
}

void reportNotifyClickInfo(int id)
{
	ClientCtrl::share()->reportNotifyClickInfo(id);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

extern "C"
{
	void Java_magic_sunflower_card_solitaire_free_AppActivity_switchScreen(JNIEnv*  env, jobject thiz, jboolean isPortrait)
	{
		switchScreen(isPortrait);
	}

	void Java_magic_sunflower_card_solitaire_free_AppActivity_clickReturnButton(JNIEnv*  env, jobject thiz)
	{
		clickReturnButton();
	}

	void Java_magic_sunflower_card_solitaire_free_AppActivity_applyNewTheme(JNIEnv*  env, jobject thiz, jstring jGameBgName, jstring jCardBgName, jstring jThemePkg)
	{
		const char* gameBgName = env->GetStringUTFChars(jGameBgName, false);
		const char* cardBgName = env->GetStringUTFChars(jCardBgName, false);
		const char* themePkg = env->GetStringUTFChars(jThemePkg, false);
		applyNewTheme(gameBgName, cardBgName,themePkg);
	}

	void Java_magic_sunflower_card_solitaire_free_AppActivity_exitGame(JNIEnv*  env, jobject thiz)
	{
		exitGame();
	}

	void Java_magic_sunflower_card_solitaire_free_AppActivity_reportNotifyClickInfo(JNIEnv*  env, jobject thiz, jint id)
	{
		reportNotifyClickInfo(id);
	}


	bool ClientAndroid::isScreenOriatationPortrait()
	{
		JniMethodInfo t;
		bool re = true;
		if (JniHelper::getStaticMethodInfo(t, CCJavaClassName, "isScreenOriatationPortrait", "()Z"))
		{
			re = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		}
		return re;
	}


	void ClientAndroid::urlView(const char * url) {
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "urlView"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(url);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}


	void ClientAndroid::mailTo(const char * email) {
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "mailTo"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(email);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::setNotification() {
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassNameNotification
			, "SetNotification"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::setNotification(string titleStr, string contentStr,int delayMs, int id) {
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassNameNotification
			, "SetNotification"
			, "(Ljava/lang/String;Ljava/lang/String;II)V"))
		{
			jstring pStr = t.env->NewStringUTF(titleStr.c_str());
			jstring pStr2 = t.env->NewStringUTF(contentStr.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr, pStr2, delayMs, id);
			t.env->DeleteLocalRef(t.classID);
			t.env->DeleteLocalRef(pStr);
			t.env->DeleteLocalRef(pStr2);
		}
	}

	void ClientAndroid::cancelNotification(int id) {
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassNameNotification
			, "CancelNotification"
			, "(I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, id);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	const char * ClientAndroid::getData(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "getData"
			, "()Ljava/lang/String;"))
		{
			jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
			t.env->DeleteLocalRef(t.classID);
			const char* aStr = t.env->GetStringUTFChars(ret, false);
			return aStr;
		}
	}

	void ClientAndroid::showGameAdsPrepare(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "showGameAdsPrepare"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::showGameAds(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "showGameAds"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::showBanner(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "showBanner"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	void ClientAndroid::hideBanner(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "hideBanner"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	bool ClientAndroid::isDevAdOn(){
		cocos2d::JniMethodInfo t;
		bool re = false;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "isDevAdOn"
			, "()Z"))
		{
			re = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
		return re;
	}

	void ClientAndroid::moreGameIcon(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "moreGameIcon"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::showExitBoard(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "showExitGameBoard"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::setScreenDic(int dic){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "setScreenDic"
			, "(I)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, dic);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	int ClientAndroid::getScreenWidth(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "getScreenWidth"
			, "()I"))
		{
			jint ret = (jint)t.env->CallStaticIntMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
	}

	int ClientAndroid::getScreenHeight(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "getScreenHeight"
			, "()I"))
		{
			jint ret = (jint)t.env->CallStaticIntMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
			return ret;
		}
	}

	void ClientAndroid::googleInvite(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "googleInvite"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::googleRank(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "googleRank"
			, "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	}
	void ClientAndroid::submitLeader(string boardId, int num){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "submitScore"
			, "(Ljava/lang/String;I)V"))
		{
			jstring pStr = t.env->NewStringUTF(boardId.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr, num);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::huanJingSdkTest(int x, int y, int w){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "huanJingSdkTest"
			, "(III)V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID, x, y, w);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::onEvent(string eventId, string paramStr){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "onEvent"
			, "(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(eventId.c_str());
			jstring pStr2 = t.env->NewStringUTF(paramStr.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr, pStr2);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::onEvent(string eventId){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "onEvent"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(eventId.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::startLevel(string eventId){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "startLevel"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(eventId.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::finishLevel(string eventId){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "finishLevel"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(eventId.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	void ClientAndroid::failLevel(string eventId){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "failLevel"
			, "(Ljava/lang/String;)V"))
		{
			jstring pStr = t.env->NewStringUTF(eventId.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(t.classID);
		}
	}

	const char * ClientAndroid::getUMengOnlineConfig(string keyStr){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, UMengJavaClassName
			, "getUMengOnlineConfig"
			, "(Ljava/lang/String;)Ljava/lang/String;"))
		{
			jstring pStr = t.env->NewStringUTF(keyStr.c_str());
			jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID, pStr));
			t.env->DeleteLocalRef(t.classID);
			const char* aStr = t.env->GetStringUTFChars(ret, false);
			return aStr;
		}
	}

	const char * ClientAndroid::getBackGroundThemeNames(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "getAllBackGroundTheme"
			, "()Ljava/lang/String;"))
		{
			jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
			t.env->DeleteLocalRef(t.classID);
			const char* aStr = t.env->GetStringUTFChars(ret, false);
			return aStr;
		}
	}

	const char * ClientAndroid::getCardBackThemeNames(){
		cocos2d::JniMethodInfo t;
		if (cocos2d::JniHelper::getStaticMethodInfo(t
			, CCJavaClassName
			, "getAllCardBackTheme"
			, "()Ljava/lang/String;"))
		{
			jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
			t.env->DeleteLocalRef(t.classID);
			const char* aStr = t.env->GetStringUTFChars(ret, false);
			return aStr;
		}
	}

	bool ClientAndroid::isInstall(string packageName){
		cocos2d::JniMethodInfo t;
		bool re = false;
		if (JniHelper::getStaticMethodInfo(t, CCJavaClassName, "isInstall", "(Ljava/lang/String;)Z"))
		{
			jstring pStr = t.env->NewStringUTF(packageName.c_str());
			re = t.env->CallStaticBooleanMethod(t.classID, t.methodID, pStr);
			t.env->DeleteLocalRef(t.classID);
		}
		return re;
	}
}
#endif
