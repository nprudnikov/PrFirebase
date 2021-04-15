// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#include "Android/PrFirebaseAnalyticsModule_Android.h"
#include "PrFirebaseDefines.h"

#if WITH_FIREBASE_ANALYTICS && PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

void UPrFirebaseAnalyticsModule_Android::LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogEvent", "(Ljava/lang/String;Ljava/util/HashMap;Ljava/util/HashMap;)V", false);

		jclass mapClass = Env->FindClass("java/util/HashMap");
		jmethodID init = Env->GetMethodID(mapClass, "<init>", "(I)V");
		jmethodID HashPut = Env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		
		jobject StringParamsHash = Env->NewObject(mapClass, init, StringParams.Num());
		for(const TPair<FString, FString>& ParamPair : StringParams)
		{
			jstring paramKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jstring paramVal = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Value));

			Env->CallObjectMethod(StringParamsHash, HashPut, paramKey, paramVal);

			Env->DeleteLocalRef(paramKey);
			Env->DeleteLocalRef(paramVal);
		}
		
		jobject FloatParamsHash = Env->NewObject(mapClass, init, FloatParams.Num());
		jclass floatClass = Env->FindClass("java/lang/Float");
		jmethodID floatInit = Env->GetMethodID(floatClass, "<init>", "(F)V");
		for(const TPair<FString, float>& ParamPair : FloatParams)
		{
			jstring paramKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jobject paramValue = Env->NewObject(floatClass, floatInit, ParamPair.Value);
			
			Env->CallObjectMethod(FloatParamsHash, HashPut, paramKey, paramValue);
			
			Env->DeleteLocalRef(paramKey);
			Env->DeleteLocalRef(paramValue);
		}
		
		jstring EventNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*EventName));

		Env->CallStaticVoidMethod(Class, Method, EventNameParam, StringParamsHash, FloatParamsHash);

		Env->DeleteLocalRef(EventNameParam);
		Env->DeleteLocalRef(mapClass);
		Env->DeleteLocalRef(floatClass);
		
		Env->DeleteLocalRef(StringParamsHash);
		Env->DeleteLocalRef(FloatParamsHash);

	}
}

void UPrFirebaseAnalyticsModule_Android::LogScreenView(FString ScreenName, FString ScreenClass)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogScreenView", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring ScreenNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ScreenName));
		jstring ScreenClassParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ScreenClass));

		Env->CallStaticVoidMethod(Class, Method, ScreenNameParam, ScreenClassParam);

		Env->DeleteLocalRef(ScreenNameParam);
		Env->DeleteLocalRef(ScreenClassParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogSpendVirtualCurrency", "(Ljava/lang/String;Ljava/lang/String;F;)V", false);

		jstring ItemNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ItemName));
		jstring CurrencyNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*CurrencyName));

		Env->CallStaticVoidMethod(Class, Method, ItemNameParam, CurrencyNameParam, Value);

		Env->DeleteLocalRef(ItemNameParam);
		Env->DeleteLocalRef(CurrencyNameParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelUp(FString Character, int Level)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogLevelUp", "(Ljava/lang/String;J;)V", false);

		jstring CharacterParam = Env->NewStringUTF(TCHAR_TO_UTF8(*Character));

		Env->CallStaticVoidMethod(Class, Method, CharacterParam, Level);

		Env->DeleteLocalRef(CharacterParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelStart(FString LevelName)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogLevelStart", "(Ljava/lang/String;)V", false);

		jstring LevelNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*LevelName));

		Env->CallStaticVoidMethod(Class, Method, LevelNameParam);

		Env->DeleteLocalRef(LevelNameParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelEnd(FString LevelName, FString Success)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "LogLevelEnd", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring LevelNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*LevelName));
		jstring SuccessParam = Env->NewStringUTF(TCHAR_TO_UTF8(*Success));

		Env->CallStaticVoidMethod(Class, Method, LevelNameParam, SuccessParam);

		Env->DeleteLocalRef(LevelNameParam);
		Env->DeleteLocalRef(SuccessParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::SetDefaultEventParameters(TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass Class = FAndroidApplication::FindJavaClassGlobalRef("com/pr/firebase/analytics/PrFirebaseAnalytics");
		static auto Method = FJavaWrapper::FindStaticMethod(Env, Class, "SetDefaultEventParameters", "(Ljava/util/HashMap;Ljava/util/HashMap;)V", false);

		jclass mapClass = Env->FindClass("java/util/HashMap");
		jmethodID init = Env->GetMethodID(mapClass, "<init>", "(I)V");
		jmethodID HashPut = Env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		
		jobject StringParamsHash = Env->NewObject(mapClass, init, StringParams.Num());
		for(const TPair<FString, FString>& ParamPair : StringParams)
		{
			jstring paramKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jstring paramVal = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Value));

			Env->CallObjectMethod(StringParamsHash, HashPut, paramKey, paramVal);

			Env->DeleteLocalRef(paramKey);
			Env->DeleteLocalRef(paramVal);
		}
		
		jobject FloatParamsHash = Env->NewObject(mapClass, init, FloatParams.Num());
		jclass floatClass = Env->FindClass("java/lang/Float");
		jmethodID floatInit = Env->GetMethodID(floatClass, "<init>", "(F)V");
		for(const TPair<FString, float>& ParamPair : FloatParams)
		{
			jstring paramKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jobject paramValue = Env->NewObject(floatClass, floatInit, ParamPair.Value);
			
			Env->CallObjectMethod(FloatParamsHash, HashPut, paramKey, paramValue);
			
			Env->DeleteLocalRef(paramKey);
			Env->DeleteLocalRef(paramValue);
		}

		Env->CallStaticVoidMethod(Class, Method, StringParamsHash, FloatParamsHash);

		Env->DeleteLocalRef(mapClass);
		Env->DeleteLocalRef(floatClass);
		
		Env->DeleteLocalRef(StringParamsHash);
		Env->DeleteLocalRef(FloatParamsHash);

	}
}

void UPrFirebaseAnalyticsModule_Android::Initialize_AnyThread()
{
	UE_LOG(LogTemp, Log, TEXT("Firebase Analytics Init"));
}

#endif // WITH_FIREBASE_ANALYTICS && PLATFORM_ANDROID
