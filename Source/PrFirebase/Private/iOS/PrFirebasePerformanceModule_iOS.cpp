// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

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
	// TODO: disabe firebase auto stats

	InternalLaunch_AnyThread();

	AsyncTask(ENamedThreads::GameThread, [WeakThis = MakeWeakObjectPtr(this)]() {
		if (WeakThis.IsValid())
		{
			WeakThis->StartWatch();
		}
	});
}

#endif // WITH_FIREBASE_PERFORMANCE && PLATFORM_IOS
