// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#include "iOS/PrFirebaseAnalyticsModule_iOS.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE && PLATFORM_IOS
#include "IOSAppDelegate.h"

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
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:[NSString stringWithFString:EventName] parameters:ParamsDict];
	});	
}

void UPrFirebaseAnalyticsModule_iOS::LogScreenView(FString ScreenName, FString ScreenClass)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventScreenView
			parameters:@{
				kFIRParameterScreenClass:[NSString stringWithFString:ScreenName],
				kFIRParameterScreenName:[NSString stringWithFString:ScreenName]
		}];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogSpendVirtualCurrency(FString ItemName, FString CurrencyName, float Value)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventSpendVirtualCurrency
	        parameters:@{
	            kFIRParameterItemName:[NSString stringWithFString:ItemName],
	            kFIRParameterVirtualCurrencyName:[NSString stringWithFString:CurrencyName],
        		kFIRParameterValue:[NSNumber numberWithFloat:Value]
	    }];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelUp(FString Character, int Level)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventLevelUp
	        parameters:@{
	            kFIRParameterLevel:[NSNumber numberWithInteger:Level],
	            kFIRParameterCharacter:[NSString stringWithFString:Character]
	    }];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelStart(FString LevelName)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventLevelStart
			parameters:@{
				kFIRParameterLevelName:[NSString stringWithFString:LevelName]
	    }];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogLevelEnd(FString LevelName, FString Success)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventLevelEnd
			parameters:@{
				kFIRParameterLevelName:[NSString stringWithFString:LevelName],
				kFIRParameterSuccess:[NSString stringWithFString:Success]
		}];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogPurchase(FPrFirebasePurchaseData PurchaseData)
{
	NSMutableDictionary* ParamsDict = [@{
		kFIRParameterTransactionID: [NSString stringWithFString:PurchaseData.TransactionID],
		kFIRParameterCurrency: [NSString stringWithFString:PurchaseData.Currency],
		kFIRParameterValue: [NSNumber numberWithFloat:PurchaseData.Value]
	} mutableCopy];
	
	for(const TPair<FString, FString>& ParamPair : PurchaseData.CustomData)
	{
		[ParamsDict setValue:[NSString stringWithFString:ParamPair.Value] forKey:[NSString stringWithFString:ParamPair.Key]];
	}
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventPurchase parameters:ParamsDict];
	});
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
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics setDefaultEventParameters:ParamsDict];
	});
}

void UPrFirebaseAnalyticsModule_iOS::LogImpression(FPrFirebaseImpressionData ImpressionData)
{
	// clang-format off
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventAdImpression parameters:@{
			kFIRParameterAdPlatform:@"ironSource",
			kFIRParameterAdSource:ImpressionData.AdNetwork.GetNSString(),
			kFIRParameterAdFormat:ImpressionData.AdUnit.GetNSString(),
			kFIRParameterAdUnitName:ImpressionData.InstanceName.GetNSString(),
			kFIRParameterCurrency:@"USD",
			kFIRParameterValue:[NSNumber numberWithFloat:ImpressionData.Revenue]
		}];
	});
	// clang-format on
}

void UPrFirebaseAnalyticsModule_iOS::RequestAppInstanceId()
{
	AppInstanceIdReadyDelegate.Broadcast();
}

FString UPrFirebaseAnalyticsModule_iOS::GetAppInstanceId()
{
	return FString([FIRAnalytics appInstanceID]);
}

void UPrFirebaseAnalyticsModule_iOS::LogRevenue(float RevenueUSD)
{
	// clang-format off
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:@"total_revenue" parameters:@{
			kFIRParameterCurrency:@"USD",
			kFIRParameterValue:[NSNumber numberWithFloat:RevenueUSD]
		}];
	});
	// clang-format on
}

#endif // WITH_FIREBASE && PLATFORM_IOS
