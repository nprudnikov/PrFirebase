// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "PrFirebaseCrashlyticsModule.h"

#include "PrFirebaseSettings.h"

#include "HAL/PlatformOutputDevices.h"
#include "Misc/CoreMisc.h"
#include "Misc/OutputDevice.h"
#include "Misc/OutputDeviceError.h"
#include "Misc/OutputDeviceHelper.h"

#if WITH_FIREBASE_REMOTECONFIG
#include "PrFirebaseDefines.h"
#include "PrFirebaseLibrary.h"
#include "PrFirebaseRemoteConfigModule.h"

#include "Async/Async.h"
#endif

class FCrashlyticsOutputDevice : public FOutputDevice
{
private:
	FOutputDevice* Original;

	TWeakObjectPtr<UPrFirebaseCrashlyticsModule> CrashlyticsModule;

public:
	FCrashlyticsOutputDevice(FOutputDevice* InOriginal, TWeakObjectPtr<UPrFirebaseCrashlyticsModule> InCrashlyticsModule)
		: Original(InOriginal)
		, CrashlyticsModule(InCrashlyticsModule)
	{
	}

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		if (CrashlyticsModule.IsValid())
		{
			CrashlyticsModule->Log(false, V, Verbosity, Category);
		}

#if (!UE_BUILD_SHIPPING || FIREBASE_LOGGING)
		Original->Serialize(V, Verbosity, Category);
#endif // !UE_BUILD_SHIPPING
	}

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category, const double Time) override
	{
		if (CrashlyticsModule.IsValid())
		{
			CrashlyticsModule->Log(false, V, Verbosity, Category);
		}

#if (!UE_BUILD_SHIPPING || FIREBASE_LOGGING)
		Original->Serialize(V, Verbosity, Category, Time);
#endif // !UE_BUILD_SHIPPING
	}
};

class FCrashlyticsOutputDeviceError : public FOutputDeviceError
{
private:
	FOutputDeviceError* Original;

	TWeakObjectPtr<UPrFirebaseCrashlyticsModule> CrashlyticsModule;

public:
	FCrashlyticsOutputDeviceError(FOutputDeviceError* InOriginal, TWeakObjectPtr<UPrFirebaseCrashlyticsModule> InCrashlyticsModule)
		: Original(InOriginal)
		, CrashlyticsModule(InCrashlyticsModule)
	{
		check(InOriginal);
	}

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		if (CrashlyticsModule.IsValid())
		{
			CrashlyticsModule->Log(true, V, Verbosity, Category);
		}

		Original->Serialize(V, Verbosity, Category);
	}

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category, const double Time) override
	{
		if (CrashlyticsModule.IsValid())
		{
			CrashlyticsModule->Log(true, V, Verbosity, Category);
		}

		Original->Serialize(V, Verbosity, Category, Time);
	}

	virtual void Flush() override
	{
		Original->Flush();
	}

	virtual void TearDown() override
	{
		Original->TearDown();
	}

	virtual void Dump(class FArchive& Ar) override
	{
		Original->Dump(Ar);
	}

	virtual bool IsMemoryOnly() const override
	{
		return Original->IsMemoryOnly();
	}

	virtual bool CanBeUsedOnAnyThread() const override
	{
		return Original->CanBeUsedOnAnyThread();
	}

	virtual void HandleError() override
	{
		Original->HandleError();
	}
};

void UPrFirebaseCrashlyticsModule::WriteError(const FString& Log)
{
	WriteError(Log, 0, TMap<FString, FString>{});
}

