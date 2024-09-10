# Dictu ui
This is a port of [bun-ui](https://github.com/liz3/bun-ui.git) to [Dictu](https://dictu-lang.com).

This Port replaces node-canvas with a wrapped version of skia.
So it does not only forward a GLFW api but also a small wrapper around Skia.

# Building
Clone the Repository: `git clone --recurse-submodules https://github.com/liz3/dictu-ui.git`

**Note:** You need a version of dictu build from Dictus [develop branch](https://github.com/dictu-lang/Dictu/tree/develop) because the FFI API is not currently in a release.
## Prerequisites
* Python3
* On windows: visual studio(MSBuild/msvc).
* On mac and linux: a recent version of LLVM.

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
### MacOS
Generate the Build files
```
bin/gn gen out/Release --args='is_official_build=true skia_enable_tools=false skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false extra_cflags_cc=["-frtti"] skia_use_system_zlib=false'
```
If you are on Apple sillicon append `target_cpu="arm64"` to `--args=`

Build:
```
third_party/ninja/ninja -C out/Release
```

### Linux

Generate the Build files
```
bin/gn gen out/Release --args='is_official_build=true skia_enable_tools=false skia_use_system_libjpeg_turbo=false skia_use_system_libwebp=false skia_use_system_libpng=false skia_use_system_icu=false skia_use_system_harfbuzz=false extra_cflags_cc=["-frtti"] skia_use_system_zlib=false skia_use_freetype=true skia_use_system_freetype2=false skia_enable_ganesh=false'
```
**Note you will need fontconfig installed!**
Build:
```
third_party/ninja/ninja -C out/Release
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

# Usage
Import `mod.du` from the root and use it!

# Docs
Here is a list with exposed functions
```
import "mod.du" as DictuUI;
ColorType = Number(0 = RGBA, 1 = RGB, 2 = BGRA)
DictuUI.getColor(r: Number, g: Number, r: Number, a:Number=255) -> Dict(Color)
DictuUI.getRandomColor() -> Dict(Color)
DictuUI.toWindow(title: string, canvas: SkiaCanvas) -> Window
DictuUI.waitWindows(windows: List[Window]) -> void
DictuUI.plot(width: Number, height: Number, entries: List[List[Number]], opts: Dict?) -> SkiaCanvas
DictuUI.pie(width: Number, height: Number, entries: List[List[Number], name: string?]) -> SkiaCanvas
DictuUI.graph(name: string, width: Number, height: Number, entries: List[List[List[Number], name: ?string]], opts: Dict?) -> SkiaCanvas

DictuUI.mod.skiaSurface(width: number, height: number) -> SkiaCanvas
DictuUI.mod.createWindow(title: string, width: Number, height: Number) -> Window
DictuUI.mod.decodePng(data: string) -> Dict {data: string, width: Number, height: Number} // color type is RGBA
DictuUI.mod.encodePng(data: string, width: Number, height: Number) -> string

#IF MAC_LOCATION
DictuUI.mod.getLocation() -> Dict {res: bool, lat: Number?, lng: Number?}
#ENDIF

Window.copyBuffer(data: string, width: Number, height: Number, colorType: ColorType)
Window.render() -> void
Window.close() -> void
Window.requestFocus() -> void
Window.show(show: bool) -> void
Window.keyState(key: Number) -> Number //(Check GLFW key defs and key state)
Window.shouldClose() -> bool
Window.setClearColor(color: Dict(Color)) -> void
Window.pollEvents() -> List[Dict]
Window.waitEvents() -> void
Window.waitEventsTimeout(timeout: Number) -> void

SkiaCanvas.drawLine(xStart: Number, yStart: Number, xEnd: Number, yEnd: Number, width: Number, color: Dict(Color)) -> void
SkiaCanvas.drawRect(xStart: Number, yStart: Number, width: Number, height: Number, border_radius: Number, color: Dict(Color)) -> void
SkiaCanvas.drawRectOutline(xStart: Number, yStart: Number, width: Number, height: Number, stroke_width: Number, border_radius: Number, color: Dict(Color)) -> void
SkiaCanvas.drawText(x: Number, y: Number, content: string, font_size: Number, color: Dict(Color)) -> void
SkiaCanvas.drawTextWithFont(x: Number, y: Number, content: string, fontName: string, font_size: Number, color: Dict(Color)) -> void
SkiaCanvas.measureText(content: string, font_size: Number) -> Number
SkiaCanvas.measureTextWithFont(content: string, font_name: string, font_size: Number) -> Number
SkiaCanvas.render() -> string // BGRA pixels of rendered buffer, size = canvas.width * canvas.height * 4(SkiaCanvas render returns ColorType 2(BGRA))
SkiaCanvas.clear(color: Dict(Color)) -> void
SkiaCanvas.drawPath(path: SkiaPath, color: Dict(Color)) -> void // Path is not usable after this
SkiaCanvas.drawPathStroke(path: SkiaPath, stroke_width: Number, color: Dict(Color)) -> void // Path is not usable after this
SkiaCanvas.createPath() -> SkiaPath
SkiaCanvas.drawBuffer(data: string, buffer_width: Number, buffer_height: Number, x: Number, y: Number, width: Number, height: Number, color_type: ColorType = 0)
SkiaCanvas.rotate(angle: Number) -> void
SkiaCanvas.translate(x: Number, y: Number) -> void

SkiaCanvas.width: Number
SkiaCanvas.height: Number

SkiaPath.arcTo(x: Number, y: Number, width: Number, height: Number, startAngle: Number, sweepAngle: Number) -> void
SkiaPath.moveTo(x: Number, y: Number) -> void
SkiaPath.lineTo(x: Number, y: Number) -> void
```