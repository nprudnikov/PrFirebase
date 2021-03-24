// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#include "Android/PrFirebaseRemoteConfigModule_Android.h"

#if WITH_FIREBASE_REMOTECONFIG && PLATFORM_ANDROID
#include "PrFirebaseDefines.h"
#include "PrFirebaseSettings.h"

#include "Async/Async.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>

#endif // WITH_FIREBASE_REMOTECONFIG && PLATFORM_ANDROID
