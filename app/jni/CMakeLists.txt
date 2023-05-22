cmake_minimum_required(VERSION 3.9)
project(pompom)

add_subdirectory(thirdparty/SDL2)
find_library(SDL2 SDL2)

add_subdirectory(thirdparty/freetype)
find_library(freetype freetype)

include_directories(src)
include_directories(thirdparty/SDL2/include)
include_directories(thirdparty/freetype/include)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(thirdparty/ekg)
find_library(ekg ekg)
include_directories(thirdparty/ekg/include)

file(GLOB_RECURSE POMPOM_SOURCE_FILES "src/*.cpp")

add_library(main SHARED ${POMPOM_SOURCE_FILES})
target_link_libraries(main SDL2 GLESv3 ekg freetype log android)