/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package magic.sunflower.card.solitaire.free;


import android.app.Activity;


 //* magic
import us.magic.sdk.SDK;
import us.magic.sdk.ads.AdBannerType;
import us.magic.sdk.ads.canvas.listener.OnExitListener;


import com.facebook.AppEventsLogger;
import com.google.android.gms.appinvite.AppInviteInvitation;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.games.Games;

import com.kochava.android.tracker.Feature;
//import com.magiplay.adsdk.AdSDK;

/*
///085
import com.racergame.racer.SDK;
import com.racergame.racer.ads.AdBannerType;
import com.racergame.racer.ads.dialog.listener.OnExitListener;
*/

//import com.umeng.mobclickcpp.MobClickCppHelper;
//import com.umeng.onlineconfig.OnlineConfigAgent;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.SystemClock;

import android.view.KeyEvent;
import android.widget.Toast;

import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.media.AudioManager;
import android.net.Uri;
import android.content.Intent;


import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;


public class AppActivity extends BaseGameActivity{
	public static Context sContext = null;
	public static Activity sActivity = null;
	public static BaseGameActivity sBaseGameActivity = null;
	public static AppActivity sAppActivity = null;
	public static long firstTime = 0;
	public static int sScreenWidth = 0;
	public static int sScreenHeight = 0;
    private static String sThemeGameBg = "";
    private static String sThemeCardBg = "";
	
	final int RC_UNUSED = 5001;
	

    private static int sCommitNum = 0;
    private static String sBoardId = "";

    private static int RESPONSE_INVITE = 987412024;
    private static int REQUEST_INVITE = 987412023;
    private static int REQUEST_NOTICE = 987412022; 
    private static String appInviteKey = "solitaireAppInvite";
    
	public static native void switchScreen(boolean isPortrait); 
	public static native void reportNotifyClickInfo(int id); 
	public static native void clickReturnButton(); 
	public static native void applyNewTheme(String gameBgName, String cardBgName, String themePkgName); 
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		sContext = this;
		sActivity = this;
		sBaseGameActivity = this;
		sAppActivity = this;
		//UMGameAgent.setDebugMode(true);
		//OnlineConfigAgent.getInstance().updateOnlineConfig(this);
		//MobClickCppHelper.init(this);
		STComm.debugLog("adroidoncreatexxxxffxxff");
		
		//AdSDK.init(this);
		
		SDK.useUmengGame(true);
		SDK.onCreate(this);	
		SDK.showFullScreen(this);
		SDK.adRequestBanner( this, AdBannerType.CENTER_BOTTOM);

		UMeng.init(this);
	
