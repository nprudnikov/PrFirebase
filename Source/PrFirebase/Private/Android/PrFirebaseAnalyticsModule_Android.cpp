// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#include "Android/PrFirebaseAnalyticsModule_Android.h"

#if WITH_FIREBASE && PLATFORM_ANDROID
#include "PrFirebaseDefines.h"
#include "PrFirebaseLibrary.h"
#include "PrFirebaseSettings.h"

#include "Async/Async.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

void UPrFirebaseAnalyticsModule_Android::LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogEvent", "(Ljava/lang/String;Landroid/os/Bundle;)V", false);

		jclass BundleClass = Env->FindClass("android/os/Bundle");
		jmethodID BundleInit = Env->GetMethodID(BundleClass, "<init>", "()V");
		jmethodID BundlePutString = Env->GetMethodID(BundleClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID BundlePutDouble = Env->GetMethodID(BundleClass, "putDouble", "(Ljava/lang/String;D)V");

		jobject BundleObject = Env->NewGlobalRef(Env->NewObject(BundleClass, BundleInit));

		for (const TPair<FString, FString>& ParamPair : StringParams)
		{
			jstring ParamKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jstring ParamVal = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Value));

			Env->CallVoidMethod(BundleObject, BundlePutString, ParamKey, ParamVal);

			Env->DeleteLocalRef(ParamKey);
			Env->DeleteLocalRef(ParamVal);
		}

		for (const TPair<FString, float>& ParamPair : FloatParams)
		{
			jstring ParamKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));

			Env->CallVoidMethod(BundleObject, BundlePutDouble, ParamKey, ParamPair.Value);

			Env->DeleteLocalRef(ParamKey);
		}

		jstring EventNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*EventName));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, EventNameParam, BundleObject);

		Env->DeleteLocalRef(EventNameParam);
		Env->DeleteLocalRef(BundleClass);
		Env->DeleteGlobalRef(BundleObject);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogScreenView(FString ScreenName, FString ScreenClass)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogScreenView", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring ScreenNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ScreenName));
		jstring ScreenClassParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ScreenClass));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, ScreenNameParam, ScreenClassParam);

		Env->DeleteLocalRef(ScreenNameParam);
		Env->DeleteLocalRef(ScreenClassParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogSpendVirtualCurrency", "(Ljava/lang/String;Ljava/lang/String;F)V", false);

		jstring ItemNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*ItemName));
		jstring CurrencyNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*CurrencyName));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, ItemNameParam, CurrencyNameParam, Value);

		Env->DeleteLocalRef(ItemNameParam);
		Env->DeleteLocalRef(CurrencyNameParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelUp(FString Character, int Level)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogLevelUp", "(Ljava/lang/String;J)V", false);

		jstring CharacterParam = Env->NewStringUTF(TCHAR_TO_UTF8(*Character));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, CharacterParam, Level);

		Env->DeleteLocalRef(CharacterParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelStart(FString LevelName)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogLevelStart", "(Ljava/lang/String;)V", false);

		jstring LevelNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*LevelName));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, LevelNameParam);

		Env->DeleteLocalRef(LevelNameParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogLevelEnd(FString LevelName, FString Success)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogLevelEnd", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring LevelNameParam = Env->NewStringUTF(TCHAR_TO_UTF8(*LevelName));
		jstring SuccessParam = Env->NewStringUTF(TCHAR_TO_UTF8(*Success));

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, LevelNameParam, SuccessParam);

		Env->DeleteLocalRef(LevelNameParam);
		Env->DeleteLocalRef(SuccessParam);
	}
}

