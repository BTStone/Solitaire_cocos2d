package magic.sunflower.card.solitaire.free;

import android.app.Activity;
import android.widget.Toast;
import android.util.Log;

abstract class STRunnable implements Runnable {
	Object mParam;
	Object mParam2; 
	Object mParam3; 
	public void setParam(Object param) {
		this.mParam = param; 
	}
	public void setParam2(Object param) {
		this.mParam2 = param; 
	}
	public void setParam3(Object param) {
		this.mParam3 = param; 
	}
}; 

public class STComm {
	private static STComm sSTComm;
	private Activity mActivity; 
	public static String sLogTag = "Solitaire"; 
	
	private static boolean sIsDebugMode = false; 
	
	public STComm(Activity activity) {
		mActivity = activity; 
		sSTComm = this; 
		
		//printHashKey(); 
		//Log.d(sLogTag, getDeviceInfo()); 
	}
	
	public static void debugLog(String info) {
		if (sIsDebugMode) {
			Log.d(sLogTag, info); 
    	}
		
	}
	
	public static void debugToast(String message) {
    	if (sIsDebugMode) {
    		toast(message); 
    	}
    }
	
	public static void toast(String message) {
		STRunnable runnable = new STRunnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				String strMessage = (String)(this.mParam); 
				Toast.makeText(sSTComm.mActivity, strMessage, Toast.LENGTH_SHORT).show(); 
			}
		}; 
		runnable.setParam(message); 
		sSTComm.mActivity.runOnUiThread(runnable); 
    }
}