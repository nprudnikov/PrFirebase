// Copyright 2021 Anton Rassadin. All Rights Reserved.

package com.pr.firebase.analytics;

import android.app.NativeActivity;
import android.os.Bundle;
import com.google.firebase.analytics.FirebaseAnalytics;

public class PrFirebaseAnalytics {

	private FirebaseAnalytics mFirebaseAnalytics;

	public PrFirebaseAnalytics(NativeActivity activity)
	{
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(activity);
	}

	public void logImpression(Bundle impressionData)
	{
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.AD_IMPRESSION, impressionData);
	}

}