void UPrFirebaseAnalyticsModule_Android::SetDefaultEventParameters(TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_SetDefaultEventParameters", "(Landroid/os/Bundle;)V", false);

		jclass BundleClass = Env->FindClass("android/os/Bundle");
		jmethodID BundleInit = Env->GetMethodID(BundleClass, "<init>", "()V");
		jmethodID BundlePutString = Env->GetMethodID(BundleClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID BundlePutDouble = Env->GetMethodID(BundleClass, "putDouble", "(Ljava/lang/String;D)V");

		jobject BundleObject = Env->NewGlobalRef(Env->NewObject(BundleClass, BundleInit));

		for (const TPair<FString, FString>& ParamPair : StringParams)
		{
			jstring ParamKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jstring ParamVal = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Value));

			Env->CallVoidMethod(BundleObject, BundlePutString, ParamKey, ParamVal);

			Env->DeleteLocalRef(ParamKey);
			Env->DeleteLocalRef(ParamVal);
		}

		for (const TPair<FString, float>& ParamPair : FloatParams)
		{
			jstring ParamKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));

			Env->CallVoidMethod(BundleObject, BundlePutDouble, ParamKey, ParamPair.Value);

			Env->DeleteLocalRef(ParamKey);
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, BundleObject);

		Env->DeleteLocalRef(BundleClass);
		Env->DeleteGlobalRef(BundleObject);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogPurchase(FPrFirebasePurchaseData PurchaseData)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass AnalyticsParamClass = AndroidJavaEnv::FindJavaClass("com/google/firebase/analytics/FirebaseAnalytics$Param");

		jclass BundleClass = Env->FindClass("android/os/Bundle");
		jmethodID BundleInit = Env->GetMethodID(BundleClass, "<init>", "()V");
		jmethodID BundlePutString = Env->GetMethodID(BundleClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID BundlePutDouble = Env->GetMethodID(BundleClass, "putDouble", "(Ljava/lang/String;D)V");

		jobject BundleObject = Env->NewGlobalRef(Env->NewObject(BundleClass, BundleInit));

		jfieldID CurrencyField = Env->GetStaticFieldID(AnalyticsParamClass, "CURRENCY", "Ljava/lang/String;");
		jstring CurrencyParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, CurrencyField));
		jstring CurrencyString = Env->NewStringUTF(TCHAR_TO_UTF8(*PurchaseData.TransactionID));
		Env->CallVoidMethod(BundleObject, BundlePutString, CurrencyParamName, CurrencyString);
		Env->DeleteLocalRef(CurrencyParamName);
		Env->DeleteLocalRef(CurrencyString);

		jfieldID TransactionIDField = Env->GetStaticFieldID(AnalyticsParamClass, "TRANSACTION_ID", "Ljava/lang/String;");
		jstring TransactionIDParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, TransactionIDField));
		jstring TransactionIDString = Env->NewStringUTF(TCHAR_TO_UTF8(*PurchaseData.TransactionID));
		Env->CallVoidMethod(BundleObject, BundlePutString, TransactionIDParamName, TransactionIDString);
		Env->DeleteLocalRef(TransactionIDParamName);
		Env->DeleteLocalRef(TransactionIDString);

		jfieldID ValueField = Env->GetStaticFieldID(AnalyticsParamClass, "VALUE", "Ljava/lang/String;");
		jstring ValueParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, ValueField));
		Env->CallVoidMethod(BundleObject, BundlePutDouble, ValueParamName, PurchaseData.Value);
		Env->DeleteLocalRef(ValueParamName);

		for (const TPair<FString, FString>& ParamPair : PurchaseData.CustomData)
		{
			jstring ParamKey = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Key));
			jstring ParamVal = Env->NewStringUTF(TCHAR_TO_UTF8(*ParamPair.Value));

			Env->CallVoidMethod(BundleObject, BundlePutString, ParamKey, ParamVal);

			Env->DeleteLocalRef(ParamKey);
			Env->DeleteLocalRef(ParamVal);
		}

		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogPurchase", "(Landroid/os/Bundle;)V", false);

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, BundleObject);

		Env->DeleteLocalRef(BundleClass);
		Env->DeleteGlobalRef(BundleObject);
	}
}

