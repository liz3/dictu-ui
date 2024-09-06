#!/bin/sh
cd build
rm -rf WeatherTest.app
mkdir -p WeatherTest.app/Contents/MacOS
mkdir -p WeatherTest.app/Contents/assets/build
cp ../tests/info.plist WeatherTest.app/Contents/Info.plist
cp ../tests/location.du WeatherTest.app/Contents/assets/build
cp libdictu-ui.dylib WeatherTest.app/Contents/assets/build
cp /Users/liz3/Projects/Dictu/dictu WeatherTest.app/Contents/assets
cp ../mod.du WeatherTest.app/Contents/assets
cp ../tests/weathertest.sh WeatherTest.app/Contents/MacOS/WeatherTest
chmod +x WeatherTest.app/Contents/MacOS/WeatherTest
