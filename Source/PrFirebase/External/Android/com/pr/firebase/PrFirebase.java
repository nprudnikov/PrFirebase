package com.pr.firebase;

import com.pr.firebase.*;
import com.pr.firebase.crashlytics.*;
import android.app.NativeActivity;

public class PrFirebase {
	
	private static boolean isInit = false;
	private static PrFirebaseCrashlytics crashlytics = null;

	static public void initialize(NativeActivity activity)
	{
		if (!isInit)
		{
			isInit = true;
			crashlytics = new PrFirebaseCrashlytics(activity);
		}
	}

	static public PrFirebaseCrashlytics crashlyticsProxy()
	{
		return crashlytics;
	}

}
