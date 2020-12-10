// Copyright 2020 Nikolay Prudnikov. All Rights Reserved.

#include "PrFirebase.h"
#include "PrFirebaseDefines.h"
#include "PrFirebaseProxy.h"
#include "PrFirebaseSettings.h"

#if PLATFORM_IOS
#include "iOS/PrFirebase_iOS.h"
#elif PLATFORM_ANDROID
#include "Android/PrFirebase_Android.h"
#endif

#include "Misc/ConfigCacheIni.h"
#include "UObject/Package.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "PrFirebase"

class FPrFirebase : public IPrFirebase
{
	virtual void StartupModule() override
	{
		Settings = NewObject<UPrFirebaseSettings>(GetTransientPackage(), "PrFirebaseSettings", RF_Standalone);
		Settings->AddToRoot();

		GConfig->GetBool(TEXT("/Script/PrFirebase.PrFirebaseSettings"), TEXT("bFirebaseEnable"), Settings->bFirebaseEnable, GEngineIni);

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "PrFirebase",
				LOCTEXT("FirebaseSettingsName", "Firebase"),
				LOCTEXT("FirebaseSettingsDescription", "Settings for firebase"),
				Settings);
		}

#if PLATFORM_IOS
		UClass* PlatformClass = UPrFirebase_iOS::StaticClass();
#elif PLATFORM_ANDROID
		UClass* PlatformClass = UPrFirebase_Android::StaticClass();
#else
		UClass* PlatformClass = UPrFirebaseProxy::StaticClass();
#endif

		FirebaseProxy = NewObject<UPrFirebaseProxy>(GetTransientPackage(), PlatformClass);
		FirebaseProxy->AddToRoot();
		FirebaseProxy->InitializeFirebase();
	}

	virtual void ShutdownModule() override
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "PrFirebase");
		}

		if (!GExitPurge)
		{
			FirebaseProxy->RemoveFromRoot();
			Settings->RemoveFromRoot();
		}
		else
		{
			FirebaseProxy = nullptr;
			Settings = nullptr;
		}
	}

private:
	UPrFirebaseSettings* Settings;
};

IMPLEMENT_MODULE(FPrFirebase, PrFirebase)

#undef LOCTEXT_NAMESPACE
