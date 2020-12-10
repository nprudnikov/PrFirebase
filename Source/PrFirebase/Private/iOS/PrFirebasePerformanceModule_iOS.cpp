// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#include "iOS/PrFirebasePerformanceModule_iOS.h"

#if WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
#include "PrFirebaseDefines.h"

#include "Async/Async.h"
#include "Misc/CoreDelegates.h"

#import <Firebase.h>
#import <Foundation/NSProcessInfo.h>

void UPrFirebasePerformanceModule_iOS::InternalStartTrace(int32 TraceIndex, const FString& Identifier)
{
	FIRTrace* trace = [FIRPerformance startTraceWithName:Identifier.GetNSString()];
	[trace retain];
	Traces.Add(TraceIndex, trace);
}

void UPrFirebasePerformanceModule_iOS::InternalStopTrace(int32 TraceIndex)
{
	FIRTrace* trace = Traces.FindAndRemoveChecked(TraceIndex);
	[trace stop];
	[trace release];
}

void UPrFirebasePerformanceModule_iOS::InternalIncrementMetric(int32 TraceIndex, const FString& Name, int32 Value)
{
	FIRTrace* trace = Traces.FindChecked(TraceIndex);
	[trace incrementMetric:Name.GetNSString() byInt:Value];
}

void UPrFirebasePerformanceModule_iOS::InternalSetMetric(int32 TraceIndex, const FString& Name, int32 Value)
{
	FIRTrace* trace = Traces.FindChecked(TraceIndex);
	[trace setIntValue:Value forMetric:Name.GetNSString()];
}

void UPrFirebasePerformanceModule_iOS::InternalSetAttribute(int32 TraceIndex, const FString& Name, const FString& Value)
{
	FIRTrace* trace = Traces.FindChecked(TraceIndex);
	[trace setValue:Value.GetNSString() forAttribute:Name.GetNSString()];
}

void UPrFirebasePerformanceModule_iOS::InternalRemoveAttribute(int32 TraceIndex, const FString& Name)
{
	FIRTrace* trace = Traces.FindChecked(TraceIndex);
	[trace removeAttribute:Name.GetNSString()];
}

void UPrFirebasePerformanceModule_iOS::PostInitialize_AnyThread()
{
	//TODO: disabe firebase auto stats

	InternalLaunch_AnyThread();

#if !PLATFORM_TVOS
	if (@available(iOS 11, *))
	{
		FCoreDelegates::ETemperatureSeverity Temp = FCoreDelegates::ETemperatureSeverity::Good;
		switch ([[NSProcessInfo processInfo] thermalState])
		{
		case NSProcessInfoThermalStateNominal:
			Temp = FCoreDelegates::ETemperatureSeverity::Good;
			break;
		case NSProcessInfoThermalStateFair:
			Temp = FCoreDelegates::ETemperatureSeverity::Bad;
			break;
		case NSProcessInfoThermalStateSerious:
			Temp = FCoreDelegates::ETemperatureSeverity::Serious;
			break;
		case NSProcessInfoThermalStateCritical:
			Temp = FCoreDelegates::ETemperatureSeverity::Critical;
			break;
		}

		bool bLowPowerMode = [[NSProcessInfo processInfo] isLowPowerModeEnabled];

		AsyncTask(ENamedThreads::GameThread, [WeakThis = MakeWeakObjectPtr(this), Temp, bLowPowerMode]() {
			if (WeakThis.IsValid())
			{
				WeakThis->SetTemperature(Temp);
				WeakThis->SetPowerMode(bLowPowerMode);
			}
		});
	}
#endif

	AsyncTask(ENamedThreads::GameThread, [WeakThis = MakeWeakObjectPtr(this)]() {
		if (WeakThis.IsValid())
		{
			WeakThis->StartWatch();
		}
	});
}

#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
