// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "PrFirebaseModule.h"

#include "PrFirebaseDefines.h"

#include "PrFirebaseCrashlyticsModule.generated.h"

class FOutputDevice;

UCLASS()
class PRFIREBASE_API UPrFirebaseCrashlyticsModule : public UPrFirebaseModule
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics|Crash")
	virtual void Crash() { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics|User")
	virtual void SetUserIdentifier(const FString& UserIdentifier) { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics|Log")
	virtual void WriteLog(const FString& Log) { Firebase_NotImplemented(); }

	void WriteError(const FString& Log);

	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics|Log", meta = (AutoCreateRefTerm = "UserInfo"))
	virtual void WriteError(const FString& Log, int32 Code, const TMap<FString, FString>& UserInfo) { Firebase_NotImplemented(); }

	UFUNCTION(BlueprintCallable, Category = "Firebase|Crashlytics|Value")
	virtual void AddAttribute(const FString& Key, const FString& Value) { Firebase_NotImplemented(); }

	virtual bool IsAvailable() const final override { return WITH_FIREBASE_CRASHLYTICS; }

protected:
	friend class FCrashlyticsOutputDevice;
	friend class FCrashlyticsOutputDeviceError;

	void CatchEngineLogs();

	FString CrashlyticsLogFormat(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category);

	void WriteBlueprintCallstack();

	void Log(bool bCritical, const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category);
};
