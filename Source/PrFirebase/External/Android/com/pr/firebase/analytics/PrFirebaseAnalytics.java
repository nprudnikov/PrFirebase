// Copyright 2021-2023 Anton Rassadin. All Rights Reserved.

package com.pr.firebase.analytics;

import java.util.HashMap;

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

	public void requestAppInstanceId()
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

	public String getAppInstanceId()
	{
		return mAppInstanceId;
	}

	public void logRevenue(float RevenueUSD)
	{
		Bundle bundle = new Bundle();
		bundle.putString(FirebaseAnalytics.Param.CURRENCY, "USD");
		bundle.putFloat(FirebaseAnalytics.Param.VALUE, RevenueUSD);
		mFirebaseAnalytics.logEvent("total_revenue", bundle);
	}
    
    public void logEvent(String Name, Bundle params)
    {
        mFirebaseAnalytics.logEvent(Name, params);
    }
    
    public void logScreenView(String ScreenName, String ScreenClass)
    {	
        Bundle analyticsparams = new Bundle();
        analyticsparams.putString(FirebaseAnalytics.Param.SCREEN_CLASS, ScreenClass);
        analyticsparams.putString(FirebaseAnalytics.Param.SCREEN_NAME, ScreenName);
        
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SCREEN_VIEW, analyticsparams);
    }
    
    public void logSpendVirtualCurrency(String ItemName, String CurrencyName, float Value)
    {	
        Bundle analyticsparams = new Bundle();
        analyticsparams.putString(FirebaseAnalytics.Param.ITEM_NAME, ItemName);
        analyticsparams.putString(FirebaseAnalytics.Param.VIRTUAL_CURRENCY_NAME, CurrencyName);
        analyticsparams.putFloat(FirebaseAnalytics.Param.VALUE, Value);
        
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SPEND_VIRTUAL_CURRENCY, analyticsparams);
    }
    
    public void logLevelUp(String Character, long Level)
    {	
        Bundle analyticsparams = new Bundle();
        analyticsparams.putLong(FirebaseAnalytics.Param.LEVEL, Level);
        analyticsparams.putString(FirebaseAnalytics.Param.CHARACTER, Character);
                    
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_UP, analyticsparams);
    }
    
    public void logLevelStart(String LevelName)
    {	
        Bundle analyticsparams = new Bundle();
        analyticsparams.putString(FirebaseAnalytics.Param.LEVEL_NAME, LevelName);
                    
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_START, analyticsparams);
    }
    
    public void logLevelEnd(String LevelName, String Success)
    {	
        Bundle analyticsparams = new Bundle();
        analyticsparams.putString(FirebaseAnalytics.Param.LEVEL_NAME, LevelName);
        analyticsparams.putString(FirebaseAnalytics.Param.SUCCESS, Success);
        
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_END, analyticsparams);
    }
    
    public void logPurchase(Bundle purchaseData)
    {
        mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.PURCHASE, purchaseData);
    }
    
    public void setDefaultEventParameters(Bundle params)
    {
        mFirebaseAnalytics.setDefaultEventParameters(params);	
    }
    
}
