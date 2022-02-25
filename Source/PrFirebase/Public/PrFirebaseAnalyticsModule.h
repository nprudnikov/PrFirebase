// Copyright 2021-2022 Anton Rassadin. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "PrFirebaseAnalyticsModule.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct PRFIREBASE_API FPrFirebaseImpressionData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AuctionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AdUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AdNetwork;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InstanceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InstanceId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Country;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Placement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Revenue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Precision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Ab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SegmentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifetimeRevenue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EncryptedCpm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConversionValue;

	FPrFirebaseImpressionData()
		: Revenue(0.f)
		, LifetimeRevenue(0.f)
		, ConversionValue(0.f)
	{
	}
};

UCLASS()
class PRFIREBASE_API UPrFirebaseAnalyticsModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	virtual bool IsAvailable() const final override { return WITH_FIREBASE; }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Analytics")
	virtual void LogImpression(FPrFirebaseImpressionData ImpressionData) { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Analytics")
	virtual void RequestAppInstanceId() { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Analytics")
	virtual FString GetAppInstanceId()
	{
		Firebase_NotImplemented();
		return FString{};
	}
};
