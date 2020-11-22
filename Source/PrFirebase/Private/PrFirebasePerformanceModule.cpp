#include "PrFirebasePerformanceModule.h"

#include "PrFirebaseCrashlyticsModule.h"
#include "PrFirebaseDefines.h"
#include "PrFirebaseLibrary.h"
#include "PrFirebaseSettings.h"

#include "Async/Async.h"

#define MIN_SYNC_TIME 0.5f

FPrFirebasePerformanceTrace::FPrFirebasePerformanceTrace()
	: TraceIndex(-1)
{
}

FPrFirebasePerformanceTrace::FPrFirebasePerformanceTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, const FString& Identifier)
	: Module(PerformanceModule)
	, TraceIndex(Module.IsValid() ? Module->StartTraceWithoutWrapper(Identifier) : -1)
{
}

FPrFirebasePerformanceTrace::FPrFirebasePerformanceTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, int32 Index)
	: Module(PerformanceModule)
	, TraceIndex(Index)
{
}

void FPrFirebasePerformanceTrace::Stop()
{
	if (Module.IsValid())
	{
		Module->StopTrace(*this);
	}
}

void FPrFirebasePerformanceTrace::IncrementMetric(const FString& Name, int32 Value)
{
	if (Module.IsValid())
	{
		Module->IncrementMetric(*this, Name, Value);
	}
}

void FPrFirebasePerformanceTrace::SetMetric(const FString& Name, int32 Value)
{
	if (Module.IsValid())
	{
		Module->SetMetric(*this, Name, Value);
	}
}

void FPrFirebasePerformanceTrace::SetAttribute(const FString& Name, const FString& Value)
{
	if (Module.IsValid())
	{
		Module->SetAttribute(*this, Name, Value);
	}
}

void FPrFirebasePerformanceTrace::RemoveAttribute(const FString& Name)
{
	if (Module.IsValid())
	{
		Module->RemoveAttribute(*this, Name);
	}
}

int32 FPrFirebasePerformanceTrace::GetTraceIndex() const
{
	return TraceIndex;
}

FPrFirebasePerformanceScopeTimeTrace::FPrFirebasePerformanceScopeTimeTrace()
{
}

FPrFirebasePerformanceScopeTimeTrace::FPrFirebasePerformanceScopeTimeTrace(TWeakObjectPtr<UPrFirebasePerformanceModule> PerformanceModule, const FString& Identifier)
	: Trace({PerformanceModule, Identifier})
{
}

FPrFirebasePerformanceScopeTimeTrace::~FPrFirebasePerformanceScopeTimeTrace()
{
	if (Trace.IsSet())
	{
		Trace->Stop();
	}
}

UPrFirebasePerformanceModule::UPrFirebasePerformanceModule()
	: LastTraceIndex(0)
	, bAppliactionLaunched(false)
	, bAppliactionFirstFrame(false)
	, AvFrameCounter(0)
{
}

void UPrFirebasePerformanceModule::SetAttributeForAllTraces(const FString& Name, const FString& Value)
{
	bool bContains = GlobalAttributes.Contains(Name);
	if (bContains && GlobalAttributes.FindChecked(Name) == Value)
	{
		return;
	}

	GlobalAttributes.Add(Name, Value);
	for (auto TraceIndex : ExistingTraceIndices)
	{
		InternalSetAttribute(TraceIndex, Name, Value);
	}
}

void UPrFirebasePerformanceModule::RemoveAttributeForAllTraces(const FString& Name)
{
	bool bContains = GlobalAttributes.Contains(Name);
	if (bContains)
	{
		GlobalAttributes.Remove(Name);
		for (auto TraceIndex : ExistingTraceIndices)
		{
			InternalRemoveAttribute(TraceIndex, Name);
		}
	}
}

void UPrFirebasePerformanceModule::SetMetricForAllTraces(const FString& Name, int32 Value)
{
	bool bContains = GlobalMetrics.Contains(Name);
	if (bContains && GlobalMetrics.FindChecked(Name) == Value)
	{
		return;
	}

	GlobalMetrics.Add(Name, Value);
	for (auto TraceIndex : ExistingTraceIndices)
	{
		InternalSetMetric(TraceIndex, Name, Value);
	}
}

int32 UPrFirebasePerformanceModule::StartTraceWithoutWrapper(const FString& Identifier)
{
	const int32 TraceIndex = ++LastTraceIndex;
	ExistingTraceIndices.Add(TraceIndex);
	InternalStartTrace(TraceIndex, Identifier);
	for (auto& Pair : GlobalAttributes)
	{
		InternalSetAttribute(TraceIndex, Pair.Key, Pair.Value);
	}
	for (auto& Pair : GlobalMetrics)
	{
		InternalSetMetric(TraceIndex, Pair.Key, Pair.Value);
	}

	return TraceIndex;
}

