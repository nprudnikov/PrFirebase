// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseSettings.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EPrFirebaseLogLevel : uint8
{
	NoLogging = 0,
	Fatal,
	Error,
	Warning,
	Display,
	Log,
	Verbose,
	VeryVerbose,
	All = VeryVerbose,
};

UCLASS(config = Engine, defaultconfig)
class UPrFirebaseSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/* Enable firebase and firebase modules */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase")
	bool bFirebaseEnable;

	/* */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase")
	bool bFirebaseLoggingInShipping;

	/* Path to GoogleService-Info.plist */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase")
	FString FirebaseIOSConfig;

	/* Path to google-services.json */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase")
	FString FirebaseAndroidConfig;

	/* Enable crashlytics module */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Crashlytics")
	bool bFirebaseCrashlyticsEnable;

	/* Send notes on improving crash collection quality */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Crashlytics")
	bool bFirebaseCrashlytics_SendNotes;

	/* You can change this parameter from Firebase Remote Config (use key "pr-log-level" e.g.: pr-log-level: Warning) */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Crashlytics")
	EPrFirebaseLogLevel FirebaseCrashlytics_LogLevel;

	/* You can change this parameter from Firebase Remote Config (use key "pr-error-log-level" e.g.: pr-error-log-level: Fatal) */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Crashlytics")
	EPrFirebaseLogLevel FirebaseCrashlytics_ErrorLogLevel;

	/* You can change this parameter from Firebase Remote Config (use key "pr-forced-log-categories" e.g.: pr-forced-log-categories: LogCrashlytics, LogPerformance, etc.) */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Crashlytics")
	TArray<FName> FirebaseCrashlytics_ForcedLogCategories;

	/* Enable remote config module */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|RemoteConfig")
	bool bFirebaseRemoteConfigEnable;

	/* Expiration duration for fetch */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|RemoteConfig")
	int32 FirebaseRemoteConfig_ExpirationDurationForFetch;

	/* Enable auth module */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Auth")
	bool bFirebaseAuthEnable;

	/* Enable performance module */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Performance")
	bool bFirebasePerformanceEnable;

	/* Time between calculations of the average frame size  */
	UPROPERTY(Config, EditAnywhere, Category = "Firebase|Performance")
	float FirebasePerformance_SyncTime;
};
