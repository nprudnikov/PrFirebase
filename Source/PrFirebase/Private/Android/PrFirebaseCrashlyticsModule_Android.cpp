#include "Android/PrFirebaseCrashlyticsModule_Android.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE_CRASHLYTICS && PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

void UPrFirebaseCrashlyticsModule_Android::Crash()
{
	UE_LOG(LogFirebase, Fatal, TEXT("Crash"));
}

void UPrFirebaseCrashlyticsModule_Android::SetUserIdentifier(const FString& UserIdentifier)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto IdentifierJava = Env->NewStringUTF(TCHAR_TO_UTF8(*UserIdentifier));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseCrashlytics_SetUserIdentifier", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, IdentifierJava);
		Env->DeleteLocalRef(IdentifierJava);
	}
}

void UPrFirebaseCrashlyticsModule_Android::WriteLog(const FString& Log)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto LogJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Log));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseCrashlytics_WriteLog", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, LogJava);
		Env->DeleteLocalRef(LogJava);
	}
}

void UPrFirebaseCrashlyticsModule_Android::WriteError(const FString& Log, int32 Code)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto LogJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Log));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseCrashlytics_WriteError", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, LogJava);
		Env->DeleteLocalRef(LogJava);
	}
}

void UPrFirebaseCrashlyticsModule_Android::AddAttribute(const FString& Key, const FString& Value)
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		auto KeyJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Key));
		auto ValueJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Value));
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_FirebaseCrashlytics_AddAttribute", "(Ljava/lang/String;Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, KeyJava, ValueJava);
		Env->DeleteLocalRef(ValueJava);
		Env->DeleteLocalRef(KeyJava);
	}
}

void UPrFirebaseCrashlyticsModule_Android::Initialize_AnyThread()
{
	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Firebase_Initialize", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}

	CatchEngineLogs();
}

#endif // WITH_FIREBASE_CRASHLYTICS && PLATFORM_ANDROID
