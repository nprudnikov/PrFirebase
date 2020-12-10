// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseRemoteConfigModule.h"

#include "PrFirebaseRemoteConfigModule_iOS.generated.h"

UCLASS()
class UPrFirebaseRemoteConfigModule_iOS : public UPrFirebaseRemoteConfigModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_REMOTECONFIG && PLATFORM_IOS

	UPrFirebaseRemoteConfigModule_iOS();

public:
	virtual bool HasValue(const FString& Key) const override;

	virtual void GetStringValue(const FString& Key, FString& OutValue) const override;

	virtual void GetIntValue(const FString& Key, int32& OutValue) const override;

	virtual void GetFloatValue(const FString& Key, float& OutValue) const override;

	virtual void GetBoolValue(const FString& Key, bool& OutValue) const override;

	virtual void Fetch() override;

	virtual bool IsFetched() const override;

	virtual int32 GetFetchCounter() const override;

protected:
	virtual void Initialize_AnyThread() override;

	virtual void PostInitialize_AnyThread() override;

private:
	bool bFetched;

	bool bFetching;

	int32 FetchCounter;

	void InternalFetch();

	void FetchCompleted(bool bSuccess);

#endif // WITH_FIREBASE_REMOTECONFIG && PLATFORM_IOS
};
