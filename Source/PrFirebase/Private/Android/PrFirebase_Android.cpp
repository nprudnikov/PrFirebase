#include "Android/PrFirebase_Android.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE && PLATFORM_ANDROID
#include "Android/PrFirebaseCrashlyticsModule_Android.h"
#include "PrFirebaseAuthModule.h"
#include "PrFirebasePerformanceModule.h"
#include "PrFirebaseRemoteConfigModule.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

void UPrFirebase_Android::InitializeModuleList()
{
	ModuleClasses.Add(UPrFirebaseRemoteConfigModule::StaticClass(), UPrFirebaseRemoteConfigModule::StaticClass());
	ModuleClasses.Add(UPrFirebaseCrashlyticsModule::StaticClass(), UPrFirebaseCrashlyticsModule_Android::StaticClass());
	ModuleClasses.Add(UPrFirebasePerformanceModule::StaticClass(), UPrFirebasePerformanceModule::StaticClass());
	ModuleClasses.Add(UPrFirebaseAuthModule::StaticClass(), UPrFirebaseAuthModule::StaticClass());
}

void UPrFirebase_Android::Initialize()
{
	PreInitializeModules_AnyThread();

	if (auto Env = FAndroidApplication::GetJavaEnv())
	{
		static auto Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Firebase_Initialize", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}

	InitializeModules_AnyThread();
}

#endif // WITH_FIREBASE && PLATFORM_ANDROID
