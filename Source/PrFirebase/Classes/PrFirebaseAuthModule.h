#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "PrFirebaseAuthModule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct PRFIREBASE_API FPrFirebaseUser
{
	GENERATED_BODY();

	FPrFirebaseUser(){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Uid;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FPrFirebaseAuthDelegate, bool, bSuccess);

UCLASS()
class PRFIREBASE_API UPrFirebaseAuthModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Firebase|Auth")
	virtual void SignInNativePlatform(FPrFirebaseAuthDelegate Delegate) { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Auth")
	virtual void GetUserInfo(FPrFirebaseUser& OutUserInfo) const { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Auth")
	virtual bool IsAuthenticated() const
	{
		Firebase_NotImplemented();
		return false;
	}

	virtual bool IsAvailable() const final { return WITH_FIREBASE_CRASHLYTICS; }
};
