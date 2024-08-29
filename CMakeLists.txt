cmake_minimum_required(VERSION 3.16.3)
PROJECT("dictu-ui")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_library(dictu-ui SHARED src/dictu-ui.c src/skia-wrapper.cc src/la.c third-party/glad/glad.c third-party/lodepng/lodepng.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(third-party/glfw)


target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/dictu/")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/glad/include/")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/glfw/include")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/lodepng")
target_include_directories(dictu-ui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third-party/skia")

target_link_libraries(dictu-ui PRIVATE glfw)
if(APPLE)
    target_link_libraries(dictu-ui PUBLIC "-framework CoreText -framework CoreGraphics -framework CoreFoundation")
endif()
target_link_libraries(dictu-ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third-party/skia/out/Release/libskia.a")


