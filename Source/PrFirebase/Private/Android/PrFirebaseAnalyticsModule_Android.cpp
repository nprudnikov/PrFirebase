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
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseAnalytics_TrackEvent", "(Ljava/lang/String;Ljava/util/HashMap;Ljava/util/HashMap;)V", false);

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

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, EventNameParam, StringParamsHash, FloatParamsHash);

		Env->DeleteLocalRef(EventNameParam);
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
