// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "Android/PrFirebasePerformanceModule_Android.h"

#if WITH_FIREBASE_PERFORMANCE && PLATFORM_ANDROID
#include "PrFirebaseDefines.h"

#include "Async/Async.h"
#include "Misc/CoreDelegates.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

void UPrFirebasePerformanceModule_Android::InternalStartTrace(int32 TraceIndex, const FString& Identifier)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto IdentifierJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Identifier));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_StartTrace", "(ILjava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex, IdentifierJava);
		Env->DeleteLocalRef(IdentifierJava);
	}
}

void UPrFirebasePerformanceModule_Android::InternalStopTrace(int32 TraceIndex)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_StopTrace", "(I)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex);
	}
}

void UPrFirebasePerformanceModule_Android::InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto NameJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_IncrementMetric", "(ILjava/lang/String;I)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex, NameJava, Value);
		Env->DeleteLocalRef(NameJava);
	}
}

void UPrFirebasePerformanceModule_Android::InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto NameJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_SetMetric", "(ILjava/lang/String;I)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex, NameJava, Value);
		Env->DeleteLocalRef(NameJava);
	}
}

void UPrFirebasePerformanceModule_Android::InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto NameJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		auto ValueJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Value));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_SetAttribute", "(ILjava/lang/String;Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex, NameJava, ValueJava);
		Env->DeleteLocalRef(NameJava);
		Env->DeleteLocalRef(ValueJava);
	}
}

void UPrFirebasePerformanceModule_Android::InternalRemoveAttribute(int32 TraceIndex, const FString& Name)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto NameJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebasePerformance_RemoveAttribute", "(ILjava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, TraceIndex, NameJava);
		Env->DeleteLocalRef(NameJava);
	}
}

void UPrFirebasePerformanceModule_Android::PostInitialize_AnyThread()
{
	// TODO: disabe firebase auto stats

	InternalLaunch_AnyThread();
	AsyncTask(ENamedThreads::GameThread, [WeakThis = MakeWeakObjectPtr(this)]() {
		if (WeakThis.IsValid())
		{
			WeakThis->StartWatch();
		}
	});
}

#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_ANDROID
