// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseProxy.generated.h"

class UPrFirebaseModule;
class UPrFirebaseCrashlyticsModule;
class UPrFirebaseRemoteConfigModule;
class UPrFirebasePerformanceModule;
class UPrFirebaseAuthModule;
class UPrFirebaseAppDistributionModule;
class UPrFirebaseAnalyticsModule;

UCLASS()
class PRFIREBASE_API UPrFirebaseProxy : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics")
	UPrFirebaseCrashlyticsModule* GetCrashlyticsModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig")
	UPrFirebaseRemoteConfigModule* GetRemoteConfigModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance")
	UPrFirebasePerformanceModule* GetPerformanceModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase|Auth")
	UPrFirebaseAuthModule* GetAuthModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase|Distribution")
	UPrFirebaseAppDistributionModule* GetAppDistributionModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase|Analytics")
	UPrFirebaseAnalyticsModule* GetAnalyticsModule() const;

	UFUNCTION(BlueprintCallable, Category = "Firebase")
	void InitializeFirebase();

	UFUNCTION(BlueprintCallable, Category = "Firebase")
	bool IsInitialized() const { return bInitialized; }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Attributes")
	void SetUserIdentifier(const FString& UserIdentifier);

protected:
	UPROPERTY()
	TMap<UClass*, UClass*> ModuleClasses;

	virtual void InitializeModuleList();

	virtual void Initialize();

	void PreInitializeModules_AnyThread();

	void InitializeModules_AnyThread();

private:
	UPROPERTY()
	TMap<UClass*, UPrFirebaseModule*> Modules;

	bool bInitialized;

	void CreateModules();

	TArray<UClass*> GetSortedModuleClasses();
};
