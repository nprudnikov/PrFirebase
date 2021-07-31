// Copyright 2020-2021 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "PrFirebaseAppDistributionModule.generated.h"

UCLASS()
class PRFIREBASE_API UPrFirebaseAppDistributionModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	virtual bool IsAvailable() const final override { return WITH_FIREBASE_APPDISTRIBUTION; }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Distribution")
	virtual void CheckForUpdate() { Firebase_NotImplemented(); }
};
