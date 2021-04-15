// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "iOS/PrFirebase_iOS.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE && PLATFORM_IOS
#include "iOS/PrFirebaseAuthModule_iOS.h"
#include "iOS/PrFirebaseCrashlyticsModule_iOS.h"
#include "iOS/PrFirebaseAnalyticsModule_iOS.h"
#include "iOS/PrFirebasePerformanceModule_iOS.h"
#include "iOS/PrFirebaseRemoteConfigModule_iOS.h"

#import <Firebase.h>

void UPrFirebase_iOS::InitializeModuleList()
{
	ModuleClasses.Add(UPrFirebaseRemoteConfigModule::StaticClass(), UPrFirebaseRemoteConfigModule_iOS::StaticClass());
	ModuleClasses.Add(UPrFirebaseCrashlyticsModule::StaticClass(), UPrFirebaseCrashlyticsModule_iOS::StaticClass());
	ModuleClasses.Add(UPrFirebaseAnalyticsModule::StaticClass(), UPrFirebaseAnalyticsModule_iOS::StaticClass());
	ModuleClasses.Add(UPrFirebasePerformanceModule::StaticClass(), UPrFirebasePerformanceModule_iOS::StaticClass());
	ModuleClasses.Add(UPrFirebaseAuthModule::StaticClass(), UPrFirebaseAuthModule_iOS::StaticClass());
}

void UPrFirebase_iOS::Initialize()
{
	dispatch_async(dispatch_get_main_queue(), ^{
	  PreInitializeModules_AnyThread();
	  [FIRApp configure];
	  InitializeModules_AnyThread();
	});
}

#endif // WITH_FIREBASE && PLATFORM_IOS
