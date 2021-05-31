#!/bin/bash

set -e
TEAM_ID=$(more ~/Developer/mac_id)

if [ "$1" == "help" ]; then
  echo "Run bash ios_build.sh build clean"
  echo "Run bash ios_build.sh version"
  echo "Go to Xcode Archive Organizer and upload!"
  exit
fi

if [ "$1" == "build" ] || [ "$1" == "configure" ]; then
echo "Running CMake configuration..."

# clean up old builds
if [ "$2" == "clean" ]; then rm -Rf build-ios; fi

# generate new builds
cmake -Bbuild-ios -GXcode -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=11.4 \
    -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM="$TEAM_ID" \
    -DCMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY="1,2" \
    -DCMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE="NO"

# if [ "$1" == "build" ]; then
# xcodebuild -project build-ios/CHOWTapeModel.xcodeproj \
#   -scheme CHOWTapeModel_Standalone archive -configuration Release \
#   -sdk iphoneos -jobs 12 -archivePath CHOWTapeModel.xcarchive | xcpretty
# fi
fi
