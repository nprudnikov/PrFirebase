// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#include "iOS/PrFirebaseAnalyticsModule_iOS.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE && PLATFORM_IOS
#include "IOSAppDelegate.h"

#import <Firebase.h>

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
	// no op
}

FString UPrFirebaseAnalyticsModule_iOS::GetAppInstanceId()
{
	return FString([FIRAnalytics appInstanceID]);
}

#endif // WITH_FIREBASE && PLATFORM_IOS
