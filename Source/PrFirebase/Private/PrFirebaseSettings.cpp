// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "PrFirebaseSettings.h"

UPrFirebaseSettings::UPrFirebaseSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bFirebaseEnable(false)
	, bFirebaseLoggingInShipping(false)
	, FirebaseIOSConfig(TEXT("Config/Firebase/GoogleService-Info.plist"))
	, FirebaseAndroidConfig(TEXT("Config/Firebase/google-services.json"))
	, bFirebaseCrashlyticsEnable(false)
	, bFirebaseCrashlytics_SendNotes(true)
	, FirebaseCrashlytics_LogLevel(EPrFirebaseLogLevel::Warning)
	, FirebaseCrashlytics_ErrorLogLevel(EPrFirebaseLogLevel::Fatal)
	, bFirebaseRemoteConfigEnable(false)
	, FirebaseRemoteConfig_ExpirationDurationForFetch(60)
	, bFirebaseAuthEnable(false)
	, bFirebasePerformanceEnable(false)
	, FirebasePerformance_SyncTime(2.f)
	, FirebasePerformance_MemSyncTime(10.f)
{
	FirebaseCrashlytics_ForcedLogCategories.Add("LogCrashlytics");
	FirebaseCrashlytics_ForcedLogCategories.Add("LogPerformance");
}
