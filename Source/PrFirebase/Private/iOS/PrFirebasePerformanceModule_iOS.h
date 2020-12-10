// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebasePerformanceModule.h"

#include "PrFirebasePerformanceModule_iOS.generated.h"

#if WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
@class FIRTrace;
#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS

UCLASS()
class UPrFirebasePerformanceModule_iOS : public UPrFirebasePerformanceModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
protected:
	TMap<int32, FIRTrace*> Traces;

	virtual void InternalStartTrace(int32 TraceIndex, const FString& Identifier) override;

	virtual void InternalStopTrace(int32 TraceIndex) override;

	virtual void InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value) override;

	virtual void InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value) override;

	virtual void InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value) override;

	virtual void InternalRemoveAttribute(int32 TraceIndex, const FString& Name) override;

	virtual void PostInitialize_AnyThread() override;

#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
};
