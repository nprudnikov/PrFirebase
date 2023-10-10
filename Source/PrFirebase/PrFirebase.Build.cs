// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

using System.IO;
using Tools.DotNETCommon;

namespace UnrealBuildTool.Rules
{
    public class PrFirebase : ModuleRules
    {
        public PrFirebase(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			ConfigHierarchy EngineConfig = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, DirectoryReference.FromFile(Target.ProjectFile), Target.Platform);

            bool bFirebaseLoggingInShipping = false;
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseLoggingInShipping", out bFirebaseLoggingInShipping);

            bool bFirebaseEnable = false;
            bool bFirebaseCrashlyticsEnable = false;
            bool bFirebaseRemoteConfigEnable = false;
            bool bFirebaseAuthEnable = false;
            bool bFirebasePerformanceEnable = false;
            bool bFirebaseAppDistributionEnable = false;
            
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseEnable", out bFirebaseEnable);
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseCrashlyticsEnable", out bFirebaseCrashlyticsEnable);
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseRemoteConfigEnable", out bFirebaseRemoteConfigEnable);
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseAuthEnable", out bFirebaseAuthEnable);
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebasePerformanceEnable", out bFirebasePerformanceEnable);
            EngineConfig.TryGetValue("/Script/PrFirebase.PrFirebaseSettings", "bFirebaseAppDistributionEnable", out bFirebaseAppDistributionEnable);

