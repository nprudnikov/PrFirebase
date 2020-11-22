package com.pr.firebase.crashlytics;

import io.fabric.sdk.android.Fabric;
import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import android.app.NativeActivity;

public class PrFirebaseCrashlytics {

	public PrFirebaseCrashlytics(NativeActivity activity)
	{
		final Fabric fabric = new Fabric.Builder(activity)
			.kits(new Crashlytics(), new CrashlyticsNdk())
			.debuggable(true)
			.build();
			
		Fabric.with(fabric);
	}

	public void setUserIdentifier(String userIdentifier)
	{
		Crashlytics.setUserIdentifier(userIdentifier);
	}

	public void writeLog(String log)
	{
		Crashlytics.log(log);
	}

	public void writeException(RuntimeException ex)
	{
		Crashlytics.logException(ex);
	}

	public void addAttribute(String key, String value)
	{
		Crashlytics.setString(key, value);
	}

}