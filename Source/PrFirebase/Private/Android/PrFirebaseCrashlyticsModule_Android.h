// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseCrashlyticsModule.h"

#include "PrFirebaseCrashlyticsModule_Android.generated.h"

UCLASS()
class UPrFirebaseCrashlyticsModule_Android : public UPrFirebaseCrashlyticsModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_CRASHLYTICS && PLATFORM_ANDROID

public:
	virtual void Crash() override;

	virtual void SetUserIdentifier(const FString& UserIdentifier) override;

	virtual void WriteLog(const FString& Log) override;

	virtual void WriteError(const FString& Log, int32 Code, const TMap<FString, FString>& UserInfo) override;

	virtual void AddAttribute(const FString& Key, const FString& Value) override;

protected:
	
	virtual void Initialize_AnyThread() override;

#endif // WITH_FIREBASE_CRASHLYTICS && PLATFORM_ANDROID
};