void UPrFirebaseAnalyticsModule_Android::LogImpression(FPrFirebaseImpressionData ImpressionData)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		jclass AnalyticsParamClass = AndroidJavaEnv::FindJavaClass("com/google/firebase/analytics/FirebaseAnalytics$Param");

		jfieldID AdPlatformField = Env->GetStaticFieldID(AnalyticsParamClass, "AD_PLATFORM", "Ljava/lang/String;");
		jstring AdPlatformParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, AdPlatformField));

		jfieldID AdSourceField = Env->GetStaticFieldID(AnalyticsParamClass, "AD_SOURCE", "Ljava/lang/String;");
		jstring AdSourceParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, AdSourceField));

		jfieldID AdFormatField = Env->GetStaticFieldID(AnalyticsParamClass, "AD_FORMAT", "Ljava/lang/String;");
		jstring AdFormatParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, AdFormatField));

		jfieldID AdUnitField = Env->GetStaticFieldID(AnalyticsParamClass, "AD_UNIT_NAME", "Ljava/lang/String;");
		jstring AdUnitParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, AdUnitField));

		jfieldID CurrencyField = Env->GetStaticFieldID(AnalyticsParamClass, "CURRENCY", "Ljava/lang/String;");
		jstring CurrencyParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, CurrencyField));

		jfieldID ValueField = Env->GetStaticFieldID(AnalyticsParamClass, "VALUE", "Ljava/lang/String;");
		jstring ValueParamName = reinterpret_cast<jstring>(Env->GetStaticObjectField(AnalyticsParamClass, ValueField));

		Env->DeleteLocalRef(AnalyticsParamClass);

		jclass BundleClass = Env->FindClass("android/os/Bundle");
		jmethodID BundleInit = Env->GetMethodID(BundleClass, "<init>", "()V");
		jmethodID BundlePutString = Env->GetMethodID(BundleClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID BundlePutDouble = Env->GetMethodID(BundleClass, "putDouble", "(Ljava/lang/String;D)V");

		jobject BundleObject = Env->NewGlobalRef(Env->NewObject(BundleClass, BundleInit));

		jstring AdPlatformJavaString = Env->NewStringUTF(TCHAR_TO_UTF8(TEXT("ironSource")));
		Env->CallVoidMethod(BundleObject, BundlePutString, AdPlatformParamName, AdPlatformJavaString);
		Env->DeleteLocalRef(AdPlatformJavaString);
		Env->DeleteLocalRef(AdPlatformParamName);

		jstring AdNetworkJavaString = Env->NewStringUTF(TCHAR_TO_UTF8(*ImpressionData.AdNetwork));
		Env->CallVoidMethod(BundleObject, BundlePutString, AdSourceParamName, AdNetworkJavaString);
		Env->DeleteLocalRef(AdNetworkJavaString);
		Env->DeleteLocalRef(AdSourceParamName);

		jstring AdUnitJavaString = Env->NewStringUTF(TCHAR_TO_UTF8(*ImpressionData.AdUnit));
		Env->CallVoidMethod(BundleObject, BundlePutString, AdFormatParamName, AdUnitJavaString);
		Env->DeleteLocalRef(AdUnitJavaString);
		Env->DeleteLocalRef(AdFormatParamName);

		jstring AdUnitNameJavaString = Env->NewStringUTF(TCHAR_TO_UTF8(*ImpressionData.InstanceName));
		Env->CallVoidMethod(BundleObject, BundlePutString, AdUnitParamName, AdUnitNameJavaString);
		Env->DeleteLocalRef(AdUnitNameJavaString);
		Env->DeleteLocalRef(AdUnitParamName);

		jstring CurrencyJavaString = Env->NewStringUTF(TCHAR_TO_UTF8(TEXT("USD")));
		Env->CallVoidMethod(BundleObject, BundlePutString, CurrencyParamName, CurrencyJavaString);
		Env->DeleteLocalRef(CurrencyJavaString);
		Env->DeleteLocalRef(CurrencyParamName);

		Env->CallVoidMethod(BundleObject, BundlePutDouble, ValueParamName, ImpressionData.Revenue);
		Env->DeleteLocalRef(ValueParamName);

		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogImpression", "(Landroid/os/Bundle;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, BundleObject);

		Env->DeleteLocalRef(BundleClass);
		Env->DeleteGlobalRef(BundleObject);
	}
}

void UPrFirebaseAnalyticsModule_Android::RequestAppInstanceId()
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_RequestAppInstanceId", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
}

FString UPrFirebaseAnalyticsModule_Android::GetAppInstanceId()
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_GetAppInstanceId", "()Ljava/lang/String;", false);
		jstring jAppInstanceId = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method);
		return FJavaHelper::FStringFromLocalRef(Env, jAppInstanceId);
	}
	else
	{
		return FString{};
	}
}

void UPrFirebaseAnalyticsModule_Android::LogRevenue(float RevenueUSD)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_LogRevenue", "(F)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, RevenueUSD);
	}
}

extern "C" {
JNIEXPORT void Java_com_pr_firebase_analytics_PrFirebaseAnalytics_OnAppInstanceIdReady(JNIEnv* env, jobject obj)
{
	AsyncTask(ENamedThreads::GameThread, []() {
		const auto AnalyticsModule = UPrFirebaseLibrary::GetFirebaseProxy()->GetAnalyticsModule();
		if (AnalyticsModule != nullptr)
		{
			AnalyticsModule->AppInstanceIdReadyDelegate.Broadcast();
		}
	});
}
}

#endif // WITH_FIREBASE && PLATFORM_ANDROID
