// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseRemoteConfigModule.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPrFirebaseRemoteConfigFetchingDelegate);

UCLASS()
class PRFIREBASE_API UPrFirebaseRemoteConfigModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Value")
	virtual bool HasValue(const FString& Key) const
	{
		Firebase_NotImplemented();
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Value")
	virtual void GetStringValue(const FString& Key, FString& OutValue) const { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Value")
	virtual void GetIntValue(const FString& Key, int32& OutValue) const { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Value")
	virtual void GetFloatValue(const FString& Key, float& OutValue) const { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Value")
	virtual void GetBoolValue(const FString& Key, bool& OutValue) const { Firebase_NotImplemented(); }

	UPROPERTY(BlueprintAssignable, Category = "Firebase|RemoteConfig|Fetch")
	FPrFirebaseRemoteConfigFetchingDelegate FetchingDelegate;

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Fetch")
	virtual void Fetch() { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Fetch")
	virtual bool IsFetched() const
	{
		Firebase_NotImplemented();
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Firebase|RemoteConfig|Fetch")
	virtual int32 GetFetchCounter() const
	{
		Firebase_NotImplemented();
		return 0;
	}

	virtual bool IsAvailable() const final override { return WITH_FIREBASE_REMOTECONFIG; }
};