            PrivateIncludePaths.AddRange(
                new string[] {
                    "PrFirebase/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine",
					"RenderCore",
					"RHI"
				});

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings",
                }
            );

            bool bSymbolsWarning = false;

            if (bFirebaseEnable)
            {
                if (Target.Platform == UnrealTargetPlatform.IOS)
                {
                	bool bGeneratedSYMBundle = false;
                	bool bGeneratedSYMFile = false;
            		EngineConfig.TryGetValue("/Script/IOSRuntimeSettings.IOSRuntimeSettings", "bGeneratedSYMBundle", out bGeneratedSYMBundle);
            		EngineConfig.TryGetValue("/Script/IOSRuntimeSettings.IOSRuntimeSettings", "bGeneratedSYMFile", out bGeneratedSYMFile);

            		if ((!bGeneratedSYMBundle || !bGeneratedSYMFile) && bFirebaseCrashlyticsEnable) 
            		{
            			bSymbolsWarning = true;
            		}

					/* Firebase / Firebase Analytics */
					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseAnalytics",
							"../../ThirdParty/iOS/FirebaseAnalytics.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseCore",
							"../../ThirdParty/iOS/FirebaseCore.embeddedframework.zip"
						)
					);

					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseCoreExtension",
							"../../ThirdParty/iOS/FirebaseCoreExtension.embeddedframework.zip"
						)
					);

					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseCoreInternal",
							"../../ThirdParty/iOS/FirebaseCoreInternal.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseInstallations",
							"../../ThirdParty/iOS/FirebaseInstallations.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"GoogleAppMeasurement",
							"../../ThirdParty/iOS/GoogleAppMeasurement.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"GoogleDataTransport",
							"../../ThirdParty/iOS/GoogleDataTransport.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"GoogleUtilities",
							"../../ThirdParty/iOS/GoogleUtilities.embeddedframework.zip"
						)
					);
					
					PublicAdditionalFrameworks.Add(
						new Framework(
							"nanopb",
							"../../ThirdParty/iOS/nanopb.embeddedframework.zip"
						)
					);

					PublicAdditionalFrameworks.Add(
						new Framework(
							"FirebaseSessions",
							"../../ThirdParty/iOS/FirebaseSessions.embeddedframework.zip"
						)
					);

					PublicAdditionalFrameworks.Add(
						new Framework(
							"PromisesSwift",
							"../../ThirdParty/iOS/PromisesSwift.embeddedframework.zip"
						)
					);
					
					/* Crashlytics */
					if (bFirebaseCrashlyticsEnable)
					{
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseCrashlytics",
								"../../ThirdParty/iOS/FirebaseCrashlytics.embeddedframework.zip"
							)
						);
                    }
                    
                    /* Remote Config */
                    if (bFirebaseRemoteConfigEnable)
                    {
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseABTesting",
								"../../ThirdParty/iOS/FirebaseABTesting.embeddedframework.zip"
							)
						);
						
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseRemoteConfig",
								"../../ThirdParty/iOS/FirebaseRemoteConfig.embeddedframework.zip"
							)
						);
						
						PublicAdditionalFrameworks.Add(
							new Framework(
								"Protobuf",
								"../../ThirdParty/iOS/Protobuf.embeddedframework.zip"
							)
						);
                    }
                    
                    /* Performance */
                    if (bFirebasePerformanceEnable)
                    {
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseABTesting",
								"../../ThirdParty/iOS/FirebaseABTesting.embeddedframework.zip"
							)
						);
						
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebasePerformance",
								"../../ThirdParty/iOS/FirebasePerformance.embeddedframework.zip"
							)
						);
						
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseRemoteConfig",
								"../../ThirdParty/iOS/FirebaseRemoteConfig.embeddedframework.zip"
							)
						);

						PublicAdditionalFrameworks.Add(
							new Framework(
								"Protobuf",
								"../../ThirdParty/iOS/Protobuf.embeddedframework.zip"
							)
						);
                    }
                    
                    /* Auth */
                    if (bFirebaseAuthEnable)
                    {
						PublicAdditionalFrameworks.Add(
							new Framework(
								"FirebaseAuth",
								"../../ThirdParty/iOS/FirebaseAuth.embeddedframework.zip"
							)
						);
						
						PublicAdditionalFrameworks.Add(
							new Framework(
								"GTMSessionFetcher",
								"../../ThirdParty/iOS/GTMSessionFetcher.embeddedframework.zip"
							)
						);
                    }
                    
                    if (bFirebaseAppDistributionEnable)
                    {
                        PublicAdditionalFrameworks.Add(
                            new Framework(
                                "FirebaseAppDistribution",
                                "../../ThirdParty/iOS/FirebaseAppDistribution.embeddedframework.zip"
                            )
                        );
                    }
                    
                    PublicSystemLibraries.Add("sqlite3");
                    
                    PrivateIncludePaths.Add("PrFirebase/External/iOS");

                    string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                    AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "PrFirebase_UPL_IOS.xml"));
                }
                else if (Target.Platform == UnrealTargetPlatform.Android)
                {
                    PublicDependencyModuleNames.AddRange(
                        new string[]
                        {
                           "Launch"
                        });

                    string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                    AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "PrFirebase_UPL_Android.xml"));
                }
                else
                {
					bFirebaseEnable = false;
                }
            }
			
			PublicDefinitions.Add("WITH_FIREBASE=" + (bFirebaseEnable ? "1" : "0"));
			PublicDefinitions.Add("WITH_FIREBASE_CRASHLYTICS=" + (bFirebaseEnable && bFirebaseCrashlyticsEnable ? "1" : "0"));
			PublicDefinitions.Add("WITH_FIREBASE_REMOTECONFIG=" + (bFirebaseEnable && bFirebaseRemoteConfigEnable ? "1" : "0"));
			PublicDefinitions.Add("WITH_FIREBASE_AUTH=" + (bFirebaseEnable && bFirebaseAuthEnable ? "1" : "0"));
			PublicDefinitions.Add("WITH_FIREBASE_PERFORMANCE=" + (bFirebaseEnable && bFirebasePerformanceEnable ? "1" : "0"));
            PublicDefinitions.Add("WITH_FIREBASE_SYMBOLS_WARNING=" + (bFirebaseEnable && bSymbolsWarning ? "1" : "0"));
            PublicDefinitions.Add("WITH_FIREBASE_APPDISTRIBUTION=" + (bFirebaseEnable && bFirebaseAppDistributionEnable ? "1" : "0"));

            PublicDefinitions.Add("FIREBASE_LOGGING=" + (bFirebaseLoggingInShipping ? "1" : "0"));
        }
    }
}
