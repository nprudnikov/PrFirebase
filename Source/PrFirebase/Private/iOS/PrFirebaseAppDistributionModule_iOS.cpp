// Copyright 2020-2021 Vladimir Alyamkin. All Rights Reserved.

#include "iOS/PrFirebaseAppDistributionModule_iOS.h"

#include "PrFirebaseDefines.h"

#if WITH_FIREBASE_APPDISTRIBUTION && PLATFORM_IOS
#include "IOSAppDelegate.h"

#import <Firebase.h>

void UPrFirebaseAppDistributionModule_iOS::CheckForUpdate()
{
	dispatch_async(dispatch_get_main_queue(), ^{
		// Sign in a tester without automatically checking for update
		if(![[FIRAppDistribution appDistribution] isTesterSignedIn]) {
			[[FIRAppDistribution appDistribution]
				signInTesterWithCompletion:^(NSError *_Nullable error) {
					if (error) {
						NSLog(@"Firebase SignIn failed: %@", error.localizedDescription);
						return;
					}
			}];
		}

		// Only check for update if tester is already signed in - do not prompt
		if([[FIRAppDistribution appDistribution] isTesterSignedIn]) {
			[[FIRAppDistribution appDistribution]
				checkForUpdateWithCompletion:^(FIRAppDistributionRelease *_Nullable release,
											   NSError *_Nullable error) {
					if (error) {
						UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Check For Update"
													message:[NSString stringWithFormat:@"Error during tester sign in! %@", error.localizedDescription]
													preferredStyle:UIAlertControllerStyleAlert];

						UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"Ok" style:UIAlertActionStyleDefault
													handler:^(UIAlertAction *action) {}];

						[alert addAction:okAction];
						[[IOSAppDelegate GetDelegate].IOSController presentViewController:alert animated:YES completion:nil];

						return;
					}

					if (release) {
						UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"New Version Available"
													message:[NSString stringWithFormat:@"Version %@ (%@) is available.", release.displayVersion,
													release.buildVersion] preferredStyle:UIAlertControllerStyleAlert];

						UIAlertAction *updateAction = [UIAlertAction actionWithTitle:@"Update"
														style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
							[[UIApplication sharedApplication] openURL:release.downloadURL options:@{} completionHandler:nil];
						}];

						UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel"
														style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {}];

						[alert addAction:updateAction];
						[alert addAction:cancelAction];

						[[IOSAppDelegate GetDelegate].IOSController presentViewController:alert animated:YES completion:nil];
					}
			}];
		}
	});
}

#endif // WITH_FIREBASE_CRASHLYTICS && PLATFORM_IOS
