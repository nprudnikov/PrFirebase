// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#pragma once

#include "PrFirebaseAnalyticsModule.h"

#include "PrFirebaseAnalyticsModule_iOS.generated.h"

UCLASS()
class UPrFirebaseAnalyticsModule_iOS : public UPrFirebaseAnalyticsModule
{
	GENERATED_BODY()

#if WITH_FIREBASE && PLATFORM_IOS

public:
	virtual void LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) override;

	virtual void LogScreenView(FString ScreenName, FString ScreenClass) override;

	virtual void LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value) override;

	virtual void LogLevelUp(FString Character, int Level) override;

	virtual void LogLevelStart(FString LevelName) override;

	virtual void LogLevelEnd(FString LevelName, FString Success) override;

	virtual void LogPurchase(FPrFirebasePurchaseData PurchaseData) override;

	virtual void SetDefaultEventParameters(TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) override;

	virtual void LogImpression(FPrFirebaseImpressionData ImpressionData) override;

	virtual void RequestAppInstanceId() override;

	virtual FString GetAppInstanceId() override;

	virtual void LogRevenue(float RevenueUSD) override;

#endif
};
