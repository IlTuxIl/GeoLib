cmake_minimum_required(VERSION 3.8)
project(gKit)

set(CMAKE_CXX_STANDARD 11)

INCLUDE(FindPkgConfig)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(include)
file( GLOB GKIT_SOURCES src/*.cpp )

#include_directories(Test ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

add_library(gKit ${GKIT_SOURCES})
