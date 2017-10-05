cmake_minimum_required(VERSION 3.8)
project(GeoLib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(gKit)

file( GLOB GEOLIB_SOURCES src/*.cpp )

add_library(GeoLib ${GEOLIB_SOURCES})
