// Copyright 2020-2021 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "PrFirebaseAppDistributionModule.h"

#include "PrFirebaseAppDistributionModule_iOS.generated.h"

UCLASS()
class UPrFirebaseAppDistributionModule_iOS : public UPrFirebaseAppDistributionModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_APPDISTRIBUTION && PLATFORM_IOS

public:
	virtual void CheckForUpdate() override;

#endif
};
