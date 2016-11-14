package magic.sunflower.card.solitaire.free;

import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.game.UMGameAgent;
//import com.umeng.onlineconfig.OnlineConfigAgent;

import android.app.Activity;
import android.content.Context;

public class UMeng {
	public static Context sContext = null;
	
	public static void init(Activity activity){
		sContext = activity; 
	}
	
	public static void onEvent(String eventId, String param) {
		STComm.debugLog("oneevent,id:" + eventId + "param:" + param);
		MobclickAgent.onEvent(sContext, eventId, param);
		STComm.debugLog("oneeventend,id:" + eventId + "param:" + param);
	} 
	public static void onEvent(String eventId) {
		STComm.debugLog("oneeventx,id:" + eventId);
		MobclickAgent.onEvent(sContext, eventId);
		STComm.debugLog("oneeventxend,id:" + eventId);
	} 
	 
	public static void startLevel(String eventId) {
		STComm.debugLog("startLevel, id:" + eventId);
		UMGameAgent.startLevel(eventId);
		STComm.debugLog("startLevelend, id:" + eventId);
	} 
	 
	public static void finishLevel(String eventId) {
		STComm.debugLog("finishLevel, id:" + eventId);
		UMGameAgent.finishLevel(eventId);
		STComm.debugLog("finishLevelend, id:" + eventId);
	} 
	 
	public static void failLevel(String eventId) {
		STComm.debugLog("failLevel, id:" + eventId);
		UMGameAgent.failLevel(eventId);
		STComm.debugLog("failLevelend, id:" + eventId);
	} 
	public static String getUMengOnlineConfig(String keyStr) { 
		 String re = "";
		 //re = OnlineConfigAgent.getInstance().getConfigParams(sContext, keyStr);
		 re = MobclickAgent.getConfigParams(sContext, keyStr);
		 STComm.debugLog("getUMengOnlineConfig,keyStr:" + keyStr + ",re:" + re);
		 return re;
    }
}
