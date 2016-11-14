package magic.sunflower.card.solitaire.free;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.content.Context;
import android.content.res.Configuration; 
import android.os.Bundle;

public class Stone extends Cocos2dxActivity{
	
	public static Context sContext = null;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		sContext = this;
		
	}
	 public boolean isScreenOriatationPortrait() { 
		 return sContext.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT; 
     } 
}
