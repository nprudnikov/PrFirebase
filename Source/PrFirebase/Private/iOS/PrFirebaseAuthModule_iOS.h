// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseAuthModule.h"

#include "PrFirebaseAuthModule_iOS.generated.h"

UCLASS()
class UPrFirebaseAuthModule_iOS : public UPrFirebaseAuthModule
{
	GENERATED_BODY()

#if WITH_FIREBASE_AUTH && PLATFORM_IOS

	UPrFirebaseAuthModule_iOS();

public:
	virtual void SignInNativePlatform(FPrFirebaseAuthDelegate Delegate) override;

	virtual void GetUserInfo(FPrFirebaseUser& OutUserInfo) const override;

	virtual bool IsAuthenticated() const override;

protected:
	virtual void Initialize_AnyThread();

private:
	bool bNativePlatform;

	TOptional<FPrFirebaseUser> UserInfo;

	void SignInWithGameCenter(FPrFirebaseAuthDelegate Delegate);

	void OnSignInWithGameCenterSuccess(FPrFirebaseAuthDelegate Delegate);

	void OnSignInWithGameCenterError(FPrFirebaseAuthDelegate Delegate);

#endif // WITH_FIREBASE_AUTH && PLATFORM_IOS
};
