// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "PrFirebasePerformanceModule.h"

#include "PrFirebaseDefines.h"
#include "PrFirebaseSettings.h"

#include "Async/Async.h"
#include "DeviceProfiles/DeviceProfile.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "RHI.h"
#include "RenderCore.h"

#define MIN_SYNC_TIME 0.5f

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseSyncAccum

FPrFirebaseSyncAccum::FPrFirebaseSyncAccum()
	: FrameCounter(0)
	, GameThreadTimeMs(0.f)
	, RenderThreadTimeMs(0.f)
	, RHIThreadTimeMs(0.f)
{
}

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseTimeline

FPrFirebaseTimeline::FPrFirebaseTimeline()
	: Count(0)
{
}

void FPrFirebaseTimeline::Add(int32 Value, const FDateTime& Time)
{
	if (Timeline.Num() == 0 || Timeline.Last().Get<0>() != Value)
	{
		Timeline.Add(TTuple<int32, FDateTime>(Value, Time));
		Count++;
	}
}

bool FPrFirebaseTimeline::IsChanged() const
{
	return Timeline.Num() > 1;
}

bool FPrFirebaseTimeline::HasValue() const
{
	return Timeline.Num() > 0;
}

int32 FPrFirebaseTimeline::GetCount() const
{
	return Count;
}

FPrFirebaseTimelineDelta FPrFirebaseTimeline::GetDelta() const
{
	if (Timeline.Num() > 1)
	{
		const auto& First = Timeline[0];
		const auto& Last = Timeline.Last();
		const auto Delta = Last.Get<0>() - First.Get<0>();
		const auto DeltaTime = static_cast<float>((Last.Get<1>() - First.Get<1>()).GetTotalSeconds());

		return {Delta, DeltaTime};
	}

	return {0, 0.f};
}

int32 FPrFirebaseTimeline::GetValue() const
{
	if (Timeline.Num() > 0)
	{
		return Timeline.Last().Get<0>();
	}

	return 0;
}

void FPrFirebaseTimeline::Reset(bool bFully)
{
	if (bFully)
	{
		Timeline.Reset();
		Count = 0;
	}
	else if (Timeline.Num() > 1)
	{
		auto Last = Timeline.Last();
		Timeline.Reset();
		Timeline.Add(std::move(Last));
	}
}

//////////////////////////////////////////////////////////////////////////
// FPrFirebaseDeviceTimelines

FPrFirebaseDeviceTimelines::FPrFirebaseDeviceTimelines()
{
}

void FPrFirebaseDeviceTimelines::Reset(bool bFully)
{
	BatteryLevel.Reset(bFully);
	VolumeLevel.Reset(bFully);
	HeatingLevel.Reset(bFully);
}

//////////////////////////////////////////////////////////////////////////
// FPrFirebasePerformanceTrace

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

//////////////////////////////////////////////////////////////////////////
// FPrFirebasePerformanceScopeTimeTrace

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

//////////////////////////////////////////////////////////////////////////
// UPrFirebasePerformanceModule

UPrFirebasePerformanceModule::UPrFirebasePerformanceModule()
	: LastTraceIndex(0)
	, bAppliactionLaunched(false)
	, bAppliactionFirstFrame(false)
{
}

