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

void UPrFirebaseAnalyticsModule_iOS::LogScreenView(FString ScreenName, FString ScreenClass)
{
	[FIRAnalytics logEventWithName:kFIREventScreenView
		parameters:@{
			kFIRParameterScreenClass:[NSString stringWithFString:ScreenName],
			kFIRParameterScreenName:[NSString stringWithFString:ScreenName]
	}];
}

void UPrFirebaseAnalyticsModule_iOS::LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value)
{
	[FIRAnalytics logEventWithName:kFIREventSpendVirtualCurrency
        parameters:@{
            kFIRParameterItemName:[NSString stringWithFString:ItemName],
            kFIRParameterVirtualCurrencyName:[NSString stringWithFString:CurrencyName],
        	kFIRParameterValue:[NSNumber numberWithFloat:Value]
    }];
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelUp(FString Character, int Level)
{
	[FIRAnalytics logEventWithName:kFIREventLevelUp
        parameters:@{
            kFIRParameterLevel:[NSNumber numberWithInteger:Level],
            kFIRParameterCharacter:[NSString stringWithFString:Character]
    }];
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelStart(FString LevelName)
{
	[FIRAnalytics logEventWithName:kFIREventLevelStart
		parameters:@{
			kFIRParameterLevelName:[NSString stringWithFString:LevelName]
    }];
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelEnd(FString LevelName, FString Success)
{
	[FIRAnalytics logEventWithName:kFIREventLevelEnd
		parameters:@{
			kFIRParameterLevelName:[NSString stringWithFString:LevelName],
			kFIRParameterSuccess:[NSString stringWithFString:Success]
	}];
}

void UPrFirebaseAnalyticsModule_iOS::SetDefaultEventParameters(TMap<FString, FString> StringParams, TMap<FString, float> FloatParams)
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
	
	[FIRAnalytics setDefaultEventParameters:ParamsDict];
}

void UPrFirebaseAnalyticsModule_iOS::Initialize_AnyThread()
{

}

#endif // WITH_FIREBASE_ANALYTICS && PLATFORM_IOS
