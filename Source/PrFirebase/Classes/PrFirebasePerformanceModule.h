// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "Misc/CoreDelegates.h"

#include "PrFirebasePerformanceModule.generated.h"

class UPrFirebasePerformanceModule;

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseSyncAccum

struct FPrFirebaseSyncAccum
{
	int64 FrameCounter;
	float GameThreadTimeMs;
	float RenderThreadTimeMs;
	float RHIThreadTimeMs;

	FPrFirebaseSyncAccum();
};

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseTimelineDelta

struct FPrFirebaseTimelineDelta
{
	int32 Value;
	float Seconds;
};

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseTimeline

struct FPrFirebaseTimeline
{
	FPrFirebaseTimeline();

	void Add(int32 Value, const FDateTime& Time);

	bool IsChanged() const;

	bool HasValue() const;

	int32 GetCount() const;

	FPrFirebaseTimelineDelta GetDelta() const;

	int32 GetValue() const;

	void Reset(bool bFully = false);

private:
	TArray<TTuple<int32, FDateTime>> Timeline;
	int32 Count;
};

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseDeviceTimelines

struct FPrFirebaseDeviceTimelines
{
	FPrFirebaseTimeline BatteryLevel;
	FPrFirebaseTimeline VolumeLevel;
	FPrFirebaseTimeline HeatingLevel;

	FPrFirebaseDeviceTimelines();

	void Reset(bool bFully = false);
};

//////////////////////////////////////////////////////////////////////////
// FPrFirebasePerformanceTrace

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

//////////////////////////////////////////////////////////////////////////
// FPrFirebasePerformanceScopeTimeTrace

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

//////////////////////////////////////////////////////////////////////////
// UPrFirebasePerformanceModule

UCLASS()
class PRFIREBASE_API UPrFirebasePerformanceModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	UPrFirebasePerformanceModule();

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

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Frame")
	void SetFrameTraceCategory(const FString& Key, const FString& Category);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Frame")
	void ResetFrameTraceCategory(const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Performance|Utils")
	void AppliactionLaunched();

	virtual bool IsAvailable() const final override { return WITH_FIREBASE_PERFORMANCE; }

protected:
	void UpdatePostfix();

	virtual void InternalStartTrace(int32 TraceIndex, const FString& Identifier) { Firebase_NotImplemented(); }

	virtual void InternalStopTrace(int32 TraceIndex) { Firebase_NotImplemented(); }

	virtual void InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value) { Firebase_NotImplemented(); }

	virtual void InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value) { Firebase_NotImplemented(); }

	virtual void InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value) { Firebase_NotImplemented(); }

	virtual void InternalRemoveAttribute(int32 TraceIndex, const FString& Name) { Firebase_NotImplemented(); }

	void InternalLaunch_AnyThread();

	void StartWatch();

private:
	int32 LastTraceIndex;

	bool bAppliactionLaunched;

	bool bAppliactionFirstFrame;

	TSet<int32> ExistingTraceIndices;

	TMap<FString, FString> FrameTraceCategories;

	FString FramePostfix;

	TOptional<FPrFirebasePerformanceTrace> AppLaunchTrace;

	TOptional<FDateTime> SyncTime;

	FPrFirebaseSyncAccum AvFrameAccum;

	FPrFirebaseDeviceTimelines DeviceTimelines;

	FString DeviceProfileName;

	FString DeviceChipset;

	FString DeviceBrand;

	void OnAppliactionLaunched();

	void OnEndFrame();

	void UpdateDeviceTimelines(const FDateTime& Now, bool bSync);

	void UpdateTraceAttributes(FPrFirebasePerformanceTrace& Trace) const;

	int32 GetTemperature() const;
};

#define PRF_SCOPE_TIME(_Identifier_) \
	FPrFirebasePerformanceScopeTimeTrace _PrFirebasePerformanceScopeTimeTrace_##_TraceName_##__LINE__(GPrFirebasePerformanceModule, #_Identifier_);

extern PRFIREBASE_API TWeakObjectPtr<UPrFirebasePerformanceModule> GPrFirebasePerformanceModule;
