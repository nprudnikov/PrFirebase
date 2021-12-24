// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

package com.pr.firebase;

import com.pr.firebase.*;
import com.pr.firebase.crashlytics.*;
import com.pr.firebase.performance.*;
import com.pr.firebase.analytics.*;
import android.app.NativeActivity;

public class PrFirebase {
	
	private static boolean isInit = false;
	private static PrFirebaseCrashlytics crashlytics = null;
	private static PrFirebasePerformance performance = null;
	private static PrFirebaseAnalytics analytics = null;

	static public void initialize(NativeActivity activity)
	{
		if (!isInit)
		{
			isInit = true;
			crashlytics = new PrFirebaseCrashlytics(activity);
			performance = new PrFirebasePerformance(activity);
			analytics = new PrFirebaseAnalytics(activity);
		}
	}

	static public PrFirebaseCrashlytics crashlyticsProxy()
	{
		return crashlytics;
	}

	static public PrFirebasePerformance performanceProxy()
	{
		return performance;
	}

	static public PrFirebaseAnalytics analyticsProxy()
	{
		return analytics;
	}

}