int32 UPrFirebasePerformanceModule::StartTraceWithoutWrapper(const FString& Identifier)
{
	const int32 TraceIndex = ++LastTraceIndex;
	ExistingTraceIndices.Add(TraceIndex);
	InternalStartTrace(TraceIndex, Identifier);

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

void UPrFirebasePerformanceModule::SetFrameTraceCategory(const FString& Key, const FString& Category)
{
	FrameTraceCategories.Add(Key, Category);
	UpdatePostfix();
}

void UPrFirebasePerformanceModule::ResetFrameTraceCategory(const FString& Key)
{
	FrameTraceCategories.Remove(Key);
	UpdatePostfix();
}

void UPrFirebasePerformanceModule::UpdatePostfix()
{
	FString NewFramePostfix;
	if (FrameTraceCategories.Num() > 0)
	{
		TArray<FString> Keys;
		FrameTraceCategories.GetKeys(Keys);

		Keys.Sort([](const FString& A, const FString& B) {
			return A < B;
		});

		for (const auto& Key : Keys)
		{
			NewFramePostfix += TEXT("_") + FrameTraceCategories.FindChecked(Key).ToLower();
		}
	}

	if (FramePostfix != NewFramePostfix)
	{
		FramePostfix = NewFramePostfix;
		SyncTime.Reset();
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

void UPrFirebasePerformanceModule::StartWatch()
{
	if (!IsAvailable())
	{
		return;
	}

	check(!GPrFirebasePerformanceModule.IsValid());
	GPrFirebasePerformanceModule = this;

	UDeviceProfile* DeviceProfile = UDeviceProfileManager::Get().GetActiveProfile();
	if (DeviceProfile)
	{
		DeviceProfileName = DeviceProfile->GetName();
	}
	else
	{
		DeviceProfileName = TEXT("Unknown");
	}

	DeviceBrand = FPlatformMisc::GetCPUBrand();
	DeviceChipset = FString::Printf(TEXT("%s | %d"), *FPlatformMisc::GetCPUChipset(), FPlatformMisc::NumberOfCores());

	UpdateDeviceTimelines(FDateTime::UtcNow(), false);

	auto WeakThis = MakeWeakObjectPtr(this);
	FCoreDelegates::OnEndFrame.AddLambda([WeakThis]() {
		if (WeakThis.IsValid())
		{
			WeakThis->OnEndFrame();
		}
	});

	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddLambda([WeakThis]() {
		if (WeakThis.IsValid())
		{
			WeakThis->DeviceTimelines.Reset();
			WeakThis->UpdateDeviceTimelines(FDateTime::UtcNow(), false);
		}
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
	static const IConsoleVariable* CVarMaxFPS = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));

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
	if (SyncTime.IsSet())
	{
		AvFrameAccum.FrameCounter++;
		AvFrameAccum.GameThreadTimeMs += FPlatformTime::ToMilliseconds(GGameThreadTime);
		AvFrameAccum.RenderThreadTimeMs += FPlatformTime::ToMilliseconds(GRenderThreadTime);
		AvFrameAccum.RHIThreadTimeMs += FPlatformTime::ToMilliseconds(GRHIThreadTime);

		const float Delta = (Now - SyncTime.GetValue()).GetTotalSeconds();
		const float SyncTimeInterval = FMath::Max(GetDefault<UPrFirebaseSettings>()->FirebasePerformance_SyncTime, MIN_SYNC_TIME);

		if (Delta >= SyncTimeInterval)
		{
			if (AvFrameAccum.FrameCounter > 0)
			{
				const int32 MaxFPS = CVarMaxFPS->GetInt();
				auto FrameTrace = StartTrace(FString::Printf(TEXT("pr_frame_%d%s"), MaxFPS, *FramePostfix));
				UpdateTraceAttributes(FrameTrace);

				const float FloatMaxFPS = static_cast<float>(CVarMaxFPS->GetInt());
				const float FloatFrameCounter = static_cast<float>(AvFrameAccum.FrameCounter);
				const float FloatFPS = FloatFrameCounter / Delta;
				FrameTrace.SetMetric(TEXT("fps"), FMath::RoundToInt(FloatFPS));
				FrameTrace.SetMetric(TEXT("fps_rel"), FMath::RoundToInt((FloatFPS / FloatMaxFPS) * 100.f));

				const float MaxTimeMs = 1000.f / FloatFPS;
				const float AvGameThreadTimeMs = AvFrameAccum.GameThreadTimeMs / FloatFrameCounter;
				const float AvRenderThreadTimeMs = AvFrameAccum.RenderThreadTimeMs / FloatFrameCounter;
				const float AvRHIThreadTimeMs = AvFrameAccum.RHIThreadTimeMs / FloatFrameCounter;

				FrameTrace.SetMetric(TEXT("game_time_mks"), FMath::RoundToInt(AvGameThreadTimeMs * 1000.f));
				FrameTrace.SetMetric(TEXT("game_time_rel"), FMath::RoundToInt((AvGameThreadTimeMs / MaxTimeMs) * 100.f));

				FrameTrace.SetMetric(TEXT("render_time_mks"), FMath::RoundToInt(AvRenderThreadTimeMs * 1000.f));
				FrameTrace.SetMetric(TEXT("render_time_rel"), FMath::RoundToInt((AvRenderThreadTimeMs / MaxTimeMs) * 100.f));

				if (AvRHIThreadTimeMs > KINDA_SMALL_NUMBER)
				{
					FrameTrace.SetMetric(TEXT("rhi_time_mks"), FMath::RoundToInt(AvRHIThreadTimeMs * 1000.f));
					FrameTrace.SetMetric(TEXT("rhi_time_rel"), FMath::RoundToInt((AvRHIThreadTimeMs / MaxTimeMs) * 100.f));
				}

				const float AvTotalThreadTimeMs = AvGameThreadTimeMs + AvRenderThreadTimeMs + AvRHIThreadTimeMs;
				FrameTrace.SetMetric(TEXT("total_time_mks"), FMath::RoundToInt(AvTotalThreadTimeMs * 1000.f));
				FrameTrace.SetMetric(TEXT("total_time_rel"), FMath::RoundToInt((AvTotalThreadTimeMs / MaxTimeMs) * 100.f));

				FrameTrace.Stop();
			}

			UpdateDeviceTimelines(Now, true);

			SyncTime.Reset();
		}
	}
	else
	{
		SyncTime = Now;
		AvFrameAccum = {};
	}
}

void UPrFirebasePerformanceModule::UpdateDeviceTimelines(const FDateTime& Now, bool bSync)
{
	const auto BatteryLevel = FPlatformMisc::GetBatteryLevel();
	if (BatteryLevel >= 0)
	{
		DeviceTimelines.BatteryLevel.Add(BatteryLevel, Now);
	}

	const auto DeviceVolume = FPlatformMisc::GetDeviceVolume();
	if (DeviceVolume >= 0)
	{
		DeviceTimelines.VolumeLevel.Add(DeviceVolume, Now);
	}

	const auto DeviceHeating = FPlatformMisc::GetDeviceTemperatureLevel();
	if (DeviceHeating >= 0)
	{
		DeviceTimelines.HeatingLevel.Add(DeviceHeating, Now);
	}

	if (bSync)
	{
		if (DeviceTimelines.BatteryLevel.IsChanged())
		{
			if (FPlatformMisc::IsRunningOnBattery())
			{
				if (DeviceTimelines.BatteryLevel.GetCount() > 2)
				{
					const auto TimelineDelta = DeviceTimelines.BatteryLevel.GetDelta();
					if (TimelineDelta.Value < 0)
					{
						auto BatteryTrace = StartTrace(TEXT("pr_battery"));
						UpdateTraceAttributes(BatteryTrace);

						const auto dSec = TimelineDelta.Seconds / static_cast<float>(-TimelineDelta.Value);
						BatteryTrace.SetMetric(TEXT("value_down_sec"), FMath::RoundToInt(dSec));
						BatteryTrace.SetMetric(TEXT("value"), DeviceTimelines.BatteryLevel.GetValue());
						BatteryTrace.Stop();
					}
				}

				DeviceTimelines.BatteryLevel.Reset();
			}
			else
			{
				DeviceTimelines.BatteryLevel.Reset(true);
			}
		}

		if (DeviceTimelines.VolumeLevel.IsChanged())
		{
			auto VolumeTrace = StartTrace(TEXT("pr_volume"));
			UpdateTraceAttributes(VolumeTrace);

			VolumeTrace.SetMetric(TEXT("value"), DeviceTimelines.VolumeLevel.GetValue());
			VolumeTrace.Stop();

			DeviceTimelines.VolumeLevel.Reset();
		}

		if (DeviceTimelines.HeatingLevel.IsChanged())
		{
			if (DeviceTimelines.HeatingLevel.GetCount() > 2)
			{
				const auto Delta = DeviceTimelines.HeatingLevel.GetDelta();
				if (Delta.Value != 0)
				{
					auto HeatingTrace = StartTrace(TEXT("pr_heating"));
					UpdateTraceAttributes(HeatingTrace);

					if (Delta.Value > 0)
					{
						const auto Sec = Delta.Seconds / static_cast<float>(Delta.Value);
						HeatingTrace.SetMetric(TEXT("value_up_sec"), FMath::RoundToInt(Sec));
					}
					else
					{
						const auto Sec = Delta.Seconds / static_cast<float>(-Delta.Value);
						HeatingTrace.SetMetric(TEXT("value_down_sec"), FMath::RoundToInt(Sec));
					}

					HeatingTrace.SetMetric(TEXT("value"), DeviceTimelines.HeatingLevel.GetValue());

					HeatingTrace.Stop();
				}
			}

			DeviceTimelines.HeatingLevel.Reset();
		}
	}
}

void UPrFirebasePerformanceModule::UpdateTraceAttributes(FPrFirebasePerformanceTrace& Trace) const
{
	Trace.SetAttribute(TEXT("Profile"), DeviceProfileName);

#if !PLATFORM_IOS
	Trace.SetAttribute(TEXT("Adapter"), GRHIAdapterName);
	Trace.SetAttribute(TEXT("Brand"), DeviceBrand);
	Trace.SetAttribute(TEXT("Chipset"), DeviceChipset);
#endif // !PLATFORM_IOS
}

PRFIREBASE_API TWeakObjectPtr<UPrFirebasePerformanceModule> GPrFirebasePerformanceModule = nullptr;