void UPrFirebasePerformanceModule::StopTraceWithoutWrapper(int32 TraceIndex)
{
	if (ExistingTraceIndices.Contains(TraceIndex))
	{
		ExistingTraceIndices.Remove(TraceIndex);
		InternalStopTrace(TraceIndex);
	}
	else
	{
		UE_LOG(LogFirebase, Warning, TEXT("Trace is already stopped or not created: %d"), TraceIndex);
	}
}

FPrFirebasePerformanceTrace UPrFirebasePerformanceModule::StartTrace(const FString& Identifier)
{
	return {this, StartTraceWithoutWrapper(Identifier)};
}

void UPrFirebasePerformanceModule::StopTrace(const FPrFirebasePerformanceTrace& Trace)
{
	StopTraceWithoutWrapper(Trace.GetTraceIndex());
}

void UPrFirebasePerformanceModule::IncrementMetric(const FPrFirebasePerformanceTrace& Trace, const FString& Name, int32 Value)
{
	const int32 TraceIndex = Trace.GetTraceIndex();
	if (ExistingTraceIndices.Contains(TraceIndex))
	{
		InternalIncrementMetric(TraceIndex, Name, Value);
	}
	else
	{
		UE_LOG(LogFirebase, Warning, TEXT("Trace is already stopped or not created: %d"), TraceIndex);
	}
}

void UPrFirebasePerformanceModule::SetMetric(const FPrFirebasePerformanceTrace& Trace, const FString& Name, int32 Value)
{
	const int32 TraceIndex = Trace.GetTraceIndex();
	if (ExistingTraceIndices.Contains(TraceIndex))
	{
		InternalSetMetric(TraceIndex, Name, Value);
	}
	else
	{
		UE_LOG(LogFirebase, Warning, TEXT("Trace is already stopped or not created: %d"), TraceIndex);
	}
}

void UPrFirebasePerformanceModule::SetAttribute(const FPrFirebasePerformanceTrace& Trace, const FString& Name, const FString& Value)
{
	const int32 TraceIndex = Trace.GetTraceIndex();
	if (ExistingTraceIndices.Contains(TraceIndex))
	{
		InternalSetAttribute(TraceIndex, Name, Value);
	}
	else
	{
		UE_LOG(LogFirebase, Warning, TEXT("Trace is already stopped or not created: %d"), TraceIndex);
	}
}

void UPrFirebasePerformanceModule::RemoveAttribute(const FPrFirebasePerformanceTrace& Trace, const FString& Name)
{
	const int32 TraceIndex = Trace.GetTraceIndex();
	if (ExistingTraceIndices.Contains(TraceIndex))
	{
		InternalRemoveAttribute(TraceIndex, Name);
	}
	else
	{
		UE_LOG(LogFirebase, Warning, TEXT("Trace is already stopped or not created: %d"), TraceIndex);
	}
}

void UPrFirebasePerformanceModule::AppliactionLaunched()
{
	if (!IsAvailable())
	{
		return;
	}

	if (bAppliactionLaunched)
	{
		return;
	}

	bAppliactionLaunched = true;
	bAppliactionFirstFrame = true;
}

void UPrFirebasePerformanceModule::InternalLaunch_AnyThread()
{
	if (!AppLaunchTrace.IsSet())
	{
		AppLaunchTrace = StartTrace(TEXT("pr_app_launch"));
	}
}

void UPrFirebasePerformanceModule::SetTemperature(FCoreDelegates::ETemperatureSeverity Temp)
{
	FString TempStr = TEXT("Unknown");
	switch (Temp)
	{
	case FCoreDelegates::ETemperatureSeverity::Good:
	{
		SetMetricForAllTraces("temp", 0);
		TempStr = TEXT("Good");
	}
	break;
	case FCoreDelegates::ETemperatureSeverity::Bad:
	{
		SetMetricForAllTraces("temp", 1);
		TempStr = TEXT("Bad");
	}
	break;
	case FCoreDelegates::ETemperatureSeverity::Serious:
	{
		SetMetricForAllTraces("temp", 2);
		TempStr = TEXT("Serious");
	}
	break;
	case FCoreDelegates::ETemperatureSeverity::Critical:
	{
		SetMetricForAllTraces("temp", 3);
		TempStr = TEXT("Critical");
	}
	break;
	}

#if WITH_FIREBASE_CRASHLYTICS
	auto CrashlyticsModule = UPrFirebaseLibrary::GetFirebaseProxy()->GetCrashlyticsModule();
	CrashlyticsModule->AddAttribute(TEXT("Temperature"), TempStr);
#endif

	UE_LOG(LogPerformance, Display, TEXT("Temperature: %s"), *TempStr);
}

