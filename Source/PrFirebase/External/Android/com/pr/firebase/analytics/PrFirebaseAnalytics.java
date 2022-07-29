// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

package com.pr.firebase.analytics;

import android.app.NativeActivity;
import android.os.Bundle;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;

public class PrFirebaseAnalytics {
	private static native void OnAppInstanceIdReady();

	private FirebaseAnalytics mFirebaseAnalytics;
	private String mAppInstanceId;

	public PrFirebaseAnalytics(NativeActivity activity)
	{
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(activity);
	}

	public void logImpression(Bundle impressionData)
	{
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.AD_IMPRESSION, impressionData);
	}

	public void RequestAppInstanceId()
	{
		Task<String> GetInstanceTask = mFirebaseAnalytics.getAppInstanceId();
		GetInstanceTask.addOnCompleteListener(new OnCompleteListener<String>()
			{
				@Override
				public void onComplete(Task<String> task)
				{
					if (task.isSuccessful())
					{
						mAppInstanceId = task.getResult();
						OnAppInstanceIdReady();
					}
				}
			});
	}

	public String GetAppInstanceId()
	{
		return mAppInstanceId;
	}

	public void LogRevenue(float RevenueUSD)
	{
		Bundle bundle = new Bundle();
		bundle.putFloat(FirebaseAnalytics.Param.VALUE, RevenueUSD);
		mFirebaseAnalytics.logEvent("total_revenue", bundle);
	}
}
