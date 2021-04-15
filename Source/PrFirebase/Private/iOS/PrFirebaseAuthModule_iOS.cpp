// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "iOS/PrFirebaseAuthModule_iOS.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE_AUTH && PLATFORM_IOS
#include "Async/Async.h"
#include "IOSAppDelegate.h"

#import <Firebase.h>
#import <GameKit/GameKit.h>

UPrFirebaseAuthModule_iOS::UPrFirebaseAuthModule_iOS()
	: bNativePlatform(false)
{
}

void UPrFirebaseAuthModule_iOS::SignInNativePlatform(FPrFirebaseAuthDelegate Delegate)
{
	SignInWithGameCenter(Delegate);
}

void UPrFirebaseAuthModule_iOS::GetUserInfo(FPrFirebaseUser& OutUserInfo) const
{
	OutUserInfo = UserInfo.GetValue();
}

bool UPrFirebaseAuthModule_iOS::IsAuthenticated() const
{
	return UserInfo.IsSet();
}

void UPrFirebaseAuthModule_iOS::Initialize_AnyThread()
{
}

void UPrFirebaseAuthModule_iOS::SignInWithGameCenter(FPrFirebaseAuthDelegate Delegate)
{
	if (bNativePlatform || true)
	{
		OnSignInWithGameCenterError(Delegate);
		return;
	}

	bNativePlatform = true;

	dispatch_async(dispatch_get_main_queue(), ^{
	  GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
	  localPlayer.authenticateHandler = ^(UIViewController* viewController, NSError* error) {
		if (viewController != nil)
		{
			[[IOSAppDelegate GetDelegate].IOSController presentViewController:viewController animated:YES completion:nil];
		}
		else if (localPlayer.isAuthenticated)
		{
			[FIRGameCenterAuthProvider getCredentialWithCompletion:^(FIRAuthCredential* credential, NSError* firError) {
			  if (firError == nil)
			  {
				  [[FIRAuth auth] signInWithCredential:credential
											completion:^(FIRAuthDataResult* result, NSError* authError) {
											  if (authError == nil)
											  {
												  OnSignInWithGameCenterSuccess(Delegate);
											  }
											  else
											  {
												  OnSignInWithGameCenterError(Delegate);
											  }
											}];
			  }
			  else
			  {
				  OnSignInWithGameCenterError(Delegate);
			  }
			}];
		}
		else
		{
			OnSignInWithGameCenterError(Delegate);
		}
	  };
	});
}

void UPrFirebaseAuthModule_iOS::OnSignInWithGameCenterSuccess(FPrFirebaseAuthDelegate Delegate)
{
	if (UserInfo.IsSet())
	{
		return;
	}

	FIRUser* firUser = [FIRAuth auth].currentUser;
	if (firUser)
	{
		FPrFirebaseUser User;
		User.UserName = FString(firUser.displayName);
		User.Uid = FString(firUser.uid);

		UserInfo = User;

		AsyncTask(ENamedThreads::GameThread, [Delegate]() {
			Delegate.ExecuteIfBound(true);
		});
	}
	else
	{
		OnSignInWithGameCenterError(Delegate);
	}
}

void UPrFirebaseAuthModule_iOS::OnSignInWithGameCenterError(FPrFirebaseAuthDelegate Delegate)
{
	AsyncTask(ENamedThreads::GameThread, [Delegate]() {
		Delegate.ExecuteIfBound(false);
	});
}

#endif // WITH_FIREBASE_AUTH && PLATFORM_IOS
