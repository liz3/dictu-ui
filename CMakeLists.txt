cmake_minimum_required(VERSION 3.16.3)
PROJECT("dictu-ui")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(INCLUDE_WINDOW_API "Include the window api(GLFW Wrapper)" ON)
option(INCLUDE_MAC_LOCATION_API "Include the macOS api to get the locatioon" OFF)

set(DICTU_UI_SOURCES src/dictu-ui.c src/la.c src/skia-wrapper.cc third-party/lodepng/lodepng.c)

if(INCLUDE_WINDOW_API)
 list(APPEND DICTU_UI_SOURCES
  third-party/glad/glad.c)
endif()

if(APPLE AND INCLUDE_MAC_LOCATION_API)
 list(APPEND DICTU_UI_SOURCES src/mac_location.mm)
endif()

add_library(dictu-ui SHARED ${DICTU_UI_SOURCES})

target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/dictu/")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/lodepng")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/skia")

if(INCLUDE_WINDOW_API)
  target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/glad/include/")
  target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/glfw/include")
  add_subdirectory(third-party/glfw)
  add_definitions(-DDICTU_UI_WINDOW_API)
  target_link_libraries(dictu-ui PUBLIC glfw)
endif()


if(APPLE)
if(INCLUDE_MAC_LOCATION_API)
    add_definitions(-DDICTU_UI_MAC_LOCATION)
        target_link_libraries(dictu-ui PUBLIC "-framework CoreLocation")
endif()
    target_link_libraries(dictu-ui PUBLIC "-framework Cocoa")
endif()
if(WIN32)
    target_link_libraries(dictu-ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third-party/skia/out/Release/skia.dll.lib")
else()
target_link_libraries(dictu-ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third-party/skia/out/Release/libskia.a")
endif()

if(UNIX AND NOT APPLE)
 target_link_libraries(dictu-ui PUBLIC "-lfontconfig")
endif()