//package net.agasper.unitynotification;
package magic.sunflower.card.solitaire.free;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.media.RingtoneManager;
import android.support.v4.app.NotificationCompat;

import magic.sunflower.card.solitaire.free.STComm;
//import com.unity3d.player.UnityPlayerNativeActivity;

public class UnityNotificationManager extends BroadcastReceiver
{	
	public static void SetNotification(String title, String message, int delayMs, int id)
    {
		String ticker = "Solitaire";
		int sound = 0;
		int vibrate = 0;
		int lights = 0;
		String largeIconResource = "icon";
		String smallIconResource = "icon";
		int bgColor = 100;
		int executeMode = 1;
		String unityClass = "magic.sunflower.card.solitaire.free.UnityNotificationManager";
    	String idStr = Integer.toString(id);
    	String delayStr = Integer.toString(delayMs);
    	STComm.debugLog("idStr:"+idStr+",delayStr:"+delayStr);
    	Activity currentActivity = AppActivity.sActivity;
        AlarmManager am = (AlarmManager)currentActivity.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(currentActivity, UnityNotificationManager.class);
        intent.putExtra("ticker", ticker);
        intent.putExtra("title", title);
        intent.putExtra("message", message);
        intent.putExtra("id", id);
        intent.putExtra("color", bgColor);
        intent.putExtra("sound", sound == 1);
        intent.putExtra("vibrate", vibrate == 1);
        intent.putExtra("lights", lights == 1);
        intent.putExtra("l_icon", largeIconResource);
        intent.putExtra("s_icon", smallIconResource);
        intent.putExtra("activity", unityClass);//System.currentTimeMillis() +
    	am.set(0,  System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));

//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
//        {
//        	if (executeMode == 2)
//        		am.setExactAndAllowWhileIdle(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        	else if (executeMode == 1)
//        		am.setExact(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        	else
//        		am.set(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        }
//        else
//        	am.set(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
		
    }
	
	public static void SetNotification()
    {
		int id = 1000;
		long delayMs = 0;
		String title = "Notification Test";
		String message = "Notification Test";
		String ticker = "Notification Test";
		int sound = 0;
		int vibrate = 0;
		int lights = 0;
		String largeIconResource = "icon";
		String smallIconResource = "icon";
		int bgColor = 100;
		int executeMode = 1;
		String unityClass = "magic.sunflower.card.solitaire.free.UnityNotificationManager";
		
    	
    	STComm.debugLog("UnityNotificationManager");
    	
    	//Toast.makeText(currentActivity, "UnityNotificationManager.", 
                //Toast.LENGTH_SHORT).show(); 
    	Activity currentActivity = AppActivity.sActivity;
        AlarmManager am = (AlarmManager)currentActivity.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(currentActivity, UnityNotificationManager.class);
        intent.putExtra("ticker", ticker);
        intent.putExtra("title", title);
        intent.putExtra("message", message);
        intent.putExtra("id", id);
        intent.putExtra("color", bgColor);
        intent.putExtra("sound", sound == 1);
        intent.putExtra("vibrate", vibrate == 1);
        intent.putExtra("lights", lights == 1);
        intent.putExtra("l_icon", largeIconResource);
        intent.putExtra("s_icon", smallIconResource);
        intent.putExtra("activity", unityClass);//System.currentTimeMillis() +
    	am.set(0,  delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));

//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
//        {
//        	if (executeMode == 2)
//        		am.setExactAndAllowWhileIdle(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        	else if (executeMode == 1)
//        		am.setExact(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        	else
//        		am.set(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
//        }
//        else
//        	am.set(0, System.currentTimeMillis() + delayMs, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
		
    }
    
    public static void SetRepeatingNotification(int id, long delay, String title, String message, String ticker, long rep, int sound, int vibrate, int lights, 
    		String largeIconResource, String smallIconResource, int bgColor, String unityClass)
    {
    	Activity currentActivity = AppActivity.sActivity;
    	AlarmManager am = (AlarmManager)currentActivity.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(currentActivity, UnityNotificationManager.class);
        intent.putExtra("ticker", ticker);
        intent.putExtra("title", title);
        intent.putExtra("message", message);
        intent.putExtra("id", id);
        intent.putExtra("color", bgColor);
        intent.putExtra("sound", sound == 1);
        intent.putExtra("vibrate", vibrate == 1);
        intent.putExtra("lights", lights == 1);
        intent.putExtra("l_icon", largeIconResource);
        intent.putExtra("s_icon", smallIconResource);
        intent.putExtra("activity", unityClass);
    	am.setRepeating(0, System.currentTimeMillis() + delay, rep, PendingIntent.getBroadcast(currentActivity, id, intent, 0));
    }
    
//    @SuppressLint("NewApi")
    @Override 
	public void onReceive(Context context, Intent intent)
    {
    	STComm.debugLog("UnityNotificationManager onReceive");
    	NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);

        String ticker = intent.getStringExtra("ticker");
        String title = intent.getStringExtra("title");
        String message = intent.getStringExtra("message");
        String s_icon = intent.getStringExtra("s_icon");
        String l_icon = intent.getStringExtra("l_icon");
        int color = intent.getIntExtra("color", 0);
        String unityClass = intent.getStringExtra("activity");
        Boolean sound = intent.getBooleanExtra("sound", false);
        Boolean vibrate = intent.getBooleanExtra("vibrate", false);
        Boolean lights = intent.getBooleanExtra("lights", false);
        int id = intent.getIntExtra("id", 0);

        Resources res = context.getResources();

//        Class<?> unityClassActivity = null;
//		try 
//		{
//			unityClassActivity = Class.forName(unityClass);
//		} catch (ClassNotFoundException e) 
//		{
//			e.printStackTrace();
//		}
		
	    Intent appIntent = context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
	    appIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
	    appIntent.putExtra("notice_type", "local");
	    appIntent.putExtra("notice_id", id);

        PendingIntent contentIntent = PendingIntent.getActivity(context, 0, appIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);
        
        builder.setContentIntent(contentIntent)
        	.setWhen(System.currentTimeMillis())
        	.setAutoCancel(true)
        	.setContentTitle(title)
        	.setContentText(message);
        
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
//        	builder.setColor(color);
        
        if(ticker != null && ticker.length() > 0)
            builder.setTicker(ticker);
               
		if (s_icon != null && s_icon.length() > 0)
			builder.setSmallIcon(res.getIdentifier(s_icon, "drawable", context.getPackageName()));
		
		if (l_icon != null && l_icon.length() > 0)
			builder.setLargeIcon(BitmapFactory.decodeResource(res, res.getIdentifier(l_icon, "drawable", context.getPackageName())));

        if(sound)
            builder.setSound(RingtoneManager.getDefaultUri(2));

        if(vibrate)
            builder.setVibrate(new long[] {
                    1000L, 1000L
            });

        if(lights)
            builder.setLights(Color.GREEN, 3000, 3000);
        
        Notification notification = builder.build();
        notificationManager.notify(id, notification);
    }

    public static void CancelNotification(int id)
    {
    	Activity currentActivity = AppActivity.sActivity;
        AlarmManager am = (AlarmManager)currentActivity.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(currentActivity, UnityNotificationManager.class);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(currentActivity, id, intent, 0);
        am.cancel(pendingIntent);
    }

    public static void CancelAll(){
    	Activity currentActivity = AppActivity.sActivity;
        NotificationManager notificationManager = (NotificationManager)currentActivity.getApplicationContext().getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancelAll();
    }
    
}
