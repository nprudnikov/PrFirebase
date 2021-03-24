// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebasePerformanceModule.h"

#include "PrFirebasePerformanceModule_Android.generated.h"

UCLASS()
class UPrFirebasePerformanceModule_Android : public UPrFirebasePerformanceModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_PERFORMANCE && PLATFORM_ANDROID
protected:
	virtual void InternalStartTrace(int32 TraceIndex, const FString& Identifier) override;

	virtual void InternalStopTrace(int32 TraceIndex) override;

	virtual void InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value) override;

	virtual void InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value) override;

	virtual void InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value) override;

	virtual void InternalRemoveAttribute(int32 TraceIndex, const FString& Name) override;

	virtual void PostInitialize_AnyThread() override;

#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_ANDROID
};
