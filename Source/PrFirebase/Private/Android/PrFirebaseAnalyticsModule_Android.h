// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseAnalyticsModule.h"
#include "PrFirebaseAnalyticsModule_Android.generated.h"

UCLASS()
class UPrFirebaseAnalyticsModule_Android : public UPrFirebaseAnalyticsModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_ANALYTICS && PLATFORM_ANDROID

public:
	
	virtual void LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) override;

	virtual void LogScreenView(FString ScreenName, FString ScreenClass) override;

	virtual void LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value) override;

	virtual void LogLevelUp(FString Character, int Level) override;

	virtual void LogLevelStart(FString LevelName) override;

	virtual void LogLevelEnd(FString LevelName, FString Success) override;

	virtual void SetDefaultEventParameters(TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) override;

protected:
	
	virtual void Initialize_AnyThread() override;

#endif // WITH_FIREBASE_ANALYTICS && PLATFORM_ANDROID
};
