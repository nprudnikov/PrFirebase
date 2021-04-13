// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"
#include "PrFirebaseDefines.h"
#include "PrFirebaseAnalyticsModule.generated.h"

UCLASS()
class PRFIREBASE_API UPrFirebaseAnalyticsModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Firebase|Analytics")
	virtual void LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams) { Firebase_NotImplemented(); }

	virtual bool IsAvailable() const final override { return WITH_FIREBASE_ANALYTICS; }

protected:

	
};
