// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#include "PrFirebaseProxy.h"

#include "PrFirebaseDefines.h"
#include "PrFirebaseSettings.h"

UPrFirebaseProxy::UPrFirebaseProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bInitialized(false)
{
}

UPrFirebaseCrashlyticsModule* UPrFirebaseProxy::GetCrashlyticsModule() const
{
	check(bInitialized);
	return CastChecked<UPrFirebaseCrashlyticsModule>(Modules.FindChecked(UPrFirebaseCrashlyticsModule::StaticClass()));
}

UPrFirebaseRemoteConfigModule* UPrFirebaseProxy::GetRemoteConfigModule() const
{
	check(bInitialized);
	return CastChecked<UPrFirebaseRemoteConfigModule>(Modules.FindChecked(UPrFirebaseRemoteConfigModule::StaticClass()));
}

UPrFirebasePerformanceModule* UPrFirebaseProxy::GetPerformanceModule() const
{
	check(bInitialized);
	return CastChecked<UPrFirebasePerformanceModule>(Modules.FindChecked(UPrFirebasePerformanceModule::StaticClass()));
}

UPrFirebaseAuthModule* UPrFirebaseProxy::GetAuthModule() const
{
	check(bInitialized);
	return CastChecked<UPrFirebaseAuthModule>(Modules.FindChecked(UPrFirebaseAuthModule::StaticClass()));
}

void UPrFirebaseProxy::InitializeFirebase()
{
	if (!bInitialized)
	{
		GetDefault<UPrFirebaseSettings>();
		InitializeModuleList();
		CreateModules();
		Initialize();
		bInitialized = true;
	}
	else
	{
		UE_LOG(LogFirebase, Fatal, TEXT("Firebase already initialized"));
	}
}

void UPrFirebaseProxy::SetUserIdentifier(const FString& UserIdentifier)
{
	check(bInitialized);
	GetCrashlyticsModule()->SetUserIdentifier(UserIdentifier);
	GetPerformanceModule()->SetAttributeForAllTraces("user_identifier", UserIdentifier);
}

void UPrFirebaseProxy::InitializeModuleList()
{
	ModuleClasses.Add(UPrFirebaseRemoteConfigModule::StaticClass(), UPrFirebaseRemoteConfigModule::StaticClass());
	ModuleClasses.Add(UPrFirebaseCrashlyticsModule::StaticClass(), UPrFirebaseCrashlyticsModule::StaticClass());
	ModuleClasses.Add(UPrFirebasePerformanceModule::StaticClass(), UPrFirebasePerformanceModule::StaticClass());
	ModuleClasses.Add(UPrFirebaseAuthModule::StaticClass(), UPrFirebaseAuthModule::StaticClass());
}

void UPrFirebaseProxy::Initialize()
{
	PreInitializeModules_AnyThread();
	InitializeModules_AnyThread();
}

void UPrFirebaseProxy::PreInitializeModules_AnyThread()
{
	auto Keys = GetSortedModuleClasses();

	for (auto Key : Keys)
	{
		Modules.FindChecked(Key)->PreInitialize_AnyThread();
	}
}

void UPrFirebaseProxy::InitializeModules_AnyThread()
{
	auto Keys = GetSortedModuleClasses();

	for (auto Key : Keys)
	{
		Modules.FindChecked(Key)->Initialize_AnyThread();
	}

	for (auto Key : Keys)
	{
		Modules.FindChecked(Key)->PostInitialize_AnyThread();
	}
}

void UPrFirebaseProxy::CreateModules()
{
	for (auto& Pair : ModuleClasses)
	{
		auto InterfaceClass = Pair.Key;
		auto ModuleClass = Pair.Value;

		auto Interface = GetDefault<UPrFirebaseModule>(InterfaceClass);
		if (Interface->IsAvailable())
		{
			auto Module = NewObject<UPrFirebaseModule>(this, ModuleClass);
			Modules.Add(InterfaceClass, Module);
		}
		else
		{
			auto Module = NewObject<UPrFirebaseModule>(this, InterfaceClass);
			Modules.Add(InterfaceClass, Module);
		}
	}
}

TArray<UClass*> UPrFirebaseProxy::GetSortedModuleClasses()
{
	TArray<UClass*> Keys;
	Keys.Add(UPrFirebaseRemoteConfigModule::StaticClass());
	Keys.Add(UPrFirebaseCrashlyticsModule::StaticClass());
	Keys.Add(UPrFirebasePerformanceModule::StaticClass());

	for (auto& Pair : Modules)
	{
		Keys.AddUnique(Pair.Key);
	}

	return Keys;
}
