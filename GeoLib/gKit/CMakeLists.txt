cmake_minimum_required(VERSION 3.8)
project(gKit)
set(CMAKE_CXX_STANDARD 11)

INCLUDE(FindPkgConfig)

include_directories(include)
file( GLOB GKIT_SOURCES src/*.cpp )

add_library(gKit ${GKIT_SOURCES})
