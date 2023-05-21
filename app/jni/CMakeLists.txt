cmake_minimum_required(VERSION 3.9)
project(pompom)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/SDL2)
find_library(SDL2 SDL2)

add_subdirectory(thirdparty/freetype)
find_library(freetype freetype)

include_directories(src)
include_directories(thirdparty/SDL2/include)
include_directories(thirdparty/freetype/include)

add_subdirectory(thirdparty/ekg)
find_library(ekg ekg)
include_directories(thirdparty/ekg/include)

add_library(main SHARED "src/main.cpp")
target_link_libraries(main ekg SDL2 freetype GLESv3)