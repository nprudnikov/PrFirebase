// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

package com.pr.firebase.analytics;

import com.google.firebase.analytics.FirebaseAnalytics;
import android.app.NativeActivity;
import java.util.HashMap;
import android.os.Bundle;
import android.util.Log;

public class PrFirebaseAnalytics {

	private static FirebaseAnalytics mFirebaseAnalytics;

	static public void Init(NativeActivity activity)
	{
		mFirebaseAnalytics = FirebaseAnalytics.getInstance(activity);
	}

	static public void LogEvent(String Name, HashMap<String, String> StringParams, HashMap<String, Float> FloatParams)
	{			
		Bundle analyticsparams = new Bundle();
		
		for(String key : StringParams.keySet()) {
			analyticsparams.putString(key, StringParams.get(key));
		}
		for(String key : FloatParams.keySet()) {
			analyticsparams.putFloat(key, FloatParams.get(key));
		}
		
		mFirebaseAnalytics.logEvent(Name, analyticsparams);
	}
	
	static public void LogScreenView(String ScreenName, String ScreenClass)
	{	
		Bundle analyticsparams = new Bundle();
		analyticsparams.putString(FirebaseAnalytics.Param.SCREEN_CLASS, ScreenClass);
		analyticsparams.putString(FirebaseAnalytics.Param.SCREEN_NAME, ScreenName);
		
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SCREEN_VIEW, analyticsparams);
	}
	
	static public void LogSpendVirtualCurrency(String ItemName, String CurrencyName, float Value)
	{	
		Bundle analyticsparams = new Bundle();
		analyticsparams.putString(FirebaseAnalytics.Param.ITEM_NAME, ItemName);
		analyticsparams.putString(FirebaseAnalytics.Param.VIRTUAL_CURRENCY_NAME, CurrencyName);
		analyticsparams.putFloat(FirebaseAnalytics.Param.VALUE, Value);
		
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.SPEND_VIRTUAL_CURRENCY, analyticsparams);
	}
	
	static public void LogLevelUp(String Character, long Level)
	{	
		Bundle analyticsparams = new Bundle();
		analyticsparams.putFloat(FirebaseAnalytics.Param.LEVEL, Level);
		analyticsparams.putString(FirebaseAnalytics.Param.CHARACTER, Character);
					
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_UP, analyticsparams);
	}
	
	static public void LogLevelStart(String LevelName)
	{	
		Bundle analyticsparams = new Bundle();
		analyticsparams.putString(FirebaseAnalytics.Param.LEVEL_NAME, LevelName);
					
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_START, analyticsparams);
	}
	
	static public void LogLevelEnd(String LevelName, String Success)
	{	
		Bundle analyticsparams = new Bundle();
		analyticsparams.putString(FirebaseAnalytics.Param.LEVEL_NAME, LevelName);
		analyticsparams.putString(FirebaseAnalytics.Param.SUCCESS, Success);
		
		mFirebaseAnalytics.logEvent(FirebaseAnalytics.Event.LEVEL_END, analyticsparams);
	}
	
	static public void SetDefaultEventParameters(HashMap<String, String> StringParams, HashMap<String, Float> FloatParams)
	{			
		Bundle analyticsparams = new Bundle();
		
		for(String key : StringParams.keySet()) {
			analyticsparams.putString(key, StringParams.get(key));
		}
		for(String key : FloatParams.keySet()) {
			analyticsparams.putFloat(key, FloatParams.get(key));
		}
		
		mFirebaseAnalytics.setDefaultEventParameters(analyticsparams);	
	}

}
