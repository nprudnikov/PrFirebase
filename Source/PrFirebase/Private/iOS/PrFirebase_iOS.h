// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseProxy.h"

#include "PrFirebase_iOS.generated.h"

UCLASS()
class UPrFirebase_iOS : public UPrFirebaseProxy
{
	GENERATED_BODY()

#if WITH_FIREBASE && PLATFORM_IOS

protected:
	virtual void InitializeModuleList() override;

	virtual void Initialize() override;

#endif // WITH_FIREBASE && PLATFORM_IOS
};
