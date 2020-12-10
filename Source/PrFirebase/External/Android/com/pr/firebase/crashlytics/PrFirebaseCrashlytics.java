// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

package com.pr.firebase.crashlytics;

import com.google.firebase.crashlytics.FirebaseCrashlytics;
import android.app.NativeActivity;

public class PrFirebaseCrashlytics {

	public PrFirebaseCrashlytics(NativeActivity activity)
	{
		// Do nothing
	}

	public void setUserIdentifier(String userIdentifier)
	{
		FirebaseCrashlytics.getInstance().setUserId(userIdentifier);
	}

	public void writeLog(String log)
	{
		FirebaseCrashlytics.getInstance().log(log);
	}

	public void writeException(RuntimeException ex)
	{
		FirebaseCrashlytics.getInstance().recordException(ex);
	}

	public void addAttribute(String key, String value)
	{
		FirebaseCrashlytics.getInstance().setCustomKey(key, value);
	}

}
