// Copyright 2021 Anton Rassadin. All Rights Reserved.

package com.pr.firebase.analytics;

import android.app.NativeActivity;
import android.os.Bundle;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;

public class PrFirebaseAnalytics {

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
					}
				}
			});
	}

	public String GetAppInstanceId()
	{
		return mAppInstanceId;
	}
}