void UPrFirebaseCrashlyticsModule::CatchEngineLogs()
{
	check(GLog);
	check(GError);

	static FCrashlyticsOutputDeviceError ErrorOutput(GError, this);
	GError = &ErrorOutput;

	const auto PlatformOutputDevices = FPlatformOutputDevices::GetLog();
	check(PlatformOutputDevices);
	check(GLog->IsRedirectingTo(PlatformOutputDevices));
	GLog->RemoveOutputDevice(PlatformOutputDevices);

	static FCrashlyticsOutputDevice DefaultOutput(PlatformOutputDevices, this);
	GLog->AddOutputDevice(&DefaultOutput);

	AddAttribute(TEXT("Start Time"), FDateTime::UtcNow().ToString());

	const bool bSendNotes = GetDefault<UPrFirebaseSettings>()->bFirebaseCrashlytics_SendNotes;
	if (bSendNotes)
	{
		WriteLog(TEXT("Note: Engine log catcher launched!"));
#if WITH_FIREBASE_SYMBOLS_WARNING
#if PLATFORM_IOS
		WriteLog(TEXT("Note: Add 'bGeneratedSYMBundle=True' and 'bGeneratedSYMFile=True' to your 'DefaultEngine.ini' (section: /Script/IOSRuntimeSettings.IOSRuntimeSettings) for crash symbolization"));
#endif // PLATFORM_IOS
#endif // WITH_FIREBASE_SYMBOLS_WARNING
#if NO_LOGGING
		WriteLog(TEXT("Note: Add 'bUseLoggingInShipping = true;' to your Project.Target.cs for extended log"));
#endif // NO_LOGGING
#if !DO_CHECK
		WriteLog(TEXT("Note: Add 'bUseChecksInShipping = true;' to your Project.Target.cs for extended log"));
#endif // !DO_CHECK
#if !DO_BLUEPRINT_GUARD
		WriteLog(TEXT("Note: Add 'GlobalDefinitions.Add(\"DO_BLUEPRINT_GUARD=1\");' to your Project.Target.cs for extended log"));
#endif // !DO_BLUEPRINT_GUARD
	}
}

FString UPrFirebaseCrashlyticsModule::CrashlyticsLogFormat(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category)
{
	if (Category == NAME_None)
	{
		return FString::Printf(TEXT("%s"), V);
	}
	else if (Verbosity > ELogVerbosity::Type::NoLogging)
	{
		return FString::Printf(TEXT("%s: %s: %s"), *Category.ToString(), FOutputDeviceHelper::VerbosityToString(Verbosity), V);
	}
	else
	{
		return FString::Printf(TEXT("%s: %s"), *Category.ToString(), V);
	}
}

void UPrFirebaseCrashlyticsModule::WriteBlueprintCallstack()
{
#if DO_BLUEPRINT_GUARD
	const FBlueprintContextTracker* BlueprintExceptionTracker = FBlueprintContextTracker::TryGet();
	if (BlueprintExceptionTracker && BlueprintExceptionTracker->GetScriptStack().Num() > 0)
	{
		FString ScriptStack = FString::Printf(TEXT("Script Stack (%d frames):\n"), BlueprintExceptionTracker->GetScriptStack().Num());
		for (int32 FrameIdx = BlueprintExceptionTracker->GetScriptStack().Num() - 1; FrameIdx >= 0; --FrameIdx)
		{
			ScriptStack += BlueprintExceptionTracker->GetScriptStack()[FrameIdx]->GetStackDescription() + TEXT("\n");
		}

		WriteLog(ScriptStack);
	}
#endif
}

