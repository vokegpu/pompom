cmake_minimum_required(VERSION 3.9)
project(pompom)

include_directories(src)
add_subdirectory(thirdparty/SDL2)

find_library(SDL2 SDL2)
add_library(SDL2 SHARED)

add_subdirectory(thirdparty/ekg)
find_library(ekg ekg)
add_library(ekg SHARED)

target_sources(pompom PRIVATE src/main.cpp)
target_link_libraries(pompom SDL2 GLESv3)
