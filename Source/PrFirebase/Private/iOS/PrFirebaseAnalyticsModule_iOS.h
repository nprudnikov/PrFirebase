// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseAnalyticsModule.h"
#include "PrFirebaseAnalyticsModule_iOS.generated.h"

UCLASS()
class UPrFirebaseAnalyticsModule_iOS : public UPrFirebaseAnalyticsModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_ANALYTICS && PLATFORM_IOS

public:
	
	virtual void LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) override;

protected:
	
	virtual void Initialize_AnyThread() override;

#endif // WITH_FIREBASE_ANALYTICS && PLATFORM_IOS
};
