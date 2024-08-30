# Dictu ui
This is a port of [bun-ui](https://github.com/liz3/bun-ui.git) to [Dictu](https://dictu-lang.com).

This Port replaces node-canvas with a wrapped version of skia.
So it does not only forward a GLFW api but also a small wrapper around Skia.

# Building
Clone the Repository: `git clone --recurse-submodules https://github.com/liz3/dictu-ui.git`
## Prerequisites
* Python3
* On windows: visual studio(MSBuild/msvc).
* On mac and linux: a recent version of LLVM.

**Note:** You need a dictu build of my fork(branch develop) because it contains patches for the FFI api, not yet merged upstream.
## Building Skia
Skia has no Cmake support so we need to build it seperatedly.
`cd third-party/skia`

```
python3 tools/git-sync-deps
python3 bin/fetch-ninja
python3 bin/fetch-gn
```
### Windows
Generate the Build files
```
bin\gn gen out\Release --args="is_official_build=true is_component_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false skia_use_system_zlib=false skia_use_expat=false skia_enable_ganesh=false"
```
Build:
```
third_party\ninja\ninja.exe -C out\Release
```
### MacOS/Linux
Generate the Build files
```
bin/gn gen out/Release --args='is_official_build=true skia_enable_tools=true skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false extra_cflags_cc=["-frtti"] skia_use_system_zlib=false'
```
If you are on Apple sillicon append `target_cpu="arm64"` to `--args=`

Build:
```
third_party\ninja\ninja -C out\Release
```

Building skia will take a while.
## Building dictu-ui
`cd` back into the repository root.

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```
### Windows
First build the library:
`cmake --build . --config Release`

Then you need to copy the DLL's from `third-party\skia\out\Release` into `build\Release`. That is required for it to run.

### MacOS/Linux
Build the library: `make -j`

## Usage
Import `mod.du` from the root and use it!