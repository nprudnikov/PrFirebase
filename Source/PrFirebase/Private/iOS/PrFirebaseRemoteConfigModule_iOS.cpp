// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "iOS/PrFirebaseRemoteConfigModule_iOS.h"

#if WITH_FIREBASE_REMOTECONFIG && PLATFORM_IOS

#include "PrFirebaseDefines.h"
#include "PrFirebaseSettings.h"

#include "Async/Async.h"

#import <Firebase.h>

UPrFirebaseRemoteConfigModule_iOS::UPrFirebaseRemoteConfigModule_iOS()
	: bFetching(true)
	, FetchCounter(0)
{
}

bool UPrFirebaseRemoteConfigModule_iOS::HasValue(const FString& Key) const
{
	if (FetchCounter > 0)
	{
		FIRRemoteConfigValue* configValue = [[FIRRemoteConfig remoteConfig] configValueForKey:Key.GetNSString()];
		if (configValue != nil)
		{
			return true;
		}
	}

	return false;
}

void UPrFirebaseRemoteConfigModule_iOS::GetStringValue(const FString& Key, FString& OutValue) const
{
	if (FetchCounter > 0)
	{
		FIRRemoteConfigValue* configValue = [[FIRRemoteConfig remoteConfig] configValueForKey:Key.GetNSString()];
		if (configValue != nil && configValue.stringValue != nil)
		{
			OutValue = FString(configValue.stringValue);
			return;
		}
	}
}

void UPrFirebaseRemoteConfigModule_iOS::GetIntValue(const FString& Key, int32& OutValue) const
{
	if (FetchCounter > 0)
	{
		FIRRemoteConfigValue* configValue = [[FIRRemoteConfig remoteConfig] configValueForKey:Key.GetNSString()];
		if (configValue != nil && configValue.numberValue != nil)
		{
			OutValue = configValue.numberValue.intValue;
			return;
		}
	}
}

void UPrFirebaseRemoteConfigModule_iOS::GetFloatValue(const FString& Key, float& OutValue) const
{
	if (FetchCounter > 0)
	{
		FIRRemoteConfigValue* configValue = [[FIRRemoteConfig remoteConfig] configValueForKey:Key.GetNSString()];
		if (configValue != nil && configValue.numberValue != nil)
		{
			OutValue = configValue.numberValue.floatValue;
			return;
		}
	}
}

void UPrFirebaseRemoteConfigModule_iOS::GetBoolValue(const FString& Key, bool& OutValue) const
{
	if (FetchCounter > 0)
	{
		FIRRemoteConfigValue* configValue = [[FIRRemoteConfig remoteConfig] configValueForKey:Key.GetNSString()];
		if (configValue != nil && configValue.stringValue != nil)
		{
			auto Value = FString(configValue.stringValue).ToLower();
			OutValue = (Value == TEXT("1") || Value == TEXT("true") || Value == TEXT("yes") || Value == TEXT("on"));
			return;
		}
	}
}

void UPrFirebaseRemoteConfigModule_iOS::Fetch()
{
	if (bFetching)
	{
		return;
	}

	bFetching = true;

	auto WeakThis = MakeWeakObjectPtr(this);
	dispatch_async(dispatch_get_main_queue(), ^{
	  if (WeakThis.IsValid())
	  {
		  WeakThis->InternalFetch();
	  }
	});
}

bool UPrFirebaseRemoteConfigModule_iOS::IsFetched() const
{
	return FetchCounter > 0;
}

int32 UPrFirebaseRemoteConfigModule_iOS::GetFetchCounter() const
{
	return FetchCounter;
}

void UPrFirebaseRemoteConfigModule_iOS::Initialize_AnyThread()
{
	FIRRemoteConfig* remoteConfig = [FIRRemoteConfig remoteConfig];
	FIRRemoteConfigSettings* remoteConfigSettings = [[FIRRemoteConfigSettings alloc] init];
	remoteConfigSettings.minimumFetchInterval = 0;
	remoteConfig.configSettings = remoteConfigSettings;
}

void UPrFirebaseRemoteConfigModule_iOS::PostInitialize_AnyThread()
{
	InternalFetch();
}

void UPrFirebaseRemoteConfigModule_iOS::InternalFetch()
{
	// TODO: call GetDefault not safe
	static const int32 ExpirationDuration = GetDefault<UPrFirebaseSettings>()->FirebaseRemoteConfig_ExpirationDurationForFetch;

	NSLog(@"Firebase: RemoteConfig fetching...");
	[[FIRRemoteConfig remoteConfig] fetchWithExpirationDuration:ExpirationDuration
											  completionHandler:^(FIRRemoteConfigFetchStatus status, NSError* error0) {
												if (status == FIRRemoteConfigFetchStatusSuccess)
												{
													NSLog(@"Firebase: RemoteConfig fetched, activation...");
													auto WeakThis = MakeWeakObjectPtr(this);
													[[FIRRemoteConfig remoteConfig] activateWithCompletion:^(BOOL changed, NSError* _Nullable error1) {
													  NSLog(@"Firebase: RemoteConfig activated");
													  if (WeakThis.IsValid())
													  {
														  WeakThis->FetchCompleted(true);
													  }
													}];
												}
												else
												{
													NSLog(@"Firebase: RemoteConfig not fetched");
													NSLog(@"Error %@", error0.localizedDescription);
													FetchCompleted(false);
												}
											  }];
}

void UPrFirebaseRemoteConfigModule_iOS::FetchCompleted(bool bSuccess)
{
	AsyncTask(ENamedThreads::GameThread, [WeakThis = MakeWeakObjectPtr(this), bSuccess]() {
		if (WeakThis.IsValid())
		{
			WeakThis->bFetching = false;

			if (bSuccess)
			{
				WeakThis->FetchCounter += 1;
				WeakThis->FetchingDelegate.Broadcast();
			}
			else
			{
				WeakThis->Fetch();
			}
		}
	});
}

#endif // WITH_FIREBASE_REMOTECONFIG && PLATFORM_IOS
