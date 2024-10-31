@echo off
git submodule update --init
cd third-party/skia
python3 tools/git-sync-deps
python3 bin/fetch-ninja
python3 bin/fetch-gn
bin\gn gen out\Release --args="is_official_build=true is_component_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false skia_use_system_zlib=false skia_use_expat=false skia_enable_ganesh=false"
third_party\ninja\ninja.exe -C out\Release
cd ../..
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
copy third-party\skia\out\Release\*.dll build\Release\