// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "Misc/CoreDelegates.h"

#include "PrFirebasePerformanceModule.generated.h"

class UPrFirebasePerformanceModule;

USTRUCT(Blueprintable, BlueprintType)
struct FPrFirebasePerformanceTrace
{
	GENERATED_BODY()

public:
	FPrFirebasePerformanceTrace();

	FPrFirebasePerformanceTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, const FString& Identifier);

	FPrFirebasePerformanceTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, int32 Index);

	void Stop();

	void IncrementMetric(const FString& Name, int32 Value);

	void SetMetric(const FString& Name, int32 Value);

	void SetAttribute(const FString& Name, const FString& Value);

	void RemoveAttribute(const FString& Name);

	int32 GetTraceIndex() const;

private:
	UPROPERTY()
	TWeakObjectPtr<UPrFirebasePerformanceModule> Module;

	UPROPERTY()
	int32 TraceIndex;
};

USTRUCT()
struct FPrFirebasePerformanceScopeTimeTrace
{
	GENERATED_BODY()

public:
	FPrFirebasePerformanceScopeTimeTrace();

	FPrFirebasePerformanceScopeTimeTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, const FString& Identifier);

	~FPrFirebasePerformanceScopeTimeTrace();

private:
	TOptional<FPrFirebasePerformanceTrace> Trace;
};

UCLASS()
class PRFIREBASE_API UPrFirebasePerformanceModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	UPrFirebasePerformanceModule();

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void SetAttributeForAllTraces(const FString& Name, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void RemoveAttributeForAllTraces(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void SetMetricForAllTraces(const FString& Name, int32 Value);

	int32 StartTraceWithoutWrapper(const FString& Identifier);

	void StopTraceWithoutWrapper(int32 TraceIndex);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	FPrFirebasePerformanceTrace StartTrace(const FString& Identifier);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void StopTrace(const FPrFirebasePerformanceTrace& Trace);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void IncrementMetric(const FPrFirebasePerformanceTrace& Trace, const FString& Name, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void SetMetric(const FPrFirebasePerformanceTrace& Trace, const FString& Name, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void SetAttribute(const FPrFirebasePerformanceTrace& Trace, const FString& Name, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Trace")
	void RemoveAttribute(const FPrFirebasePerformanceTrace& Trace, const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Utils")
	void AppliactionLaunched();

	virtual bool IsAvailable() const final override { return WITH_FIREBASE_PERFORMANCE; }

protected:
	virtual void InternalStartTrace(int32 TraceIndex, const FString& Identifier) { Firebase_NotImplemented(); }

	virtual void InternalStopTrace(int32 TraceIndex) { Firebase_NotImplemented(); }

	virtual void InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value) { Firebase_NotImplemented(); }

	virtual void InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value) { Firebase_NotImplemented(); }

	virtual void InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value) { Firebase_NotImplemented(); }

	virtual void InternalRemoveAttribute(int32 TraceIndex, const FString& Name) { Firebase_NotImplemented(); }

	void InternalLaunch_AnyThread();

	void SetTemperature(FCoreDelegates::ETemperatureSeverity Temp);

	void SetPowerMode(bool bLowPowerMode);

	void StartWatch();

private:
	int32 LastTraceIndex;

	bool bAppliactionLaunched;

	bool bAppliactionFirstFrame;

	TMap<FString, FString> GlobalAttributes;

	TMap<FString, int32> GlobalMetrics;

	TSet<int32> ExistingTraceIndices;

	TOptional<FPrFirebasePerformanceTrace> AppLaunchTrace;

	TOptional<FPrFirebasePerformanceTrace> AvFrameTrace;

	TOptional<FDateTime> AvFrameTime;

	int64 AvFrameCounter;

	void OnAppliactionLaunched();

	void OnEndFrame();
};

#define PRF_SCOPE_TIME(_Identifier_) \
	FPrFirebasePerformanceScopeTimeTrace _PrFirebasePerformanceScopeTimeTrace_##_TraceName_##__LINE__(GPrFirebasePerformanceModule, #_Identifier_);

extern PRFIREBASE_API TWeakObjectPtr<UPrFirebasePerformanceModule> GPrFirebasePerformanceModule;