void UPrFirebaseCrashlyticsModule::Log(bool bCritical, const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category)
{
	static uint8 RecursionCounter = 0;
	if (RecursionCounter > 0)
	{
		WriteError(TEXT("Crashlytics internal recursion!"));
		return;
	}

	++RecursionCounter;

	if (bCritical)
	{
		WriteBlueprintCallstack();
		WriteError(FString::Printf(TEXT("Critical: %s"), *CrashlyticsLogFormat(V, Verbosity, Category)));
	}
	else
	{
		static EPrFirebaseLogLevel DefaultLogLevel = GetDefault<UPrFirebaseSettings>()->FirebaseCrashlytics_LogLevel;
		static EPrFirebaseLogLevel ErrorLogLevel = GetDefault<UPrFirebaseSettings>()->FirebaseCrashlytics_ErrorLogLevel;
		static TSet<FName> ForcedLogCategories = TSet<FName>(GetDefault<UPrFirebaseSettings>()->FirebaseCrashlytics_ForcedLogCategories);
		static TSet<FName> IgnoredLogCategories = TSet<FName>(GetDefault<UPrFirebaseSettings>()->FirebaseCrashlytics_IgnoredLogCategories);

#if WITH_FIREBASE_REMOTECONFIG
		static const FString ForcedLogCategoriesParameterName = TEXT("pr-forced-log-categories");
		static const FString IgnoredLogCategoriesParameterName = TEXT("pr-ignored-log-categories");
		static const FString LogLevelParameterName = TEXT("pr-log-level");
		static const FString ErrorLogLevelParameterName = TEXT("pr-error-log-level");
		static bool bRemoteConfigReady = false;

		UPrFirebaseRemoteConfigModule* RemoteConfigModule = UPrFirebaseLibrary::GetFirebaseProxy()->GetRemoteConfigModule();
		if (!bRemoteConfigReady && RemoteConfigModule->IsFetched())
		{
			// Forced log categories
			if (RemoteConfigModule->HasValue(ForcedLogCategoriesParameterName))
			{
				FString LogCategoriesRaw = TEXT("");
				RemoteConfigModule->GetStringValue(ForcedLogCategoriesParameterName, LogCategoriesRaw);

				TArray<FString> LogCategoriesRawArray;
				LogCategoriesRaw.ParseIntoArray(LogCategoriesRawArray, TEXT(","), true);

				for (FString& LogCategoryRaw : LogCategoriesRawArray)
				{
					LogCategoryRaw.TrimStartAndEndInline();
					if (LogCategoryRaw.Len() > 0)
					{
						ForcedLogCategories.Add(FName(*LogCategoryRaw));
					}
				}
			}

			// Ignored log categories
			if (RemoteConfigModule->HasValue(IgnoredLogCategoriesParameterName))
			{
				FString LogCategoriesRaw = TEXT("");
				RemoteConfigModule->GetStringValue(IgnoredLogCategoriesParameterName, LogCategoriesRaw);

				TArray<FString> LogCategoriesRawArray;
				LogCategoriesRaw.ParseIntoArray(LogCategoriesRawArray, TEXT(","), true);

				for (FString& LogCategoryRaw : LogCategoriesRawArray)
				{
					LogCategoryRaw.TrimStartAndEndInline();
					if (LogCategoryRaw.Len() > 0)
					{
						IgnoredLogCategories.Add(FName(*LogCategoryRaw));
					}
				}
			}

			if (RemoteConfigModule->HasValue(LogLevelParameterName))
			{
				static const UEnum* EnumType = StaticEnum<EPrFirebaseLogLevel>();
				if (EnumType)
				{
					FString LogLevelStringRaw = EnumType->GetNameStringByValue(static_cast<int64>(DefaultLogLevel));
					RemoteConfigModule->GetStringValue(LogLevelParameterName, LogLevelStringRaw);
					LogLevelStringRaw.TrimStartAndEndInline();
					const int64 EnumValue = EnumType->GetValueByNameString(LogLevelStringRaw, EGetByNameFlags::None);
					if (EnumValue != INDEX_NONE)
					{
						DefaultLogLevel = static_cast<EPrFirebaseLogLevel>(EnumValue);
					}
				}
			}

			if (RemoteConfigModule->HasValue(ErrorLogLevelParameterName))
			{
				static const UEnum* EnumType = StaticEnum<EPrFirebaseLogLevel>();
				if (EnumType)
				{
					FString LogLevelStringRaw = EnumType->GetNameStringByValue(static_cast<int64>(ErrorLogLevel));
					RemoteConfigModule->GetStringValue(ErrorLogLevelParameterName, LogLevelStringRaw);
					LogLevelStringRaw.TrimStartAndEndInline();
					const int64 EnumValue = EnumType->GetValueByNameString(LogLevelStringRaw, EGetByNameFlags::None);
					if (EnumValue != INDEX_NONE)
					{
						ErrorLogLevel = static_cast<EPrFirebaseLogLevel>(EnumValue);
					}
				}
			}

			bRemoteConfigReady = true;
		}
#endif

		const uint8 VerbosityValue = static_cast<uint8>(Verbosity);
		if (VerbosityValue != static_cast<uint8>(EPrFirebaseLogLevel::NoLogging))
		{
			const uint8 LogLevelValue = static_cast<uint8>(DefaultLogLevel);
			const uint8 ErrorLogLevelValue = static_cast<uint8>(ErrorLogLevel);

			if (ForcedLogCategories.Contains(Category))
			{
				if (ErrorLogLevelValue >= VerbosityValue)
				{
					WriteBlueprintCallstack();
					const auto Msg = CrashlyticsLogFormat(V, Verbosity, Category);
					WriteLog(Msg);
					WriteError(Msg);
				}
				else
				{
					WriteLog(CrashlyticsLogFormat(V, Verbosity, Category));
				}
			}
			else if (IgnoredLogCategories.Contains(Category))
			{
				// Do nothing
			}
			else if (ErrorLogLevelValue >= VerbosityValue)
			{
				WriteBlueprintCallstack();
				const auto Msg = CrashlyticsLogFormat(V, Verbosity, Category);
				WriteLog(Msg);
				WriteError(Msg);
			}
			else if (LogLevelValue >= VerbosityValue)
			{
				WriteLog(CrashlyticsLogFormat(V, Verbosity, Category));
			}
		}
	}

	--RecursionCounter;
}
