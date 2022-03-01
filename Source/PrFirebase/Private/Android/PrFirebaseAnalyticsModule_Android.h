// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#pragma once

#include "PrFirebaseAnalyticsModule.h"

#include "PrFirebaseAnalyticsModule_Android.generated.h"

UCLASS()
class UPrFirebaseAnalyticsModule_Android : public UPrFirebaseAnalyticsModule
{
	GENERATED_BODY()

#if WITH_FIREBASE && PLATFORM_ANDROID

public:
	virtual void LogImpression(FPrFirebaseImpressionData ImpressionData) override;

	virtual void RequestAppInstanceId() override;

	virtual FString GetAppInstanceId() override;

#endif // WITH_FIREBASE && PLATFORM_ANDROID
};
