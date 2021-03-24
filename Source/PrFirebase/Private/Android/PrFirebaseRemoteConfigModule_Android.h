// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseRemoteConfigModule.h"

#include "PrFirebaseRemoteConfigModule_Android.generated.h"

UCLASS()
class UPrFirebaseRemoteConfigModule_Android : public UPrFirebaseRemoteConfigModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_REMOTECONFIG && PLATFORM_ANDROID

#endif // WITH_FIREBASE_REMOTECONFIG && PLATFORM_ANDROID
};
