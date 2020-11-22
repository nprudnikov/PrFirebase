#pragma once

#include "PrFirebaseDefines.h"

#include "CoreMinimal.h"

#include "PrFirebaseModule.generated.h"

UCLASS()
class PRFIREBASE_API UPrFirebaseModule : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Firebase|Module")
	virtual bool IsAvailable() const
	{
		Firebase_NotImplemented();
		return false;
	}

protected:
	friend class UPrFirebaseProxy;

	virtual void PreInitialize_AnyThread() {}
	virtual void Initialize_AnyThread() {}
	virtual void PostInitialize_AnyThread() {}
};