		reportUmeng(getIntent());
		copyOnCreate();
		Feature kTracker = new Feature( this, "");
	}
	
	
	protected void onNewIntent(Intent intent) {
		STComm.debugLog("onNewIntenttt");
		super.onNewIntent(intent);
		this.setIntent(intent);
		copyOnCreate();
		reportUmeng(getIntent());
	}
	
	private boolean _copy(String b, String toDir, String bName){
    	try {
//        	String 
        	Uri returnUri = Uri.parse(b);
        	AssetFileDescriptor parcelFileDescriptor = getContentResolver().openAssetFileDescriptor(returnUri, "r");
        	
        	//Log.d("GameApp", String.format("%d", parcelFileDescriptor.getDeclaredLength()));
//			FileDescriptor fileDescriptor = parcelFileDescriptor.getFileDescriptor();
        	//Log.d("GameApp", "2");
//        	fileDescriptor.r
        	InputStream inStream = parcelFileDescriptor.createInputStream();
        	inStream.reset();
        	
        	
//	        Bitmap bitmap = BitmapFactory.decodeStream(i);//decodeFileDescriptor(fileDescriptor);
	        String file_path = this.getFilesDir().getAbsolutePath() + 
	        		toDir ;
			File dir = new File(file_path);
			if(!dir.exists()){
				dir.mkdirs();
			}
			File file = new File(dir, bName);
			FileOutputStream fOut = new FileOutputStream(file);
			 byte[] buffer=new byte[1024];
		        int length=-1;
		        while((length=inStream.read(buffer))!=-1)   {
		        	fOut.write(buffer,0,length);
		        }
//			fOut.write(i.)
//			bitmap.compress(Bitmap.CompressFormat.JPEG, 85, fOut);
			fOut.flush();
			fOut.close();
	        
        	//Log.d("GameApp", "3");
	        parcelFileDescriptor.close();
	        return true;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			//Log.d("GameApp", e.getMessage());
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	return false;
    }

	public void copyOnCreate() {
		STComm.debugLog("xxxcopyoncreate");
		Intent intent = getIntent();
//      Uri returnUri = intent.getData();
        Bundle extras = intent.getExtras();
        String b = null;
      String card = null;
      String cardName = null;
      String bName = null;
      String themePkg = null;
      if(extras != null){
//      	extras.getS
  		STComm.debugLog("extranotnull");
      	b = extras.getString("b");
      	bName = extras.getString("b.name");
      	card = extras.getString("card");
      	cardName = extras.getString("card.name");
      	themePkg = extras.getString("themePkgName");
      	
      	if(b != null && card != null && cardName!= null && bName != null){
          	//Log.d("GameApp", b.toString());
          	this._copy(b, "/background/", bName);
          	this._copy(card, "/card/", cardName);
          	
          	STComm.debugLog("copyOnCreateParam,bName:" + bName + "cardName:" + cardName + "themePkg:" + themePkg);
          	applyNewTheme(bName, cardName, themePkg);
        }
      	
      	
      }
		
	}
	
	public static String getAllBackGroundTheme(){
		return sAppActivity.getAllFileNameInDirBackGround();
	}
	
	public static String getAllCardBackTheme(){
		return sAppActivity.getAllFileNameInDirCard();
	}
	
	public String getAllFileNameInDirBackGround(){
        STComm.debugLog("getAllFileNameInDirBackGround");
		String toDir = "/background/";
		String file_path = this.getFilesDir().getAbsolutePath() + 
	        		toDir ;
		File file = new File(file_path);
		if(!file.exists()){
			file.mkdirs();
		}
        File[] subFile = file.listFiles();
        
        String re = "";
 
        for (int iFileLength = 0; iFileLength < subFile.length; iFileLength++) {
            // 閸掋倖鏌囬弰顖氭儊娑撶儤鏋冩禒璺恒仚
            if (!subFile[iFileLength].isDirectory()) {
                String filename = subFile[iFileLength].getName();
                re = re + filename + ",";
                
            }
        }
        STComm.debugLog("gggetThemenames:" + re + ",lengh:" + Integer.toString(subFile.length));
		 return re;
	}
	public String getAllFileNameInDirCard(){

        STComm.debugLog("getAllFileNameInDirCard");
		String toDir = "/card/";
		String file_path = this.getFilesDir().getAbsolutePath() + 
	        		toDir ;
		File file = new File(file_path);
		if(!file.exists()){
			file.mkdirs();
		}
        File[] subFile = file.listFiles();
        
        String re = "";
 
        for (int iFileLength = 0; iFileLength < subFile.length; iFileLength++) {
            // 閸掋倖鏌囬弰顖氭儊娑撶儤鏋冩禒璺恒仚
            if (!subFile[iFileLength].isDirectory()) {
                String filename = subFile[iFileLength].getName();
                re = re + filename + ",";
                
            }
        }
        STComm.debugLog("xxxxetThemenames:" + re);
		 return re;
	}
	
	@Override
	public void onResume() {
		super.onResume();
		
		SDK.onResume(this);
		//AdSDK.onResume();
		AppEventsLogger.activateApp(this);
	}
	
	public static void googleRank() {
		sAppActivity.googleRank_();
	}
	
	public void googleRank_() {
		/*
		if (isSignedIn()) {
			STComm.debugLog("googleRank_,has sign in");
			startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(getApiClient()),RC_UNUSED);
	    } else {
			STComm.debugLog("googleRank_,has not sign in");
	    	try {
				beginUserInitiatedSignIn();
			} 
			catch (Exception e) {
				// TODO: handle exception
			}

        }*/
		
		int re = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(sActivity);
        //Log.d("isGooglePlayServicesAvailable","re:" + re);
        
        if( re != ConnectionResult.SUCCESS)
        {
        	return;
        }
		
		if (isSignedIn()) {
			STComm.debugLog("googleRank_,has sign in");
			startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(getApiClient()),RC_UNUSED);
	    } else {
			STComm.debugLog("googleRank_,has not sign in");
			beginUserInitiatedSignIn();
        }
	}

	public static void submitScore(String id, int score) {
		sAppActivity.submitScore_(id,score);
	}
	
	public void submitScore_(String id, int score) {
		if(isSignedIn()){
			STComm.debugLog("submitScore_,has sign in, boardId:" + id + ",num:" + Integer.toString(score));
			Games.Leaderboards.submitScore(getApiClient(),id, score);
		}
		/*
	    sCommitNum = score;
	    sBoardId = id;
	    if (!isSignedIn()) {
	    	STComm.debugLog("submitScore_,has not sign in");
            return; 
        }
		sActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				STComm.debugLog("submitScore_,has sign in, boardId:" + sBoardId + ",num:" + Integer.toString(sCommitNum));
				Games.Leaderboards.submitScore(getApiClient(),sBoardId, sCommitNum);
			}
		});*/
		
		
	}

	
	public static void googleInvite() {
		String title = "Message";
		String message = "Your friend sent a Windows 10 theme to you.";
		String act = "Invite act";
		STComm.debugLog("javagoogleInvite");
		 Intent intent = new AppInviteInvitation.IntentBuilder(title)
	        .setMessage(message)
//	        .setCustomImage(Uri.parse("https://lh3.googleusercontent.com/EbMsVqe3zOnAFJYsHHQi-CDQXvt5oZstkUGa4i4liRGKmeXwHXEboAJVRHATUXv8J8kb=w300-rw"))
//	        .setCallToActionText(act)
	        .build();
	        intent.putExtra(appInviteKey, 1);
	        int re = GoogleApiAvailability.getInstance().isGooglePlayServicesAvailable(sActivity);
	        //Log.d("isGooglePlayServicesAvailable","re:" + re);
	        
	        if( re == ConnectionResult.SUCCESS)
	        {
	        	sActivity.startActivityForResult(intent, REQUEST_INVITE);
	        }
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    super.onActivityResult(requestCode, resultCode, data);
	    //Agent.onActivityResult( requestCode,  resultCode,  data);
	    //Log.d("appActivity", "onActivityResult: requestCode=" + requestCode + ", resultCode=" + resultCode);
	    if (requestCode == REQUEST_INVITE) {
	        if (resultCode == RESULT_OK) {
	            // Get the invitation IDs of all sent messages
	        	//Log.d("appActivity", "resultOk");
	    	    
	            String[] ids = AppInviteInvitation.getInvitationIds(resultCode, data);
	            for (String id : ids) {
	                //Log.d("appActivity" , "onActivityResult: sent invitation " + id);
	            }
	        } else {
	            // Sending failed or it was canceled, show failure message to the user
	            // ...
	        }
	    }
	}
	    
	
	@Override
	public void onPause() {
		
		SDK.onPause(this);
		
		super.onPause();
		//AdSDK.onPause();
		AppEventsLogger.deactivateApp(this);
	}

	@Override
	public void onDestroy() {
		
		SDK.onDestroy(this);
		
		super.onDestroy();
	}
	
	@Override
	public void onStart() {
        super.onStart(); 
        
		//SDK.onStart(this);
	}
	
	@Override
	public void onStop() {
		
		//SDK.onStop(this);
		
		super.onStop();
	}
	
	@Override
	public void onBackPressed() {
		
		if(!SDK.onBackPressed()){
			   return;
		}
		SDK.exit( AppActivity.this,new OnExitListener() {
			@Override
			public void onExitEvent() {
				SDK.exitExtra();
				android.os.Process.killProcess(android.os.Process.myPid());
			}});	
			
	}
	/*
	static {
        MobClickCppHelper.loadLibrary();
	}
	*/
	public void reportUmeng(Intent intent) {
		Bundle extras = intent.getExtras();
	    if(extras != null){
	        if(extras.containsKey("notice_type"))
	        {
	            String notice_type = intent.getStringExtra("notice_type");
	            int id = intent.getIntExtra("notice_id", 0);
	            int message_id = intent.getIntExtra("message_id", -1);
	            
	            STComm.debugLog("notifyId:" + Integer.toString(id));
	            
	            reportNotifyClickInfo(id);
	        }
	    }
	}
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig); 
		if(newConfig.orientation==Configuration.ORIENTATION_PORTRAIT){      
			switchScreen(true);
		}   
		else if(newConfig.orientation==Configuration.ORIENTATION_LANDSCAPE){    
			switchScreen(false);
		}
		//STComm.debugLog("onConfigurationChanged,screenH:" + Integer.toString(sScreenHeight) + "screenW:" + Integer.toString(sScreenWidth));
	}
	
	 public static boolean isScreenOriatationPortrait() { 
		 return sContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT; 
     }
	 
	 public static boolean isInstall(String pkgName) { 
		 return sAppActivity.isPkgInstalled(pkgName); 
     }
	 
	 private boolean isPkgInstalled(String pkgName) {
    	PackageInfo packageInfo = null;
    	try {
    	  packageInfo = this.getPackageManager().getPackageInfo(pkgName, 0);
    	} catch (NameNotFoundException e) {
    	  packageInfo = null;
    	  e.printStackTrace();
    	}
    	if (packageInfo == null) {
    	  return false;
    	} else {
    	  return true;
    	}
	}
	 
	 //more 閻犲搫鐤囧ù锟�
	 public static void moreGame() { 
		 
		 SDK.showMoreGames(sActivity); 
		 
     }
	 
	 public static void exitGame() { 
		 
		 SDK.exit(sActivity,new OnExitListener() {
				@Override
				public void onExitEvent() {
					SDK.exitExtra();
					android.os.Process.killProcess(android.os.Process.myPid());
		}});
		  
     }
	 
	 public static void showExitGameBoard() { 
		 
		 sActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				SDK.exit(sActivity,new OnExitListener() {
					@Override
					public void onExitEvent() {
						SDK.exitExtra();
						android.os.Process.killProcess(android.os.Process.myPid());
				}});
			}
		});
		
     }
	 
	 //闁圭粯甯掗惈鍡涚嵁閸喗鍟�
	 public static void showGameAdsPrepare() { 
		 
		 sActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				 SDK.showGameAd(sActivity,0); //yiwendian

			}
		});
		
     }
	 
	 
	 public static void showGameAds() { 
		 
		 sActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					 SDK.showGameAd(sActivity,1);

				}
			});
			
     }
	 
	 public static void showBanner() { 
		 
		 sActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					SDK.showBanner(sActivity);
				}});
				
     }
	 
	 public static void hideBanner() { 
		 
		 sActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					 SDK.hideBanner(sActivity);

				}
			});
			
     }
	 
	 public static boolean isDevAdOn() { 
		 return SDK.getDevAdSwitch();
			
     }
	 
	 public static void huanJingSdkTest(final int x, final int y, final int w) { 
		 sActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					//AdSDK.showBounceView(x, y, w);
				}
			});
			
     }
	 
	 public static void moreGameIcon() { 
		 sActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					STComm.debugLog("moregameiconx");
					 SDK.devAdClick();
					 SDK.getDevAdSwitch();
				}
			});
			
     }
	 
	 public static void setScreenDic(int dic) { 
		 switch (dic)
		 {
		 case 0:
			 sActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_USER);
			 break;
		 case 1:
			 sActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
			 break;
		 case -1:
			 //sActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_USER_LANDSCAPE);
			 sActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
			 break;
		 default:
			 
			 break;
			 
		 }
     }
	 
	 public static int getScreenWidth() { 
		 int re = 0;
		 DisplayMetrics dm = new DisplayMetrics();

		 sActivity.getWindowManager().getDefaultDisplay().getMetrics(dm);

		 int screenWidth =dm.widthPixels;

		 int screenHeight =dm.heightPixels;
		 re = screenWidth;
		 
		 return re;
		
     }
	 
	 public static int getScreenHeight() { 
		 int re = 0;
		 DisplayMetrics dm = new DisplayMetrics();

		 sActivity.getWindowManager().getDefaultDisplay().getMetrics(dm);

		 int screenWidth =dm.widthPixels;

		 int screenHeight =dm.heightPixels;
		 re = screenHeight;
		
		 return re;
     }
	 
	
	 public static String getDatax() { 
		 URL url;  
		 String re = "0-0-0";
		 try {  
			 re = "";
		     url = new URL("http://www.baidu.com");  
		     HttpURLConnection  uc = (HttpURLConnection)url.openConnection();// 闁汇垻鍠愰崹姘交閻愭潙澶嶉悗鐢殿攰閽栵拷  
		     uc.setDoInput(true);
		     uc.setDoOutput(true); 
		     uc.setConnectTimeout(1000);
		     uc.setReadTimeout(1000);
		     uc.setRequestMethod("GET");
		     uc.connect(); // 闁告瑦鍨甸崵顓熸交閻愭潙澶�  
		     //uc.setRequestMethod(GET);
			 int code = uc.getResponseCode();
			 if (code == 200) {
			     long ld = uc.getDate(); // 闁告瑦鐗曠欢杈╃磾閹寸姷褰查柡鍐﹀劜濠�锟犲籍閸洘锛�  
			     Date date = new Date(ld); // 閺夌儐鍓氬畷鍙夌▔閻戞鍨奸柛鎴濇濡炲倿姊婚弶鎴殸閻犵儑鎷� 
			     SimpleDateFormat sdFormatter = new SimpleDateFormat("yyyy-MM-dd");
			     String retStrFormatNowDate = sdFormatter.format(date);
			     re = retStrFormatNowDate;
			 } else {
				 re = "0-0-0";
			 }
		 } catch (MalformedURLException e) {  
		     // TODO Auto-generated catch block  
		     e.printStackTrace();  
		     re = "0-0-0";
		 }// 闁告瑦鐗曠欢杈╂導閸曨剛鐖遍悗鐢殿攰閽栵拷  
		 catch (Exception  e) {  
		     // TODO Auto-generated catch block  
		     e.printStackTrace();  
		     re = "0-0-0";
		 }  
		 return re;
     }
	 
	 public static String getData() { 
		 String re = "";
		 SntpClient client = new SntpClient();
	        if (client.requestTime("pool.ntp.org", 5000)) {
	            long now = client.getNtpTime() + SystemClock.elapsedRealtime() - client.getNtpTimeReference();
//	            Date current = new Date(client.getNtpTime());
	            Date current = new Date(now);
	            SimpleDateFormat sdFormatter = new SimpleDateFormat("yyyy-MM-dd");
	            String retStrFormatNowDate = sdFormatter.format(current);
			     re = retStrFormatNowDate;
	            //Log.d("Egg", current.toString());
	        }else{
	        	re = "0-0-0";
	        }

		 return re;
     }
	 //閻犲搫鐤囧ù鍡欑磾閹达负锟斤拷
	 public static void urlView(String url) {
			Uri uri = Uri.parse(url);
			Intent it = new Intent(Intent.ACTION_VIEW, uri);
			sActivity.startActivity(it); 
	    }
	 
	 //闁告瑦鍨块敓浠嬫焽椤旂粯顐�
	 public static void mailTo(String uriStr) {	
		 Uri uri = Uri.parse(uriStr); 
		 Intent intent = new Intent(Intent.ACTION_SENDTO, uri);  
		 String titleStr = "Solitaire sunflower feedback";
		 //intent.putExtra(Intent.EXTRA_CC, email); // 闁硅泛瀚伴敓鑺ョ閿燂拷 
		 TelephonyManager tm = (TelephonyManager) sActivity.getSystemService(Context.TELEPHONY_SERVICE);  

		 
         String mtype = android.os.Build.MODEL; // 闁归潧顑嗗┃锟介柛銊ヮ儏瑜帮拷
         String systemtype = android.os.Build.VERSION.RELEASE; // 缂侇垵宕电划娲偋閸喐鎷遍柛娆欐嫹
         String APPVersion = getAppVersionName(sContext);//app 闁绘鐗婂﹢锟�
         String sdkVersion = "";  
         
         titleStr = titleStr + "(" + mtype + "," + systemtype + "," + APPVersion + "," + sdkVersion + ")";
		 intent.putExtra(Intent.EXTRA_SUBJECT, titleStr); // 濞戞挸顭烽。锟�  
		 //intent.putExtra(Intent.EXTRA_TEXT, "this is doc"); // 婵繐绲鹃弸锟�  
  
		sActivity.startActivity(Intent.createChooser(intent, "Send Email"));

		}
	 
	 public static String getAppVersionName(Context context) {  
		    String versionName = "";  
		    try {  
		        // ---get the package info---  
		        PackageManager pm = context.getPackageManager();  
		        PackageInfo pi = pm.getPackageInfo(context.getPackageName(), 0);  
		        versionName = pi.versionName;  
		        //versioncode = pi.versionCode;
		        if (versionName == null || versionName.length() <= 0) {  
		            return "";  
		        }  
		    } catch (Exception e) {  
		        Log.e("VersionInfo", "Exception", e);  
		    }  
		    return versionName;  
		}  
	 
	 @Override 
     public boolean onKeyUp(int keyCode, KeyEvent event) { 
        if (keyCode == KeyEvent.KEYCODE_BACK) { 
        	boolean bl = true;
        	if(bl){
        		clickReturnButton();
        	}else{
        		long secondTime = System.currentTimeMillis(); 
                if (secondTime - firstTime > 800) {//婵犵锟藉啿锟藉綊鎮樻径瀣枖闁靛牆妫崑褔鏌熺粙娆炬闁轰緡鍙冨顕�宕奸弴鐕傜吹闂傚倸鍊归幐鍐测枔瑜庡鍕槺缂侇煉鎷�00濠殿噯缍嗛崑鍡涒�栭幋锔芥櫖閻忕偠妫勯悘鐔封槈閹惧磭效闁跨喍绮欏畷娆撴晸閿燂拷
                    Toast.makeText(this, "Press again to quit.", 
                            Toast.LENGTH_SHORT).show(); 
                    firstTime = secondTime;//闂佸搫娲ら悺銊╁蓟婵傜杯rstTime 
                    return true; 
                } else { 
                	//exitGame();
                    System.exit(0);//闂佸憡鐔粻鎴﹀垂椤栫偞鐒婚柨鐔绘濮ｅ绱掔�ｎ亶鍎忕紒顭掓嫹
                } 
        	}
        	
            
        } 
        return super.onKeyUp(keyCode, event); 
    }
	 
	 @Override
	 public boolean onKeyDown(int keyCode, KeyEvent event) { 
		 AudioManager mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		 int currentVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
		 if( keyCode == KeyEvent.KEYCODE_VOLUME_UP)// 闂傚﹥濞婇崳鐑樻櫠閻愬眰浜�
		 {
			mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume+1, 1);
			return true;
		 }
		 else if( keyCode == KeyEvent.KEYCODE_VOLUME_DOWN)// 闂傚﹥濞婇崳鐑樻櫠閻愬眰浜�
		 {
			mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume-1, 1);
			return true;
		 }
		 //return true;
        return super.onKeyDown(keyCode, event); 
    }
	@Override
	public void onSignInFailed() {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onSignInSucceeded() {
		// TODO Auto-generated method stub
		
	}
}
