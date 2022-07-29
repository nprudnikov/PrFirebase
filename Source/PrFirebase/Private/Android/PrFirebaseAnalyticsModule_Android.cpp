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
