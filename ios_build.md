# Building Surge for iOS

To build for an iOS device follow the template
in `ios_build.sh`, replacing the `TEAM_ID` with
your own Apple Developer ID.

## Problems:
- Need to find an installation directory for Surge on iOS, and get the relevant data to that directory (surge-extra-content??)
- Do we need versiontools to set `SURGE_BUILD_ARCH` for iOS?
- AUv3 builds get error:
    > 'sharedApplication' is unavailable: not available on iOS (App Extension) - Use view controller based solutions where appropriate instead. (juce_ios_MessageManager)
- Unable to get Lua support working on iOS
- Unable to build for iOS simulator (not a big deal except for debugging)
- Standalone will compile, but crashes early because of no "SurgePatches.db" file