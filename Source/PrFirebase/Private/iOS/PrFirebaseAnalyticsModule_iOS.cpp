// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#include "iOS/PrFirebaseAnalyticsModule_iOS.h"
#include "PrFirebaseDefines.h"

#if WITH_FIREBASE_ANALYTICS && PLATFORM_IOS

#import <Firebase.h>

void UPrFirebaseAnalyticsModule_iOS::LogEvent(FString EventName, TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
{
	NSMutableDictionary* ParamsDict = [NSMutableDictionary dictionary];

	for(const TPair<FString, FString>& ParamPair : StringParams)
	{
		[ParamsDict setValue:[NSString stringWithFString:ParamPair.Value] forKey:[NSString stringWithFString:ParamPair.Key]];
	}
	for(const TPair<FString, float>& ParamPair : FloatParams)
	{
		[ParamsDict setValue:[NSNumber numberWithFloat:ParamPair.Value] forKey:[NSString stringWithFString:ParamPair.Key]];
	}
	
	[FIRAnalytics logEventWithName:[NSString stringWithFString:EventName] parameters:ParamsDict];
}

void UPrFirebaseAnalyticsModule_iOS::Initialize_AnyThread()
{

}

#endif // WITH_FIREBASE_ANALYTICS && PLATFORM_IOS