void UPrFirebasePerformanceModule::SetPowerMode(bool bLowPowerMode)
{
	FString PowerModeStr = TEXT("Unknown");
	if (bLowPowerMode)
	{
		SetMetricForAllTraces("low_power", 1);
		PowerModeStr = TEXT("Low");
	}
	else
	{
		SetMetricForAllTraces("low_power", 0);
		PowerModeStr = TEXT("Normal");
	}

#if WITH_FIREBASE_CRASHLYTICS
	auto CrashlyticsModule = UPrFirebaseLibrary::GetFirebaseProxy()->GetCrashlyticsModule();
	CrashlyticsModule->AddAttribute(TEXT("PowerMode"), PowerModeStr);
#endif

	UE_LOG(LogPerformance, Display, TEXT("PowerMode: %s"), *PowerModeStr);
}

void UPrFirebasePerformanceModule::StartWatch()
{
	if (!IsAvailable())
	{
		return;
	}

	check(!GPrFirebasePerformanceModule.IsValid());
	GPrFirebasePerformanceModule = this;

	auto WeakThis = MakeWeakObjectPtr(this);
	FCoreDelegates::OnEndFrame.AddLambda([WeakThis]() {
		if (WeakThis.IsValid())
		{
			WeakThis->OnEndFrame();
		}
	});

	FCoreDelegates::OnTemperatureChange.AddLambda([WeakThis](FCoreDelegates::ETemperatureSeverity Temp) {
		AsyncTask(ENamedThreads::GameThread, [WeakThis, Temp]() {
			if (WeakThis.IsValid())
			{
				WeakThis->SetTemperature(Temp);
			}
		});
	});

	FCoreDelegates::OnLowPowerMode.AddLambda([WeakThis](bool bLowPowerMode) {
		AsyncTask(ENamedThreads::GameThread, [WeakThis, bLowPowerMode]() {
			if (WeakThis.IsValid())
			{
				WeakThis->SetPowerMode(bLowPowerMode);
			}
		});
	});
}

void UPrFirebasePerformanceModule::OnAppliactionLaunched()
{
	if (AppLaunchTrace.IsSet())
	{
		AppLaunchTrace->Stop();
		AppLaunchTrace.Reset();
	}

	FDateTime StartTime;
	FDateTime::Parse(GSystemStartTime, StartTime);

	const float Delta = (FDateTime::Now() - StartTime).GetTotalSeconds();

#if WITH_FIREBASE_CRASHLYTICS
	auto CrashlyticsModule = UPrFirebaseLibrary::GetFirebaseProxy()->GetCrashlyticsModule();
	CrashlyticsModule->AddAttribute(TEXT("LaunchTime"), FString::Printf(TEXT("%.2fs"), Delta));
#endif

	UE_LOG(LogPerformance, Display, TEXT("LaunchTime: %.2fs"), Delta);
}

void UPrFirebasePerformanceModule::OnEndFrame()
{
	if (!bAppliactionLaunched)
	{
		return;
	}

	if (bAppliactionFirstFrame)
	{
		bAppliactionFirstFrame = false;
		OnAppliactionLaunched();
	}

	const FDateTime Now = FDateTime::UtcNow();
	if (AvFrameTrace.IsSet() && AvFrameTime.IsSet())
	{
		++AvFrameCounter;
		const float Delta = (Now - AvFrameTime.GetValue()).GetTotalSeconds();
		static float SyncsTime = GetDefault<UPrFirebaseSettings>()->FirebasePerformance_SyncTime;
		if (SyncsTime < MIN_SYNC_TIME)
		{
			SyncsTime = MIN_SYNC_TIME;
		}

		if (Delta >= SyncsTime)
		{
			const int32 FPS = FMath::RoundToInt(static_cast<float>(AvFrameCounter) / Delta);
			AvFrameTrace->SetMetric(TEXT("av_fps"), FPS);
			AvFrameTrace->Stop();
			AvFrameTrace.Reset();
		}
	}

	if (!AvFrameTrace.IsSet())
	{
		AvFrameTrace = StartTrace(TEXT("pr_av_frame"));
		AvFrameTime = Now;
		AvFrameCounter = 0;
	}
}

PRFIREBASE_API TWeakObjectPtr<UPrFirebasePerformanceModule> GPrFirebasePerformanceModule = nullptr;
