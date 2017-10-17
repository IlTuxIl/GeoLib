cmake_minimum_required(VERSION 3.6)
project(GeoLib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

#add_subdirectory(gKit)
find_package(GLEW REQUIRED)

include_directories(gKit/src)
include_directories(include)

file( GLOB GKIT_SOURCES gKit/src/*.cpp)
file( GLOB GEOLIB_SOURCES src/*.cpp include/*.h)
add_library(GeoLib ${GEOLIB_SOURCES} ${GKIT_SOURCES})
